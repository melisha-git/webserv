$(FLAGS) : -Wall -Wextra -Werror

all: clean server

server:
	g++ server.cpp SocketClass.cpp -o server

clean:
	rm -rf server