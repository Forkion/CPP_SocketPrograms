#include "../hdr/app1.hpp"

app1::app1() : app_gen(), mtx(), buffer_status(false){}

void app1::func_thread1(){
	std::string new_str;
	while(true){
		std::cout << "Введите цифры (записываются первые 64 символа):" << std::endl;
		init(new_str);
		if(new_str.empty()) {
            continue;
        }
		lib::counting_sort(new_str);
		if(new_str == "exit"){
			app_exit = true;
			cv.notify_all();
			break;
		}

		{
			std::lock_guard<std::mutex> lock(mtx);
			buffer = new_str;
			new_str.clear();
			buffer_status = true;
			cv.notify_one();
		}
	}
}

void app1::func_thread2(){
	while(true){
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [this](){return buffer_status || app_exit;});
		if(app_exit){
			break;
		}

		if(socket_fd == -1){
			std::cerr << "Ошибка. Нет соединения с программой 2" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(4));
			// reconnect;
		}

		std::cout << "Запись в буфере: " << buffer << std::endl;

		uint32_t res_sum = lib::sum(buffer);
		uint32_t data_size = htonl(buffer.size());

        if(send(socket_fd, &data_size, sizeof(data_size), MSG_MORE) == -1) {
            error();
            continue;
        }
        if(send(socket_fd, buffer.data(), buffer.size(), MSG_MORE) == -1) {
            error();
            continue;
        }

		res_sum = htonl(res_sum);
        if(send(socket_fd, &res_sum, sizeof(res_sum), 0) == -1) {
            error();
            continue;
        }

		buffer_status = false;
		buffer.clear();
	}
}

void app1::error() noexcept{
		std::cerr << "Ошибка отправки" << std::endl;
		buffer_status = true;
		close(socket_fd);
		socket_fd = -1;
		std::this_thread::sleep_for(std::chrono::seconds(4));
}

using namespace std;
#include <signal.h>
int main(){
	signal(SIGPIPE, SIG_IGN);
	app1 app1;

	app1.set_socket_fd(socket(AF_INET, SOCK_STREAM, 0));

	sockaddr_in server_addr;

	server_addr.sin_family = (AF_INET);
	server_addr.sin_port = htons(32929);  // свободный порт на моей виртуалке
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	int count = 0;
	while(connect(app1.get_socket_fd(), (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		cout << "Ошибка подключения к серверу, попытка переподключения..." << endl;
		std::this_thread::sleep_for(std::chrono::seconds(4));
		++count;
		if(count >= 10){
			cerr << "Не удалось подключиться к серверу" << endl;
			return 1;
		}
	}

	cout << "Подключение к app2 прошло успешно (app1)" << endl;

	thread thread1([&app1](){ app1.func_thread1(); });
	thread thread2([&app1](){ app1.func_thread2(); });

	thread1.join();
	thread2.join();

	close(app1.get_socket_fd());

	return 0;
}
