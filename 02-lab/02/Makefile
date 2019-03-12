## 
#  Makefile
#  Stanisław Grams <sgrams@fmdx.pl>
##
CC=gcc
CFLAGS=-Wall -Werror -pedantic -std=gnu99 -g
LDFLAGS=

# main targets
all: server client

# *.c
server: server.c common.c
client: client.c common.c

# other procedures
.PHONY: clean
clean:  rmobj
rmobj:
	rm -f server client