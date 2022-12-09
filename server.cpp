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
#include "PollManager.hpp"

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1602
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
	Socket socket;

	std::cout << "Server: Socket for server was successfully created.\n";

	struct sockaddr_in addrStruct;
	addrStruct.sin_family = AF_INET;
	addrStruct.sin_port = htons(DEFAULT_PORT);
	addrStruct.sin_addr.s_addr = htonl(INADDR_ANY);

	try {
		socket.setOptSocket();
		socket.bindSocket(addrStruct);
		socket.listenSocket(128);
		socket.setNonBlockSocket();
	} catch (Socket::SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(0);
	}
	int ret;
	PollManager pM(socket);
	while (1) {
		try {
			pM.start();
		}
		catch (const PollManager::PollException &e) {
			std::cerr << e.what() << std::endl;
			exit(0);
		}
		for (int i = 0; i < pM.countClients(); ++i) {
			if (pM[i].revents & POLLIN) {
				if (pM[i].fd == pM[0].fd) {
					Socket client(pM.acceptNewClient(reinterpret_cast<struct sockaddr*>(&addrStruct)));
					if (client.get() == -1) {
						std::cout << "Error: Can't accepting client.\n";
						continue;
					}
					if (pM.countClients() <= 10)
						pM.newClient(client);
					else {
						std::cout << "=> Too many clients\n";
						pM.newClient(client);
						pM.sendClient(client, "too many clients");
						pM.deleteClient(pM.countClients() - 1);
					}
				} else {
					std::string message = pM.recvClient(pM[i].fd);

                    if (message == std::string())
						pM.deleteClient(i);
                    else {
						message = "=> Client " + std::to_string(pM[i].fd) + ": " + message;
						std::cout << message << std::endl;
						for (int j = 0; j < pM.countClients(); ++j) {
							if (pM[j].fd != pM[i].fd)
								pM.sendClient(pM[j].fd, message);
						}
                    }
                }
			} else if (pM[i].revents & POLLERR) {
				if (pM[i].fd == pM[0].fd) {
 					std::cerr << "listen socket error\n";
					exit(1);
                } else
					pM.deleteClient(i);
			}
		}
	}

	return (0);
}
