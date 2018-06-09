CC=g++
CFLAGS= -std=c++11 -Wextra -Wall -pedantic -g -lm -Wno-unused-variable -Wno-unused-parameter



main: main.cpp ppmloader.cpp
	$(CC) $(CFLAGS) $^ -o tp3


clean:
	rm tp3
	rm *.txt

