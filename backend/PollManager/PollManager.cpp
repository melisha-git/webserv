#include "PollManager.hpp"

PollManager::PollManager(const Socket &listen) {
	this->newClient(listen);
}

PollManager::PollManager() {}

size_t PollManager::countClients() const {
	return this->pollSets_.size();
}

void PollManager::clear() {
	while (!(this->pollSets_.empty())) {
		close(pollSets_[pollSets_.size() - 1].fd);
		std::cout << "no connect " << pollSets_[pollSets_.size() - 1].fd << std::endl;
		this->pollSets_.pop_back();
	}
}

void PollManager::setListen(const Socket &listen) {
	if (!this->pollSets_.empty())
		this->clear();
	this->newClient(listen);
}

pollfd &PollManager::operator[](const size_t &index) {
	return ((this->pollSets_)[index]);
}

void PollManager::start() {
	if (poll(dynamic_cast<pollfd *>(&this->pollSets_[0]), this->countClients(), -1) <= 0)
		throw Exceptions("Poll function error");
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

int PollManager::recvClient(const Socket &socket, std::string &message) {
	char mess[1024] = {0};
	int readSize = recv(socket.get(), mess, 1024, 0);
	message = std::string(mess);
	return readSize;
}