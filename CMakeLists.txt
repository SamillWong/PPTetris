cmake_minimum_required(VERSION 3.13)

project(TetrisPP)

add_executable(TetrisPP main.cpp 
		Tetromino Tetromino.cpp
		Board Board.cpp
		Window Window.cpp)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/main DESTINATION bin)
