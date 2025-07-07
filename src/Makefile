CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

SERVER = server
CLIENT = client

# Sources
SERVER_SRC = server.c dh.c
CLIENT_SRC = client.c dh.c

# Targets
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(SERVER) $(CLIENT) *.o

.PHONY: all clean
