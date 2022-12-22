#pragma once

#include "SocketClass.hpp"
#include <netinet/in.h>
#include <functional>
#include <fcntl.h>
#include <poll.h>

void newClient(std::vector<pollfd> &poll_sets, const Socket &newSocket) {
	poll_sets.push_back(newSocket);
	std::cout << "now_client_num: " << static_cast<unsigned int>(poll_sets.size() - 1) << std::endl;
}

void deleteClient(std::vector<pollfd> &poll_sets, int index) {
	std::vector<pollfd>::iterator it = poll_sets.begin() + index;
	poll_sets.erase(it, it);
	std::cout << "no connect " << it->fd << std::endl;
	close(it->fd);
}

int recvClient(const int &fd, std::string &result) {
	char message[1024] = {0};
	int readSize = recv(fd, message, 1024, 0);
	result = std::string(message);
	return readSize;
}

class Acceptor {
private:
	struct sockaddr_in	addr_;
	socklen_t			addrSize_;
	Socket				listen_;
	std::vector<pollfd>	clients_;
public:


	Acceptor(int port) {
		this->addr_.sin_family = AF_INET;
		this->addr_.sin_port = htons(port);
		this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addrSize_ = sizeof(this->addr_);

		this->_setSockOpt(static_cast<pollfd>(listen_).fd);
		this->_binding(static_cast<pollfd>(listen_).fd);
		this->_listening(static_cast<pollfd>(listen_).fd);
		clients_.push_back(listen_);
	}

	Socket &getListen() {
		return this->listen_;
	}

	void Accept(Socket &socket, std::function<void(int)> t) {
		while (this->_poll()) {
			for (int i = 0; i < clients_.size(); ++i) {
				if (clients_[i].revents & POLLIN) {
					if (clients_[i].fd == static_cast<pollfd>(this->listen_).fd) {
						socket = accept(static_cast<pollfd>(listen_).fd, reinterpret_cast<struct sockaddr*>(&this->addr_), &this->addrSize_);
						if (static_cast<pollfd>(socket).fd > 0)
							newClient(clients_, socket);
					} else {
						socket = clients_[i].fd;
						std::string message;

						if (recvClient(clients_[i].fd, message) <= 0)
							deleteClient(clients_, i);
						else {
							message = "=> Client " + std::to_string(clients_[i].fd) + ": " + message;
							std::cout << message;
							message = "Server: \n";
							socket.sendMessage(message);
						}
					}
				} else if (clients_[i].revents & POLLERR) {
					if (clients_[i].fd == static_cast<pollfd>(this->listen_).fd) {
						std::cerr << "listen socket error\n";
						exit(1);
					} else
						deleteClient(clients_, i);
				}
			}
		}
		//t(socket == -1 ? 1 : 0);
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

	bool _poll() {
		int result = poll(dynamic_cast<pollfd *>(&clients_[0]), (unsigned int)clients_.size(), -1);
		return result > 0;
	}
};