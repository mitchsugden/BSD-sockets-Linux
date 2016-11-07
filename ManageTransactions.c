#include "ManageTransactions.h"

void add_node(transaction_t *new_transaction) {
    node_t *newHead = (node_t*) malloc(sizeof(node_t));

    newHead->transaction = new_transaction;
    newHead->next = head;
    head = newHead;
}

void add_node_to_end(transaction_t *new_transaction) {
    node_t *ptr = head;
    node_t *newHead = (node_t*) malloc(sizeof(node_t));
    newHead->transaction = new_transaction;
    newHead->next = NULL;

    while(ptr->next != NULL) {
        ptr = ptr->next;
    }

    ptr->next = newHead;
}

void printList() {
    node_t *ptr = head;

    printf("\n[head] =>");
    while(ptr != NULL) {
        printf(" %d =>", ptr->transaction->fromAccountNum);
        ptr = ptr->next;
    }
    printf(" [null]\n");
}

void addTransaction(int fromAccountNum, int toAccountNum, int tranType, float amount) {
    transaction_t *new_transaction = (transaction_t *)malloc(sizeof(transaction_t));

    new_transaction->fromAccountNum = fromAccountNum;
    new_transaction->toAccountNum = toAccountNum;
    new_transaction->tranType = tranType;
    new_transaction->amount = amount;

    add_node_to_end(new_transaction);
}

node_t *sortList(node_t *head) {

    node_t *tmpPtr = head;
    node_t *tmpNxt = head->next;

    transaction_t *tmp;

    while(tmpNxt != NULL){
           while(tmpNxt != tmpPtr){
                    if(tmpNxt->transaction->fromAccountNum < tmpPtr->transaction->fromAccountNum){
                            tmp = tmpPtr->transaction;
                            tmpPtr->transaction = tmpNxt->transaction;
                            tmpNxt->transaction = tmp;
                    }
                    tmpPtr = tmpPtr->next;
            }
            tmpPtr = head;
            tmpNxt = tmpNxt->next;
    }
         return tmpPtr ; // Place holder
}
