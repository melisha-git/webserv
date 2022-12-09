#include "ServerClass.hpp"

Server::Server(const Socket &listen, const size_t &port, const std::string &ip) : listenSock_(listen) {
	this->_initSockAddr(port, ip);
	this->pollManager_.setListen(listen);
}

Server::Server(const size_t &port, const std::string &ip) {
	this->_initSockAddr(port, ip);
	this->_initSocket(this->listenSock_);
	(*this) = Server(this->listenSock_, port, ip);
}

void Server::start() {
	while (1) {
		try {
			this->pollManager_.start();
		}
		catch (const PollManager::PollException &e) {
			std::cerr << e.what() << std::endl;
			exit(0);
		}
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
		std::cout << "Error: Can't accepting client.\n";
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
	std::string message = this->pollManager_.recvClient(this->pollManager_[index].fd);
	if (message == std::string())
		this->pollManager_.deleteClient(index);
    else {
		message = "=> Client " + std::to_string(this->pollManager_[index].fd) + ": " + message;
		std::cout << message << std::endl;
		for (int j = 0; j < this->pollManager_.countClients(); ++j) {
			if (this->pollManager_[j].fd != this->pollManager_[index].fd)
				this->pollManager_.sendClient(this->pollManager_[j].fd, message);
		}
    }
}

void Server::_pollErr(const size_t &index) {
	if (this->pollManager_[index].fd == this->listenSock_.get()) {
		std::cerr << "listen socket error\n";
		exit(1);
    } else
		this->pollManager_.deleteClient(index);
}

void Server::_initSocket(Socket &socket) {
	try {
		socket.setOptSocket();
		socket.bindSocket(this->addr_);
		socket.listenSocket(128);
		socket.setNonBlockSocket();
	} catch (Socket::SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(0);
	}
}

void Server::_initSockAddr(const size_t &port, const std::string &ip) {
	this->addr_.sin_family = AF_INET;
	this->addr_.sin_port = htons(port);
	this->addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}