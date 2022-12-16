#include "ServerClass.hpp"
#include <fstream>

Server::Server(const struct sockaddr_in addr, const Socket &listen, const size_t &port, const std::string &ip) : addr_(addr), listenSock_(listen) {
	this->pollManager_.setListen(listenSock_);
}

Server::Server(const Socket &listen, const size_t &port, const std::string &ip) : listenSock_(listen) {
	(*this) = Server(this->addr_, this->listenSock_, port, ip);
}

Server::Server(const size_t &port, const std::string &ip) {
	this->_initSockAddr(port, ip);
	this->_initSocket(this->listenSock_);
	(*this) = Server(this->addr_, this->listenSock_, port, ip);
}

void Server::start() {
	while (1) {
		this->pollManager_.start();
		this->_serviceAllSockets();
	}
}

void Server::_serviceAllSockets() {
	for (int i = 0; i < this->pollManager_.countClients(); ++i) {
		if (this->pollManager_[i].revents & POLLIN) {
			this->_pollIn(i);
		} else if (this->pollManager_[i].revents & POLLOUT) {
			this->pollManager_.deleteClient(i);
		}
		else if (this->pollManager_[i].revents & POLLERR) {
			this->_pollErr(i);
		}
	}
}

void Server::_pollIn(const size_t &index) {
	if (this->pollManager_[index].fd == this->listenSock_.get())
		this->_addSocket();
	else
		this->_writer(index);
}

void Server::_addSocket() {
	Socket client(this->pollManager_.acceptNewClient(reinterpret_cast<struct sockaddr*>(&this->addr_)));
	if (client.get() == -1)
		std::cout << "Can't accepting client.\n";
	else
		this->pollManager_.newClient(client);
}

void Server::_writer(const size_t &index) {
	std::string message;
	if ((this->pollManager_.recvClient(this->pollManager_[index].fd, message)) <= 0)
		this->pollManager_.deleteClient(index);
    else {
		Messages mess(message);
		message = mess.response();
		this->pollManager_.sendClient(this->pollManager_[index].fd, message);
		this->pollManager_.deleteClient(index);
    }
}

void Server::_pollErr(const size_t &index) {
	if (this->pollManager_[index].fd == this->listenSock_.get())
		throw Exceptions("listen socket error");
	else
		this->pollManager_.deleteClient(index);
}

void Server::_initSocket(Socket &socket) {
	socket.setOptSocket();
	socket.bindSocket(this->addr_);
	socket.listenSocket(1);
	socket.setNonBlockSocket();
}

void Server::_initSockAddr(const size_t &port, const std::string &ip) {
	this->addr_.sin_family = AF_INET;
	this->addr_.sin_port = htons(port);
	this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}
