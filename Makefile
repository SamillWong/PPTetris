# Makefile

all: Window.cpp Board.cpp Tetromino.cpp main.cpp
	g++ Window.cpp Board.cpp Tetromino.cpp main.cpp -o main -g -fstack-protector-all
