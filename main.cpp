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

//void newClient(std::vector<pollfd> &poll_sets, const Socket &newSocket) {
//	poll_sets.push_back(newSocket);
//	std::cout << "now_client_num: " << static_cast<unsigned int>(poll_sets.size() - 1) << std::endl;
//}

//void deleteClient(std::vector<pollfd> &poll_sets, int index) {
//	std::vector<pollfd>::iterator it = poll_sets.begin() + index;
//	poll_sets.erase(it, it);
//	std::cout << "no connect " << it->fd << std::endl;
//	close(it->fd);
//}

//int recvClient(const int &fd, std::string &result) {
//	char message[1024] = {0};
//	int readSize = recv(fd, message, 1024, 0);
//	result = std::string(message);
//	return readSize;
//}

int main(int argc, char **argv) {
	Acceptor acceptor(DEFAULT_PORT);
	
	Socket src;
	acceptor.Accept(src, [&src, &acceptor](int error) {
		if (error)
			return;
		std::string message;

		src.recvMessage(message, [&message, &acceptor, &src](int error) {
			if (error) {
				acceptor.deleteClient(src);
				return;
			}
			else {
				message = "=> Client " + std::to_string(src.operator pollfd().fd) + ": " + message;
				std::cout << message;
				message = "Server: \n";
				src.sendMessage(message);
			}
		});
	});
	return (0);
}