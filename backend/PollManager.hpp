#ifndef POLL_MANAGER_HPP
#define POLL_MANAGER_HPP

#include "SocketClass.hpp"
#include "ExceptionClass.hpp"
#include <poll.h>
#include <vector>

class PollManager {
private:
	std::vector<pollfd> pollSets_;
public:
	explicit PollManager(const Socket &listen);
	PollManager();

	void clear();
	void setListen(const Socket &listen);
	size_t countClients() const;
	pollfd &operator[](const size_t &index);

	void start();

	int acceptNewClient(struct sockaddr *addr);
	void newClient(const Socket &newSocket);
	void deleteClient(const size_t &index);
	void sendClient(const Socket &socket, const std::string &message);
	int recvClient(const Socket &socket, std::string &message);
};

#endif