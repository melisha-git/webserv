#ifndef SOCKET_CLASS_HPP
#define SOCKET_CLASS_HPP

#include "../ExceptionClass/ExceptionClass.hpp"
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

class Socket {
private:
	int		socket_;
public:
	Socket();
	Socket(const int& fd);
	Socket(const Socket &);

	int get() const;

	Socket &operator=(const Socket &);
	void setOptSocket(const int &level = SOL_SOCKET, const int &optName = SO_REUSEADDR);
	void bindSocket(struct sockaddr_in &addr);
	void listenSocket(const int &);
	void setNonBlockSocket();
};

#endif