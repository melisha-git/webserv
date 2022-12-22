#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <vector>
#include <algorithm>
#include <memory>
#include "AcceptorClass.hpp"

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1602
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void newClient(std::vector<pollfd> &poll_sets, const int &newSocket) {
	pollfd client_pollfd;
	client_pollfd.fd = newSocket;
	client_pollfd.events = POLLIN;
	poll_sets.push_back(client_pollfd);
	std::cout << "now_client_num: " << static_cast<unsigned int>(poll_sets.size() - 1) << std::endl;
}

void deleteClient(std::vector<pollfd> &poll_sets, std::vector<pollfd>::iterator &it) {
	poll_sets.erase(it, it);
	std::cout << "no connect " << it->fd << std::endl;
	close(it->fd);
}

int recvClient(const int &fd, std::string &result) {
	char message[1024] = {0};
	int readSize = recv(fd, message, 1024, 0);
	result = std::string(message);
	return readSize;
}

int main(int argc, char **argv) {
	Acceptor accept(DEFAULT_PORT);
	
	std::vector<pollfd>	poll_sets;
	newClient(poll_sets, static_cast<int>(accept.getListen()));
	Socket newSocket;
	while (1) {
		int ret = poll(dynamic_cast<pollfd *>(&poll_sets[0]), (unsigned int)poll_sets.size(), -1);
		if (ret <= 0) {
			std::cerr << "poll error\n";
			exit(1);
		}
		std::vector<pollfd>::iterator it;
		std::vector<pollfd>::iterator end = poll_sets.end();
		for (it = poll_sets.begin(); it != end; it++) {
			if (it->revents & POLLIN) {
				if (it->fd == static_cast<int>(accept.getListen())) {
					accept.Accept(newSocket, [&newSocket, &poll_sets](int error) {
						if (error)
							return;
						else
							newClient(poll_sets, static_cast<int>(newSocket));
					});
				} else {
					newSocket = it->fd;
					std::string message;

					if (recvClient(it->fd, message) <= 0)
						deleteClient(poll_sets, it);
					else {
						message = "=> Client " + std::to_string(it->fd) + ": " + message;
						std::cout << message;
						message = "Server: \n";
						newSocket.sendMessage(message);
					}
				}
			} else if (it->revents & POLLERR) {
				if (it->fd == static_cast<int>(accept.getListen())) {
					std::cerr << "listen socket error\n";
					exit(1);
				} else
					deleteClient(poll_sets, it);
			}
		}
	}

	return (0);
}