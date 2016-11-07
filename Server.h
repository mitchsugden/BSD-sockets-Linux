#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define LINE_LENGTH 50
#define NUM_CLIENTS 10
#define NUM_ACCOUNTS 24
#define BUFFER_SIZE 2000

typedef struct ClientDetails {
    char firstName[LINE_LENGTH];
    char lastName[LINE_LENGTH];
    int clientNo;
    int savingsAccount;
    int creditAccount;
    int loanAccount;
} ClientDetails;

typedef struct Account {
    int accountNumber;
    float openingBalance;
    float closingBalance;
} Account;

typedef struct transaction transaction_t;
struct transaction {
    int fromAccountNum;
    int toAccountNum;
    int tranType;
    float amount;
};

typedef struct node node_t;
struct node {
    transaction_t *transaction;
    node_t *next;
};

ClientDetails clients[NUM_CLIENTS];
Account accounts[NUM_ACCOUNTS];

#include "ManageClient.h"
#include "FileIO.h"
#include "StringBuilders.h"
#include "ManageTransactions.h"

#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#define MAX_THREADS 10

int currentThreads;
typedef struct ThreadParameters threadParameters_t;
struct ThreadParameters {
    int *newSock;
    node_t *log;
};

// the thread function
void *connection_handler(void *);
void interuptHandler();
void removeClient(int);
#endif
