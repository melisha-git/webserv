#include "SocketClass.hpp"
#include "PollManager.hpp"
#include "ServerClass.hpp"

int main(int argc, char **argv) {
	try {
		Socket socket;
		std::cout << "Server: Socket for server was successfully created.\n";
		Server s;
		s.start();
	} catch(const Exceptions &e) {
		std::cerr << e.what() << std::endl;
		throw;
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
	std::cout << "Id reprehenderit nulla consequat est esse ex aliquip enim aute.";
	return (0);
}
