##
#  This file is part of 08-sockets task.
#
#  Makefile
#  Stanisław Grams <sgrams@fmdx.pl>
##

CC=gcc
CFLAGS=-Wall -Werror -pedantic -std=c99
LDFLAGS=
OUTNAME=client

all: $(OUTNAME) rmobj
$(OUTNAME): client.o
client.o: client.c


rmobj:
	rm -f client.o
clean:
	rm -f $(OUTNAME) client.o
