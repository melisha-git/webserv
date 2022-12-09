#ifndef POLL_MANAGER_HPP
#define POLL_MANAGER_HPP

#include "SocketClass.hpp"
#include <poll.h>
#include <vector>

class PollManager {
private:
	std::vector<pollfd> pollSets_;
public:
	class PollException : public std::exception {
	private:
		std::string message_;
	public:
		virtual const char * what() const throw();
		PollException(const std::string &);
	};

	explicit PollManager(const Socket &listen);

	size_t countClients() const;
	pollfd &operator[](const size_t &index);

	void start();

	int acceptNewClient(struct sockaddr *addr);
	void newClient(const Socket &newSocket);
	void deleteClient(const size_t &index);
	void sendClient(const Socket &socket, const std::string &message);
	std::string recvClient(const Socket &socket);
};

#endif