CFLAGS += -Wno-unused-result

all: server client

server: server.o
server.o: server.c shared.h

client: LDLIBS += -lncurses
client: client.o
client.o: client.c shared.h

.PHONY: all
