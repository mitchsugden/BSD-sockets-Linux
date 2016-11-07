#include "StringBuilders.h"

// External Transfer Completed
char * externalTransferMessage(float transferAmount, int fromAccount, int destAccount, float closingBalance) {
    char *externalTransferMessage = malloc (sizeof (char) * BUFFER_SIZE);
    char intString[32];

    strcpy(externalTransferMessage, "\nEXTERNAL TRANSFER\n\nDeducted $");
    sprintf(intString, "%.2f", transferAmount);
    strcat(externalTransferMessage, intString);
    strcat(externalTransferMessage, " From: Account ");
    sprintf(intString, "%d", fromAccount);
    strcat(externalTransferMessage, intString);
    strcat(externalTransferMessage, " - Closing Balance - $");
    sprintf(intString, "%.2f", closingBalance);
    strcat(externalTransferMessage, intString);
    strcat(externalTransferMessage, "\nTransfer $");
    sprintf(intString, "%.2f", transferAmount);
    strcat(externalTransferMessage, intString);
    strcat(externalTransferMessage, " Dest: Account ");
    sprintf(intString, "%d", destAccount);
    strcat(externalTransferMessage, intString);
    strcat(externalTransferMessage, "\n\n\n==============================================================\n\n");

    return externalTransferMessage;
}

// Internal Transfer Completed
char * getInternalTransferMessage(float transferAmount, int fromAccount, int destAccount, float closingBalance, float destClosingBalance) {
    char *internalTransferMessage = malloc (sizeof (char) * BUFFER_SIZE);
    char intString[32];

    strcpy(internalTransferMessage, "\nINTERNAL TRANSFER\n\nDeducted $");
    sprintf(intString, "%.2f", transferAmount);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, " From: Account ");
    sprintf(intString, "%d", fromAccount);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, " - Closing Balance - $");
    sprintf(intString, "%.2f", closingBalance);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, "\nTransfer $");
    sprintf(intString, "%.2f", transferAmount);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, " Dest: Account ");
    sprintf(intString, "%d", destAccount);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, " - Closing Balance - $");
    sprintf(intString, "%.2f", destClosingBalance);
    strcat(internalTransferMessage, intString);
    strcat(internalTransferMessage, "\n\n\n==============================================================\n\n");

    return internalTransferMessage;

}

// Available account for transfer
char * getAvailableAccountsString(int accountNumber, ClientDetails client) {
    char intString[32];
    char *toAccountMessage = malloc (sizeof (char) * BUFFER_SIZE);
    int i = 0;

    strcpy(toAccountMessage, "\n\nSelect Account Type\n");

    if (client.creditAccount == accountNumber) {
        i++;
        sprintf(intString, "%d", i);
        strcat(toAccountMessage, intString);
        strcat(toAccountMessage, ". Savings Account\n");
    }
    else if (client.savingsAccount == accountNumber) {
        i++;
        sprintf(intString, "%d", i);
        strcat(toAccountMessage, intString);
        strcat(toAccountMessage, ". Credit Account\n");
    }

    if (client.loanAccount != 0) {
        i++;
        sprintf(intString, "%d", i);
        strcat(toAccountMessage, intString);
        strcat(toAccountMessage, ". Loan Account\n");
    }

    i++;
    sprintf(intString, "%d", i);
    strcat(toAccountMessage, intString);
    strcat(toAccountMessage, ". External Account\n\n");

    strcat(toAccountMessage, "Select Account to Transfer To (E/e to exit) ->");

    return toAccountMessage;
}
