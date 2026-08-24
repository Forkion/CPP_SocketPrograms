#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <condition_variable>
#include "../../lib/hdr/lib.hpp"

class app_gen{
protected:
	std::string buffer;
	bool app_exit;
	int socket_fd;

public:
	app_gen();
	app_gen(std::string &new_str);
	app_gen(std::string &&new_str);
	~app_gen() = default;

	void set_buffer(const std::string &new_str);
	void set_app_exit(const bool &new_app_exit) noexcept;
	void set_socket_fd(const int &new_socket_fd) noexcept;

	[[nodiscard]] std::string get_buffer() const noexcept;
	[[nodiscard]] bool get_app_exit() const noexcept;
	[[nodiscard]] int get_socket_fd() const noexcept;

	void init(std::string &str);
};
