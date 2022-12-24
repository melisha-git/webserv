#pragma once

#include "SocketClass.hpp"
#include <netinet/in.h>
#include <algorithm>
#include <functional>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <list>

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

	void Accept(Socket &socket, std::function<void(int)> f) {
		while (this->_poll()) {
			for (int i = 0; i < clients_.size(); ++i) {
				int code = 0;
				if (clients_[i].revents & POLLIN && i == 0) {
					socket = accept(static_cast<pollfd>(listen_).fd, reinterpret_cast<struct sockaddr*>(&this->addr_), &this->addrSize_);
					if (static_cast<pollfd>(socket).fd > 0) {
						clients_.push_back(socket);
						std::cout << "now_client_num: " << static_cast<unsigned int>(clients_.size() - 1) << std::endl;
					}
				} else if (clients_[i].revents & POLLERR) {
					if (clients_[i].fd == static_cast<pollfd>(this->listen_).fd)
						throw Exception("Listen socket error\n");
					code = 1;
				}
				if (i != 0 && (clients_[i].revents & POLLIN || clients_[i].revents & POLLERR)) {
					socket = clients_[i].fd;
					try {
						f(code);
					} catch (const Exception &ex) {
						this->_delete(i);
					}
				}
			}
		}
	}

private:
	void _delete(int index) {
		std::list<pollfd> copy{clients_.begin(), clients_.end()};
		for (std::list<pollfd>::iterator it = copy.begin(); it != copy.end(); ++it) {
			if (it->fd == clients_[index].fd) {
				copy.erase(it);
				std::cout << "no connect " << (clients_.begin() + index)->fd << std::endl;
				close((clients_.begin() + index)->fd);
			}
		}
		clients_.clear();
		clients_ = {copy.begin(), copy.end()};
	}

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