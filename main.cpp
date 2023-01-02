#include "AcceptorClass.hpp"
#include "ResponseClass.hpp"

void recvF(int error, std::string message, Socket &src) {
	if (error)
		return;
	Response res(message);
	src.sendMessage(res);
}

void acceptF(int error, Socket &src) {
	if (error)
		return;
	std::string message;
	src.recvMessage(message, recvF);
}

int main(int argc, char **argv) {
	Acceptor acceptor(1602);

	Socket src;
	acceptor.Accept(src, acceptF);
	return (0);
}
