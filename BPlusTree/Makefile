CC=g++
CF=-ggdb -std=c++11 -Wall -pthread

all: bplustree
		$(CC) $(CF) *.o main.cpp

bplustree:
		$(CC) $(CF) -c BPlusTree.cpp

clean:
		rm *.o a.out
