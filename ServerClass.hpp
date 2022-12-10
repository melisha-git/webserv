#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <netinet/in.h>
#include "ExceptionClass.hpp"
#include "SocketClass.hpp"
#include "PollManager.hpp"

class Server {
private:
	Socket listenSock_;
	struct sockaddr_in addr_;
	PollManager	pollManager_;
public:
	Server(const struct sockaddr_in addr, const Socket &listen, const size_t &port= 1602, const std::string &ip = "127.0.0.1");
	Server(const Socket &listen, const size_t &port = 1602, const std::string &ip = "127.0.0.1");
	Server(const size_t &port = 1602, const std::string &ip = "127.0.0.1");
	void start();

private:
	void _serviceAllSockets();
	void _pollIn(const size_t &index);
	void _addSocket();
	void _writer(const size_t &index);
	void _pollErr(const size_t &index);
	void _initSocket(Socket &socket);
	void _initSockAddr(const size_t &port, const std::string &ip);
};

#endif