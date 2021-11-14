CC= gcc
CFLAGS = -w  -pthread
LDFLAGS = -pthread -lssl -lcrypto
PACKAGE = `pkg-config --cflags --libs gtk+-2.0`
LIB = /usr/local/lib/
all : Tchat 

Tchat  : clserv.c graphique.c RSA.c
	$(CC) $(CFLAGS) -L$(LIB) clserv.c graphique.c RSA.c -o  Tchat $(PACKAGE) $(LDFLAGS)

clean:
	rm  Tchat


# gcc -g -o rsa *.c -L/usr/local/lib/ #-lssl -lcrypto
