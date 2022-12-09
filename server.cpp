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
#include "ServerClass.hpp"

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1602
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
	Socket socket;

	std::cout << "Server: Socket for server was successfully created.\n";
	Server s;
	s.start();
	return (0);
}
