#pragma once

#include "ExceptionClass.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>

class Socket {
protected:
	pollfd	socket_;
public:

	Socket() {
		this->socket_.fd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->socket_.fd == -1)
			throw Exception("Establishing socket error\n");
		this->socket_.events = POLLIN;
		fcntl(static_cast<int>(socket_.fd), F_SETFL, fcntl(static_cast<int>(socket_.fd), F_GETFL) | O_NONBLOCK);
	}

	Socket(const int &socket) {
		*this = socket;
		this->socket_.events = POLLIN;
		fcntl(static_cast<int>(socket_.fd), F_SETFL, fcntl(static_cast<int>(socket_.fd), F_GETFL) | O_NONBLOCK);
	}

	Socket &operator=(const int &socket) {
		this->socket_.fd = socket;
		this->socket_.events = POLLIN;
		return (*this);
	}

	bool operator==(const int &socket) {
		return this->socket_.fd == socket;
	}

	operator pollfd() const { 
		return this->socket_;
	}

	int sendMessage(const std::string &message) {
		return send(this->socket_.fd, message.c_str(), message.size(), 0);
	}

	~Socket() {
		close(this->socket_.fd);
	}
};