FILES=main.cpp

FLAGS=-std=c++98

NAME=server

all: clean build run

clean:
	rm -rf $(NAME)

build:
	g++ $(FILES) -o $(NAME)

run:
	./$(NAME)