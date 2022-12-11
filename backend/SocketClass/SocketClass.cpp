#include "SocketClass.hpp"

Socket::Socket() {
	// Активация соккета
	this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket_ < 0)
		throw Exceptions("Error: establishing socket error.");
}

Socket::Socket(const int &fd) {
	this->socket_ = fd;
}

Socket::Socket(const Socket &other) {
	*this = other;
}

int Socket::get() const {
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
		throw Exceptions("Error: set_reuse_addr error");
}

void Socket::bindSocket(struct sockaddr_in &addr) {
	socklen_t size = sizeof(addr);
	//Снабдить соккет адресом
	int ret = bind(this->socket_, reinterpret_cast<struct sockaddr*>(&addr), size);
	if (ret < 0)
		throw Exceptions("Error: binding connection. Socket already been establishing.");
}

void Socket::listenSocket(const int &maxQuequClients) {
	if (listen(this->socket_, maxQuequClients) == -1)
		throw Exceptions("Error: listen socket");
}

void Socket::setNonBlockSocket() {
	fcntl(this->socket_, F_SETFL, fcntl(this->socket_, F_GETFL) | O_NONBLOCK);
}