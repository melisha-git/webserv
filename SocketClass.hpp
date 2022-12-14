#pragma once

#include "ExceptionClass.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <functional>
#include <poll.h>
#include <iostream>

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

	Socket(const Socket &socket) {
		*this = socket;
	}

	Socket &operator=(const Socket &socket) {
		this->socket_ = socket.socket_;
		return (*this);
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

	int recvMessage(std::string &result) {
		char message[1024] = {0};
		int readSize = recv(this->socket_.fd, message, 1024, 0);
		result = std::string(message);
		return readSize;
	}

	void recvMessage(std::string &result, void(*f)(int, std::string, Socket &)) {
		int readSize = this->recvMessage(result);
		f(readSize <= 0, result, *(this));
	}

	~Socket() {
		close(this->socket_.fd);
	}
};