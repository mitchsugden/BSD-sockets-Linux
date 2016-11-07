#ifndef STRINGBUILDERS_H
#define STRINGBUILDERS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "Server.h"
#endif

char * getAvailableAccountsString(int, ClientDetails);
char * externalTransferMessage(float, int, int, float);
char * getInternalTransferMessage(float, int, int, float, float);

