#pragma once

#include "SocketClass.hpp"
#include <netinet/in.h>
#include <fcntl.h>

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
		fcntl(static_cast<int>(listen_), F_SETFL, fcntl(static_cast<int>(listen_), F_GETFL) | O_NONBLOCK);
	}

	int getListen() {
		return static_cast<int>(this->listen_);
	}

	void Accept(int &socket, void (function)(int)) {
		socket = accept(static_cast<int>(listen_), reinterpret_cast<struct sockaddr*>(&this->addr_), &this->addrSize_);
		function(socket == -1 ? 1 : 0);
	}

private:
	void _binding(const int &socket) {
		if ((bind(socket, reinterpret_cast<struct sockaddr*>(&this->addr_), this->addrSize_)) == -1)
			throw Exception("Error binding connection. Socket already been establishing.\n");
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