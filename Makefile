all:
	g++ -std=c++14 -Wall -Wextra -pedantic -g -O0 -o main file_handler.cpp file_utils.cpp main.cpp
