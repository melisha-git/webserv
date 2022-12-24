#include "AcceptorClass.hpp"
#include "ResponseClass.hpp"

int main(int argc, char **argv) {
	Acceptor acceptor(1602);

	Socket src;
	acceptor.Accept(src, [&src, &acceptor](int error) {
		if (error)
			return;
		std::string message;

		src.recvMessage(message, [&message, &acceptor, &src](int error) {
			if (error)
				return;
			Response res(message);
			message = res;
			src.sendMessage(message);
		});
	});
	return (0);
}