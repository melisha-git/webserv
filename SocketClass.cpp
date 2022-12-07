#include "SocketClass.hpp"

Socket::Socket(const int &domain, const int &type, const int &protocol) {
	// Активация соккета
	this->socket_ = socket(domain, type, protocol);
	if (this->socket_ < 0) {
		std::cout << "Error: " << "establishing socket error.\n";
		exit (0);
	}
}

Socket::Socket(const int &fd) {
	this->socket_ = fd;
}

Socket::Socket(const Socket &other) {
	*this = other;
}

int Socket::getSocket() const {
	return this->socket_;
}

Socket &Socket::operator=(const Socket &other) {
	this->socket_ = other.socket_;
	return (*this);
}

// Устраняем проблему залипшего соккета
void Socket::setOptSocket(const int &level, const int &optName) {
	int number = 1;
	if (setsockopt(this->socket_, level, optName, &number, sizeof(number)) == -1)
	{
		std::cerr << "set_reuse_addr error\n";
		exit(0);
	}
}

void Socket::bindSocket(struct sockaddr_in &addr) {
	socklen_t size = sizeof(addr);
	//socket.bindSocket(addrStruct);
	//Снабдить соккет адресом
	int ret = bind(this->socket_, reinterpret_cast<struct sockaddr*>(&addr), size);
	if (ret < 0) {
		std::cerr << "Error: " << "binding connection. Socket already been establishing.\n";
		exit(0);
	}
}

void Socket::listenSocket(const int &maxQuequClients) {
	if (listen(this->socket_, maxQuequClients) == -1) {
		std::cerr << "error listen socket\n";
		exit(0);
	}
}

void Socket::setNonBlockSocket() {
	fcntl(this->socket_, F_SETFL, fcntl(this->socket_, F_GETFL) | O_NONBLOCK);
}