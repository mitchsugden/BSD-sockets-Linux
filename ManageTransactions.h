#ifndef MANAGETRANSACTIONS_H
#define MANAGETRANSACTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "stdbool.h"
#include "Server.h"
#include "ManageClient.h"

#endif

void add_node(transaction_t*);
void printList();
void addTransaction(int, int, int, float);
node_t* sortList(node_t*);
node_t *head;
