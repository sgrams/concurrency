##
#  This file is part of 06-ipcq task.
#
#  Makefile
#  Stanisław Grams <sgrams@fmdx.pl>
##

CC=gcc
CFLAGS=-Wall -Werror
LDFLAGS=

all: client server rmobj
server:	server.o
client: client.o

clean:
	rm -f server server.o
	rm -f client client.o
rmobj:
	rm -f server.o client.o
