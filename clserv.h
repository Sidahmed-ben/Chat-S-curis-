#ifndef CLSERV_H
#define CLSERV_H

#include <sys/socket.h>
#include "RSA.h"

extern int sock, n;
extern socklen_t fromlen;
extern struct sockaddr_in server_sr;
extern struct sockaddr_in server_cl;
extern struct sockaddr_in from_cl;
extern struct sockaddr_in from_sr;
extern socklen_t length;

extern struct hostent *hp;

extern void *premiere_connection_cl(void *arg);
extern void *premiere_connection_sr(void *arg);
extern void *reception_cl(void *arg);
extern void *reception_sr(void *arg);

extern void envoi_sr(const char *chaine);
extern void envoi_cl(const char *chaine);
extern void error(char *msg);

#endif