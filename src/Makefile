CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lcrypto

OPENSSL_PATH = $(shell brew --prefix openssl)

CFLAGS += -I$(OPENSSL_PATH)/include
LDFLAGS += -L$(OPENSSL_PATH)/lib -lcrypto

SERVER = server
CLIENT = client

# Sources
SERVER_SRC = server.c dh.c crypto_utils.c
CLIENT_SRC = client.c dh.c crypto_utils.c

# Targets
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(SERVER) $(CLIENT) *.o

.PHONY: all clean
