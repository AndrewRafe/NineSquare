#  Makefile
#  Nine-Board Tic-Tac-Toe
#  COMP3411 Artificial Intelligence
#  Andrew Rafe (edited from Alan Blair's supplied Makefile)
#  UNSW Session 1, 2014

CC = gcc
CFLAGS = -Wall -g -O3

servt9: servt9.o game.o common.h game.h agent.h
	$(CC) $(CFLAGS) -o servt9 servt9.o game.o

randt9: randt9.o client.o game.o common.h game.h agent.h
	$(CC) $(CFLAGS) -o randt9 randt9.o client.o game.o
	
agent: agent.o client.o game.o common.h game.h agent.h
	$(CC) $(CFLAGS) -o agent agent.o client.o game.o
	
all: servt9 randt9 agent

%o:%c common.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f servt9 randt9 agent *.o
