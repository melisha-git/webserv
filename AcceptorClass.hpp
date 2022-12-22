#pragma once

#include "SocketClass.hpp"
#include <netinet/in.h>
#include <functional>
#include <fcntl.h>
#include <poll.h>

class Acceptor {
private:
	struct sockaddr_in	addr_;
	socklen_t			addrSize_;
	Socket				listen_;
public:


	Acceptor(int port) {
		this->addr_.sin_family = AF_INET;
		this->addr_.sin_port = htons(port);
		this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addrSize_ = sizeof(this->addr_);

		this->_setSockOpt(static_cast<int>(listen_));
		this->_binding(static_cast<int>(listen_));
		this->_listening(static_cast<int>(listen_));
	}

	int getListen() {
		return static_cast<int>(this->listen_);
	}

	void Accept(Socket &socket, std::function<void(int)> t) {
		socket = accept(static_cast<int>(listen_), reinterpret_cast<struct sockaddr*>(&this->addr_), &this->addrSize_);
		t(socket == -1 ? 1 : 0);
	}

private:
	void _binding(const int &socket) {
		if ((bind(socket, reinterpret_cast<struct sockaddr*>(&this->addr_), this->addrSize_)) == -1)
			throw Exception("Error binding connection. Socket already been establishing.\n");
	}

	static void _start() {
	}

	void _listening(const int &socket) {
		if (listen(socket, 128) == -1)
			throw Exception("Error listen socket\n");
	}

	void _setSockOpt(const int &socket) {
		int number = 1;
		if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &number, sizeof(number)) == -1)
			throw Exception("Error set_reuse_addr error\n");
	}
};