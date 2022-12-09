#include "PollManager.hpp"

PollManager::PollManager(const Socket &listen) {
	this->newClient(listen);
}

size_t PollManager::countClients() const {
	return this->pollSets_.size();
}

pollfd &PollManager::operator[](const size_t &index) {
	return ((this->pollSets_)[index]);
}

void PollManager::start() {
	if (poll(dynamic_cast<pollfd *>(&this->pollSets_[0]), this->countClients(), -1) <= 0)
		throw PollManager::PollException("Poll function error");
}

int PollManager::acceptNewClient(struct sockaddr *addr) {
	socklen_t size = sizeof(addr);
	int ret = accept(this->pollSets_[0].fd, addr, &size);
	return ret;
}

void PollManager::newClient(const Socket &newSocket) {
	pollfd client_pollfd;
	client_pollfd.fd = newSocket.get();
	client_pollfd.events = POLLIN;
	this->pollSets_.push_back(client_pollfd);
	std::cout << "now_client_num: " << this->countClients() - 1 << std::endl;
}

void PollManager::deleteClient(const size_t &index) {
	std::vector<pollfd>::iterator it = this->pollSets_.begin() + index;
	this->pollSets_.erase(it);
	std::cout << "no connect " << it->fd << std::endl;
	close(it->fd);
}

void PollManager::sendClient(const Socket &socket, const std::string &message) {
	send(socket.get(), message.c_str(), message.size(), 0);
}

std::string PollManager::recvClient(const Socket &socket) {
	char message[1024] = {0};
	int readSize = recv(socket.get(), message, 1024, 0);
	std::string result = readSize <= 0 ? std::string() : std::string(message);
	return result;
}

PollManager::PollException::PollException(const std::string &message) : message_(message) {}

const char * PollManager::PollException::what() const throw() {
	return (this->message_.c_str());
}