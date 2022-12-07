$(FLAGS) : -Wall -Wextra -Werror

all: clean server

server:
	g++ server.cpp -o server

clean:
	rm -rf server