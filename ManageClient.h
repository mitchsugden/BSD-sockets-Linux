#ifndef MANAGECLIENT_H
#define MANAGECLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "Server.h"
#include "MenuFunctions.h"

#define e 69
#define E 101
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#endif

char * readInput(int);
int authenticateClient(int);
int * getAvailableAccounts(int, ClientDetails);
int validateInput(int, char*, int);
float checkFloat(int, char*);
char invalidAmountMessage[BUFFER_SIZE];
