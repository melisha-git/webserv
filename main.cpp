#include "AcceptorClass.hpp"
#include "ResponseClass.hpp"

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
			Response res;
			res.setBody("./index.html");
			res.setHeader("Server", "bebrochka");
			res.setHeader("Content-Length", std::to_string(res.getBodySize()));
			res.setHeader("Content-Type", "text/html");
			res.setHeader("Connection", "close");
			res.setStartLine(Status(202));
			message = res;
			src.sendMessage(message);
		});
	});
	return (0);
}