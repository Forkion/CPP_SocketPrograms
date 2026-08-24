#include "../hdr/app1.hpp"

app1::app1() : app_gen(), mtx(), buffer_status(false) {}

void app1::func_thread1() {
  std::string new_str;
  while (true) {
    init(new_str);
    if (new_str.empty()) {
      continue;
    }
    lib::counting_sort(new_str);

    {
      std::lock_guard<std::mutex> lock(mtx);
      buffer = new_str;
      new_str.clear();
      buffer_status = true;
      cv.notify_one();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void app1::func_thread2() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [this]() { return buffer_status; });

    if (socket_fd == -1) {
      std::cerr << "Ошибка. Нет соединения с программой 2" << std::endl;
      lock.unlock();
      reconnect();
      lock.lock();
      if (!buffer_status) {
        continue;
      }
    }

    std::cout << "Запись в буфере: " << buffer << std::endl;

    uint32_t res_sum = lib::sum(buffer);
    uint32_t data_size = htonl(buffer.size());

    if (send(socket_fd, &data_size, sizeof(data_size), MSG_MORE) == -1) {
      lock.unlock();
      error();
      lock.lock();
      continue;
    }
    if (send(socket_fd, buffer.data(), buffer.size(), MSG_MORE) == -1) {
      lock.unlock();
      error();
      lock.lock();
      continue;
    }

    res_sum = htonl(res_sum);
    if (send(socket_fd, &res_sum, sizeof(res_sum), 0) == -1) {
      lock.unlock();
      error();
      lock.lock();
      continue;
    }

    buffer_status = false;
    buffer.clear();
    std::cout << std::endl;
  }
}

void app1::error() noexcept {
  std::cerr << "Ошибка отправки" << std::endl;
  {
    std::lock_guard<std::mutex> lock(mtx);
    close(socket_fd);
    socket_fd = -1;
  }
}

bool app1::connect_to_server() {
  int new_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (new_socket == -1) {
    return false;
  }

  sockaddr_in server_addr;
  server_addr.sin_family = (AF_INET);
  server_addr.sin_port = htons(32929);  // свободный порт на моей виртуалке
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  if (connect(new_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    close(new_socket);
    return false;
  }

  {
    std::lock_guard<std::mutex> lock(mtx);
    if (socket_fd != -1) {
      close(socket_fd);
    }
    socket_fd = new_socket;
  }

  std::cout << "Подключение к app2 прошло успешно (app1)" << std::endl;
  return true;
}

void app1::reconnect() {
  while (!connect_to_server()) {
    std::cout << "Сервер недоступен, повтор через 4 секунды..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
  }
}

using namespace std;

int main() {
  signal(SIGPIPE, SIG_IGN);
  app1 app1;

  while (!app1.connect_to_server()) {
    std::cout << "Ожидание сервера..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
  }

  thread thread1([&app1]() { app1.func_thread1(); });
  thread thread2([&app1]() { app1.func_thread2(); });

  thread1.join();
  thread2.join();

  close(app1.get_socket_fd());

  return 0;
}
