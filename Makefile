FLAGS=-Wall -Wextra -Werror

FILES=./backend/server.cpp \
		./backend/ExceptionClass/ExceptionClass.cpp \
		./backend/PollManager/PollManager.cpp \
		./backend/ServerClass/ServerClass.cpp \
		./backend/SocketClass/SocketClass.cpp \
		./backend/MessageClass/MessagesClass.cpp

all: clean server

server:
	g++ $(FILES) -o server

clean:
	rm -rf server