CC= gcc
CFLAGS = -w  -pthread
LDFLAGS = -pthread -lssl -lcrypto
PACKAGE = `pkg-config --cflags --libs gtk+-2.0`
LIB = /usr/local/lib/
all : Tchat 

Tchat  : clserv.c graphique.c RSA.c Des.c Main.c
	$(CC) $(CFLAGS) -L$(LIB) clserv.c graphique.c RSA.c Des.c Main.c -o  Tchat $(PACKAGE) $(LDFLAGS)

clean:
	rm  Tchat
