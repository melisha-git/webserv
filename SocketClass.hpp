#pragma once

#include "ExceptionClass.hpp"
#include <unistd.h>
#include <sys/socket.h>

class Socket {
protected:
	int	socket_;
public:

	Socket() {
		this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if (this->socket_ == -1)
			throw Exception("Establishing socket error\n");
	}

	Socket(const int &socket) {
		*this = socket;
	}

	Socket &operator=(const int &socket) {
		this->socket_ = socket;
		return (*this);
	}

	bool operator==(const int &socket) {
		return this->socket_ == socket;
	}

	explicit operator int() const { 
		return this->socket_;
	}

	int sendMessage(const std::string &message) {
		return send(this->socket_, message.c_str(), message.size(), 0);
	}

	~Socket() {
		close(this->socket_);
	}
};