#include "AcceptorClass.hpp"

int main(int argc, char **argv) {
	Acceptor acceptor(1602);

	Socket src;
	acceptor.Accept(src, [&src, &acceptor](int error) {
		if (error)
			throw Exception("No accept socket\n");
		std::string message;

		src.recvMessage(message, [&message, &acceptor, &src](int error) {
			if (error)
				throw Exception("No read bytes\n");
			message = "=> Client " + src.operator pollfd().fd + std::string(" ") + message;
			std::cout << message;
			message = "Server: \n";
			src.sendMessage(message);
		});
	});
	return (0);
}