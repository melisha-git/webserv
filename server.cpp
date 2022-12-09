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
#include "SocketClass.hpp"

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1602
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int acceptNewClient(const int &listenSocket, struct sockaddr *addrStruct) {
	socklen_t size = sizeof(addrStruct);
	int ret = accept(listenSocket, addrStruct, &size);
	if (ret == -1)
		std::cout << ERROR_S << "Can't accepting client.\n";
	return ret;
}

void newClient(std::vector<pollfd> &poll_sets, const int &newSocket) {
	pollfd client_pollfd;
    client_pollfd.fd = newSocket;
    client_pollfd.events = POLLIN;
    poll_sets.push_back(client_pollfd);
    std::cout << "now_client_num: " << static_cast<unsigned int>(poll_sets.size() - 1) << std::endl;
}

void deleteClient(std::vector<pollfd> &poll_sets, std::vector<pollfd>::iterator &it) {
	poll_sets.erase(it);
	std::cout << "no connect" << it->fd << std::endl;
	close(it->fd);
}

void sendClient(const int &fd, const std::string &message) {
	send(fd, message.c_str(), message.size(), 0);
}

std::string recvClient(const int &fd) {
	char message[1024] = {0};
	int readSize = recv(fd, message, 1024, 0);
	std::string result = readSize <= 0 ? std::string() : std::string(message);
	return result;
}

int main(int argc, char **argv) {
	Socket socket;

	std::cout << "Server: Socket for server was successfully created.\n";

	struct sockaddr_in addrStruct;
	addrStruct.sin_family = AF_INET;
	addrStruct.sin_port = htons(DEFAULT_PORT);
	addrStruct.sin_addr.s_addr = htonl(INADDR_ANY);

	try {
		socket.setOptSocket();
		socklen_t size = sizeof(addrStruct);
		socket.bindSocket(addrStruct);
		socket.listenSocket(128);
		socket.setNonBlockSocket();
	} catch (Socket::SocketException &e) {
		std::cerr << e.what() << std::endl;
	}
	int ret;
	int newSocket;
	int listenSocket = socket.get();
	std::vector<pollfd> poll_sets;
	newClient(poll_sets, listenSocket);
	while (1) {
		ret = poll(dynamic_cast<pollfd *>(&poll_sets[0]), (unsigned int)poll_sets.size(), -1);
		if (ret <= 0)
        {
            std::cerr << "poll error\n";
            exit(1);
        }
		std::vector<pollfd>::iterator it;
        std::vector<pollfd>::iterator end = poll_sets.end();
		for (it = poll_sets.begin(); it != end; it++) {
			if (it->revents & POLLIN) {
				if (it->fd == listenSocket) {
					if ((newSocket = acceptNewClient(listenSocket, reinterpret_cast<struct sockaddr*>(&addrStruct))) == -1)
						continue;
					if (poll_sets.size() - 1 < 10)
						newClient(poll_sets, newSocket);
					else {
						std::cout << "=> Too many clients\n";
						sendClient(newSocket, "too many clients");
    					close(newSocket);
                    }
				} else {
					std::string message = recvClient(it->fd);

                    if (message == std::string())
						deleteClient(poll_sets, it);
                    else {
						message = "=> Client " + std::to_string(it->fd) + ": " + message;
						std::cout << message << std::endl;
						for (auto p : poll_sets) {
							if (p.fd != it->fd)
								sendClient(p.fd, message);
						}
                    }
                }
			} else if (it->revents & POLLERR) {
				if (it->fd == listenSocket) {
 					std::cerr << "listen socket error\n";
					exit(1);
                } else
					deleteClient(poll_sets, it);
			}
		}
	}

	return (0);
}