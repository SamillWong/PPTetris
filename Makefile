# Makefile

all: Window.cpp Board.cpp Tetromino.cpp main.cpp
	g++ Window.cpp Board.cpp Tetromino.cpp main.cpp -o tetrispp -g -fstack-protector-all
