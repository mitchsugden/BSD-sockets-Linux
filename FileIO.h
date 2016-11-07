#ifndef FILEIO_H
#define FILEIO_H

#include "Server.h"
#include "ManageTransactions.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#endif

void readFile();
void readClientDetails();
void readAccountFile();
void readTransactionsFile();
void writeAccountsFile();
void writeTransactionsFile();
float getClosingBalance(int);
float getOpeningBalance(int);
float appendClosingBalance(int, float, int);
int accountExists(int);
