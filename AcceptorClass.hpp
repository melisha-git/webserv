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
	std::vector<pollfd>	clients_;
public:


	void deleteClient(const Socket &socket) {
		std::vector<pollfd>::iterator it = clients_.begin();
		for (;it != clients_.end(); ++it) {
			if (it->fd == static_cast<pollfd>(socket).fd) {
				clients_.erase(it, it);
				std::cout << "no connect " << it->fd << std::endl;
				close(it->fd);
			}
		}
	}

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

	void Accept(Socket &socket, std::function<void(int)> f) {
		while (this->_poll()) {
			for (int i = 0; i < clients_.size(); ++i) {
				if (clients_[i].revents & POLLIN) {
					if (i == 0) {
						socket = accept(static_cast<pollfd>(listen_).fd, reinterpret_cast<struct sockaddr*>(&this->addr_), &this->addrSize_);
						if (static_cast<pollfd>(socket).fd > 0) {
							clients_.push_back(socket);
							std::cout << "now_client_num: " << static_cast<unsigned int>(clients_.size() - 1) << std::endl;
						}
					} else {
						socket = clients_[i].fd;
						f(0);
					}
				} else if (clients_[i].revents & POLLERR) {
					socket = clients_[i].fd;
					if (clients_[i].fd == static_cast<pollfd>(this->listen_).fd)
						throw Exception("Listen socket error\n");
					else
						this->deleteClient(socket);
					f(1);
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
		int result = poll(dynamic_cast<pollfd *>(&clients_[0]), (unsigned int)clients_.size(), 10000);
		return result > 0;
	}
};