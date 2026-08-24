#include "../hdr/app2.hpp"

void fucn_3(std::string& buffer, unsigned& new_sum){
	if(lib::check_multiple_of_32(buffer, new_sum)){
		std::cout << "Данные получены (app2)" << std::endl;
		std::cout << "Строка: " << buffer << std::endl;
		std::cout << "Сумма: " << new_sum << std::endl;
		std::cout << std::endl;
	} else{
		std::cout << "Ошибка проверки данных (app2)" << std::endl;
		std::cout << std::endl;
	}
}

using namespace std;

int main(){
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(server_fd == -1){
        cerr << "Ошибка создания сокета (app2)" << endl;
        return 1;
	}

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(32929);

	if(bind(server_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
        cerr << "Ошибка bind (app2)" << endl;
        close(server_fd);
        return 1;
    }

	if(listen(server_fd, 1) == -1) {
        cerr << "Ошибка listen (app2)" << endl;
        close(server_fd);
        return 1;
    }

	while(true){
		cout << "Ожидание подключения app1... (app2)" << endl;

		std::this_thread::sleep_for(std::chrono::seconds(2));

		int client_fd = accept(server_fd, NULL, NULL);

		if(client_fd == -1) {
            cerr << "Ошибка accept (app2)" << endl;
            continue;
        }

		cout << "Подключение к app1 прошло успешно (app2)" << endl;

		while(true){
			uint32_t data_size;

			ssize_t bytes = recv(client_fd, &data_size, sizeof(data_size), MSG_WAITALL);
			if(bytes <= 0){
				cerr << "Программа app1 выключена (app2)" << endl;
				break;
			}

			data_size = ntohl(data_size);
			string buffer(data_size, '\0');
			bytes = recv(client_fd, &buffer[0], data_size, MSG_WAITALL);
			if(bytes <= 0){
				cerr << "Программа app1 выключена (app2)" << endl;
				break;
			}

			uint32_t res_sum;
			bytes = recv(client_fd, &res_sum, sizeof(res_sum), MSG_WAITALL);
			if(bytes <= 0){
				cerr << "Программа app1 выключена (app2)" << endl;
				break;
			}

			unsigned new_sum = ntohl(res_sum);
			fucn_3(buffer, new_sum);

		}
	}

	close(server_fd);

	return 0;
}
