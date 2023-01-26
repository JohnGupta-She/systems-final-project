#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>

#ifndef HEADERS_H

#define HEADERS_H

#define NUMROOMS 1
#define MAXCLIENTS 32

struct message_data{
char message[1024];
char name[32];
int id;
};

#endif
