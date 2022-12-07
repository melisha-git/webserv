#ifndef SOCKET_CLASS_HPP
#define SOCKET_CLASS_HPP

#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>

class Socket {
private:
	int		socket_;
public:
	Socket(const int &domain = AF_INET, const int &type = SOCK_STREAM, const int &protocol = 0);
	Socket(const int& fd);
	Socket(const Socket &);

	int getSocket() const;

	Socket &operator=(const Socket &);
	void setOptSocket(const int &level = SOL_SOCKET, const int &optName = SO_REUSEADDR);
	void bindSocket(struct sockaddr_in &addr);
	void listenSocket(const int &);
	void setNonBlockSocket();
};

//#include "SocketClass.cpp"

#endif