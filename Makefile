$(FLAGS):-Wall -Wextra -Werror

all: clean server

server:
	g++ backend/*.cpp -o server

clean:
	rm -rf server