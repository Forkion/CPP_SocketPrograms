#include "../hdr/app.hpp"

	app_gen::app_gen() : buffer(), app_exit(false){}

	app_gen::app_gen(std::string &new_str) : buffer(new_str), app_exit(false){}

	app_gen::app_gen(std::string &&new_str) : app_exit(false){
		buffer = std::move(new_str);
	}

	void app_gen::set_buffer(const std::string &new_str){
		buffer = new_str;
	}

	void app_gen::set_app_exit(const bool &new_app_exit) noexcept{
		app_exit = new_app_exit;
	}

	void app_gen::set_socket_fd(const int &new_socket_fd) noexcept{
		socket_fd = new_socket_fd;
	}

	[[nodiscard]] std::string app_gen::get_buffer() const noexcept{
		return buffer;
	}

	[[nodiscard]] bool app_gen::get_app_exit() const noexcept{
		return app_exit;
	}

	[[nodiscard]] int app_gen::get_socket_fd() const noexcept{
		return socket_fd;
	}

	void app_gen::init(std::string &buffer){
		short size = 0;
		buffer.clear();
		buffer.resize(64);

		char sim = '0';
		bool status_error = false;
		while(true){
			while(sim != '\n'){
				for(short i = short(0); i < short(64); ++i){
					sim = getchar();

					if(sim == '\n'){
						break;
					}

					if(sim < '0' || sim > '9'){
						status_error = true;
						break;
					}

					buffer[i] = sim;
					++size;
				}
			}

			if(status_error){
				std::cout << "Введён неверный символ, принимается строка только из цифр:" << std::endl;
				status_error = false;
				continue;
			}
			break;
		}
		buffer.resize(size);
	}
