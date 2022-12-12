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
		if (this->pollManager_[i].revents & POLLIN)
			this->_pollIn(i);
		else if (this->pollManager_[i].revents & POLLERR)
			this->_pollErr(i);
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
	else if (this->pollManager_.countClients() <= 10)
		this->pollManager_.newClient(client);
	else {
		std::cout << "=> Too many clients\n";
		this->pollManager_.newClient(client);
		this->pollManager_.sendClient(client, "too many clients");
		this->pollManager_.deleteClient(this->pollManager_.countClients() - 1);
	}
}

void Server::_writer(const size_t &index) {
	std::string message;
	if ((this->pollManager_.recvClient(this->pollManager_[index].fd, message)) <=0)
		this->pollManager_.deleteClient(index);
    else {
		//Messages request(message);
		std::string html = "<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>\n<h1>Welcome to webserv</h1>\n<img src=\"../../frontend/images/752775.jpg\" width=\"100%\"></img>\n</body>\n</html>";
		message = "HTTP/1.1 202 OK\nServer: localhost\nContent-type: text/html\nContent-Lenght: " + std::to_string(html.size()) + "\nConnection: close\n\n" + html;
		//std::cout << message << std::endl;
		this->pollManager_.sendClient(this->pollManager_[index].fd, message);
		// for (int j = 0; j < this->pollManager_.countClients(); ++j) {
		// 	if (this->pollManager_[j].fd != this->pollManager_[index].fd)
		// 		this->pollManager_.sendClient(this->pollManager_[j].fd, message);
		// }
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
	socket.listenSocket(5);
	socket.setNonBlockSocket();
}

void Server::_initSockAddr(const size_t &port, const std::string &ip) {
	this->addr_.sin_family = AF_INET;
	this->addr_.sin_port = htons(port);
	this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}