#ifndef MENUFUNCTIONS_H
#define MENUFUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "stdbool.h"
#include "Server.h"
#include "ManageClient.h"

#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#endif

void getAccountBalance(int, ClientDetails);
void withdrawal(int, ClientDetails);
void deposit(int, ClientDetails);
void transfer(int, ClientDetails);
void listTransactions(int, ClientDetails);
char *invalidInputMessage;
transaction_t newTransaction();
