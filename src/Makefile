# Makefile for CPE464 program 1

CC= gcc
CFLAGS= 

# The  -lsocket -lnsl are needed for the sockets.
# The -L/usr/ucblib -lucb gives location for the Berkeley library needed for
# the bcopy, bzero, and bcmp.  The -R/usr/ucblib tells where to load
# the runtime library.

# The next line is needed on Sun boxes (so uncomment it if your on a
# sun box)
# LIBS =  -lsocket -lnsl

# For Linux/Mac boxes uncomment the next line - the socket and nsl
# libraries are already in the link path.
LIBS =

all:   starChat starTest

starChat: starChat.c star_packet.c
	$(CC) $(CFLAGS) -o starChat starChat.c star_packet.c $(LIBS)

starTest: starTest.c star_packet.c
	$(CC) $(CFLAGS) -o starTest starTest.c star_packet.c $(LIBS)

clean:
	rm starChat starTest



