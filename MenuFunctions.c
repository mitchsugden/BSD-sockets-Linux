#include "MenuFunctions.h"
#define MAXIMUM_ACCOUNT_INDEX 4

char *invalidInputMessage = "\n\nInvalid selection. Please select option from menu!\n\n";

/**
*   brief   Asks the user to select an account, and returns
*           the closing balance
*   pre     sock != NULL, client != NULL
*   post    Sends account balance message to client
**/
void getAccountBalance(int sock, ClientDetails client) {

    int i = 0, accountNumber = 0;
    char selectAccountMessage[BUFFER_SIZE], accountBalanceMessage[BUFFER_SIZE], accountBalanceMessagePrefix[BUFFER_SIZE], intString[BUFFER_SIZE], *client_message;
    char account[MAXIMUM_ACCOUNT_INDEX][LINE_LENGTH];
    float closingBalance;
    char closingBalanceString[LINE_LENGTH];

    client_message = malloc(BUFFER_SIZE);

    // Build the 'Select Account' message
    strcpy(selectAccountMessage, "\n\n===========================================================\n\n");
    strcat(selectAccountMessage, "Select Account Type\n");

    if ( client.savingsAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.savingsAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Savings Account\n");
    }
    if ( client.loanAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.loanAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Loan Account\n");
    }
    if ( client.creditAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.creditAccount);
        strcpy(account[i], intString);;
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Credit Account\n");
    }

    strcat(selectAccountMessage, "\nEnter your selection (E/e to exit) ->");

    // Build the main text of the 'Account Balance' message
    strcpy(accountBalanceMessagePrefix, "\n\n===========================================================\n\n");
    strcat(accountBalanceMessagePrefix, "Account Name - ");
    strcat(accountBalanceMessagePrefix, client.firstName);
    strcat(accountBalanceMessagePrefix, " ");
    strcat(accountBalanceMessagePrefix, client.lastName);
    strcat(accountBalanceMessagePrefix, "\n\nCurrent Balance for Account ");

    // Continue looping until client exits or selects an account
    while(1) {
        //Send the message to client
        send(sock, selectAccountMessage, strlen(selectAccountMessage), 0);

        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);
        int input = validateInput(sock, client_message, i);

        if (input < 0) {
            // do nothing, repreat input loop
        }
        else {
                // Convert char to int by negating the ASCII character zero
                int clientSelection = client_message[0] - '0';

                // Exit for 'E' or 'e'
                if ( client_message[0] == E || client_message[0] == e ) {
                    if (strlen(client_message) <= 1) {
                        // Clear the memory
                        memset(accountBalanceMessage,0 , BUFFER_SIZE);
                        memset(client_message, 0, BUFFER_SIZE);
                        memset(accountBalanceMessagePrefix, 0, BUFFER_SIZE);
                        memset(selectAccountMessage, 0, BUFFER_SIZE);
                        memset(intString, 0, BUFFER_SIZE);
                        memset(account, 0, BUFFER_SIZE);
                        memset(closingBalanceString, 0, LINE_LENGTH);
                        break;
                    }
                }
                // Add the main text of the 'Account Balance' message
                strcat(accountBalanceMessage, accountBalanceMessagePrefix);
                strcat(accountBalanceMessage, account[clientSelection]);
                strcat(accountBalanceMessage, " : $");
                // Store account number and closing balance, then concatinate to message
                accountNumber = atoi(account[clientSelection]);
                closingBalance = getClosingBalance(accountNumber);
                sprintf(closingBalanceString, "%.2f", closingBalance);
                strcat(accountBalanceMessage, closingBalanceString);
                strcat(accountBalanceMessage, "\n\n===========================================================\n\n");
                // Send the completed message to the client
                send(sock, accountBalanceMessage, strlen(accountBalanceMessage), 0);
                // Clear the memory
                memset(accountBalanceMessage,0 , BUFFER_SIZE);
                memset(client_message, 0, BUFFER_SIZE);
                memset(accountBalanceMessagePrefix, 0, BUFFER_SIZE);
                memset(selectAccountMessage, 0, BUFFER_SIZE);
                memset(intString, 0, BUFFER_SIZE);
                memset(account, 0, BUFFER_SIZE);
                memset(closingBalanceString, 0, LINE_LENGTH);
                // End while loop
                break;
            }
        }
}

/**
*   brief   Takes the client through a withdrawal
*   pre     sock != NULL, client != NULL
*   post    Creates a transaction, adjusts clients closing balance
**/
void withdrawal(int sock, ClientDetails client) {
    int i = 1, accountNumber = 0, numberOfAccounts;
    char selectAccountMessage[BUFFER_SIZE], intString[BUFFER_SIZE], enterAmountMessage[BUFFER_SIZE], completedMessage[BUFFER_SIZE], insufficientFundsMessage[BUFFER_SIZE], garbage;
    char *client_message;
    char account[MAXIMUM_ACCOUNT_INDEX][LINE_LENGTH];
    float withdrawalAmount = 0, closingBalance = 0;

    client_message = malloc(BUFFER_SIZE);

    // Build strings
    strcpy(insufficientFundsMessage, "\nInsufficient Funds - Unable to process request\n");
    strcpy(enterAmountMessage, "\nEnter the amount to withdraw (E/e to exit) -> $");
    strcpy(completedMessage, "\n\nWithdrawal Completed: Closing Balance : $");

    // Build 'Select Account' message
    strcpy(selectAccountMessage, "\n===========================================================\n\n\n");
    strcat(selectAccountMessage, "Select Account Type\n");

    if (client.savingsAccount != 0) {
        sprintf(intString, "%d", client.savingsAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Savings Account\n");
        i++;
    }

    if (client.creditAccount != 0) {
        sprintf(intString, "%d", client.creditAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Credit Account\n");
    }

    strcat(selectAccountMessage, "\nSelect Account Type (E/e to exit) ->");

    // Loop until the client has selected an account or exited
    while(1) {
        //Send the message back to client
        send(sock, selectAccountMessage, strlen(selectAccountMessage), 0);
        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);
        int input = validateInput(sock, client_message, i);

        if (input < 0) {
            // do nothing, repeat input loop
        }
        else {
                // Convert char to int by negating the ASCII character zero
                int clientSelection = client_message[0] - '0';

                // Loop until client has entered amount or exited
                while(1) {
                    // Send message to client
                    send(sock, enterAmountMessage, strlen(enterAmountMessage), 0);
                    // Read reply from client
                    memset(client_message, 0, BUFFER_SIZE);
                    free(client_message);
                    client_message = readInput(sock);
                    // Exit for 'E' or 'e'
                    if ( client_message[0] == E || client_message[0] == e ) {
                        if (strlen(client_message) <= 1) {
                                // Free memory
                                memset(completedMessage, 0 , BUFFER_SIZE);
                                memset(insufficientFundsMessage, 0 , BUFFER_SIZE);
                                memset(enterAmountMessage, 0 , BUFFER_SIZE);
                                memset(intString, 0 , BUFFER_SIZE);
                                memset(selectAccountMessage, 0 , BUFFER_SIZE);
                                break;
                            }
                        }
                    // Check amount input
                    if ((withdrawalAmount = checkFloat(sock, client_message)) > 0) {
                        accountNumber = atoi(account[clientSelection]);
                        closingBalance = appendClosingBalance(accountNumber, withdrawalAmount, 1);
                        // Check if appendClosingBalance returned an error (insufficient funds)
                        if (closingBalance != -1) {
                            // Build message to client
                            sprintf(intString, "%.2f", closingBalance);
                            strcat(completedMessage, intString);
                            strcat(completedMessage, "\n\n\n===========================================================\n\n\n");
                            // Send completed message to the client
                            send(sock, completedMessage, strlen(completedMessage), 0);
                            // Add a transaction to the list
                            addTransaction(accountNumber, accountNumber, 2, -withdrawalAmount);
                            // Free memory
                            memset(completedMessage, 0 , BUFFER_SIZE);
                            memset(insufficientFundsMessage, 0 , BUFFER_SIZE);
                            memset(enterAmountMessage, 0 , BUFFER_SIZE);
                            memset(intString, 0 , BUFFER_SIZE);
                            memset(selectAccountMessage, 0 , BUFFER_SIZE);
                            // End nested while loop
                            break;
                        }
                        else {
                            send(sock, insufficientFundsMessage, strlen(insufficientFundsMessage), 0);
                        }
                    }
                } // end while
                // end main while loop
                break;
            }
        }
}

/**
*   brief   Takes the client through a deposit
*   pre     sock != NULL, client != NULL
*   post    Creates a transaction, adjusts clients closing balance
**/
void deposit(int sock, ClientDetails client) {
    int i = 0, accountNumber = 0;
    char selectAccountMessage[BUFFER_SIZE], intString[BUFFER_SIZE], enterAmountMessage[BUFFER_SIZE], completedMessage[BUFFER_SIZE], warningMessage[BUFFER_SIZE], *client_message;
    char account[MAXIMUM_ACCOUNT_INDEX][LINE_LENGTH];
    float closingBalance, depositAmount;


    client_message = malloc(BUFFER_SIZE);

    // Build strings
    strcpy(enterAmountMessage, "\nEnter the amount to deposit (E/e to exit) -> $");
    strcpy(completedMessage, "\n\nDeposit Completed: Closing Balance : $");
    strcpy(warningMessage, "\n\nYou can not deposit more than $1000 in a single transaction!");
    strcpy(invalidAmountMessage, "\nInvalid Input! Please enter a valid amount\n");

    // Build 'Select Account' message
    strcpy(selectAccountMessage, "\n\n===========================================================\n\n");
    strcat(selectAccountMessage, "The maximum daily deposit is $1000.00!\n\n\n");
    strcat(selectAccountMessage, "Select Account Type\n");

    if ( client.savingsAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.savingsAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Savings Account\n");
    }
    if ( client.loanAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.loanAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Loan Account\n");
    }
    if ( client.creditAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.creditAccount);
        strcpy(account[i], intString);;
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Credit Account\n");
    }

    strcat(selectAccountMessage, "\nSelect Account Type (E/e to exit) ->");

    // Loop until client has select an account or exited
    while(1) {
        //Send the message back to client
        send(sock, selectAccountMessage, strlen(selectAccountMessage), 0);
        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);

        int input = validateInput(sock, client_message, i);

        if (input < 0) {
            // do nothing
        }
        else {
            // Convert char to int by negating the ASCII character zero
            int clientSelection = client_message[0] - '0';

            // Loop until client has entered amount or exited
            while(1) {
                // Send message to client
                send(sock, enterAmountMessage, strlen(enterAmountMessage), 0);
                // Read reply from client
                client_message = readInput(sock);
                // Exit for 'E' or 'e'
                if ( client_message[0] == E || client_message[0] == e ) {
                    if (strlen(client_message) <= 1) {
                        // Free memory
                        memset(selectAccountMessage, 0, BUFFER_SIZE);
                        memset(intString, 0, BUFFER_SIZE);
                        memset(enterAmountMessage, 0, BUFFER_SIZE);
                        memset(completedMessage, 0, BUFFER_SIZE);
                        memset(warningMessage, 0, BUFFER_SIZE);
                        break;
                    }
                }
                // Check if the amount entered is a float
                if ( (depositAmount = checkFloat(sock, client_message)) > 0 ) {
                    // Check if deposit amount is between 0 and 1000 inclusively
                    if (depositAmount <= 1000 && depositAmount > 0) {
                        // Store account number and closing balance
                        accountNumber = atoi(account[clientSelection]);
                        closingBalance = appendClosingBalance(accountNumber, depositAmount, 2);
                        // Add to message
                        sprintf(intString, "%.2f", closingBalance);
                        strcat(completedMessage, intString);
                        strcat(completedMessage, "\n\n\n===========================================================\n\n");
                        // Send message to client
                        send(sock, completedMessage, strlen(completedMessage), 0);
                        // Add transaction to list
                        addTransaction(accountNumber, accountNumber, 3, depositAmount);
                        // Free memory
                        memset(selectAccountMessage, 0, BUFFER_SIZE);
                        memset(intString, 0, BUFFER_SIZE);
                        memset(enterAmountMessage, 0, BUFFER_SIZE);
                        memset(completedMessage, 0, BUFFER_SIZE);
                        memset(warningMessage, 0, BUFFER_SIZE);
                        break;
                    }
                    else {
                        send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
                    }
                }
            }
            // End while loop
            break;
        }
    }
}

/**
*   brief   Takes the client through a transfer
*   pre     sock != NULL, client != NULL
*   post    Creates a transaction, adjusts clients closing balance
**/
void transfer(int sock, ClientDetails client) {
    int i = 1, fromAccount = 0, destAccount = 0, j, *accounts;
    char selectAccountMessage[BUFFER_SIZE], *internalTransferMessage, numberOfAccounts, invalidAccountMessage[BUFFER_SIZE], *sendBuffer, destinationAccountMessage[BUFFER_SIZE], transferAmountMessage[BUFFER_SIZE], invalidAmountMessage[BUFFER_SIZE], *selectToAccountMessage, intString[BUFFER_SIZE], *client_message;
    float withdrawalAmount = 0, closingBalance, destClosingBalance, transferAmount = 0, fromAccountBalance = 0;
    char account[MAXIMUM_ACCOUNT_INDEX][LINE_LENGTH];
    char garbage;

    client_message = malloc(BUFFER_SIZE);
    selectToAccountMessage = malloc(BUFFER_SIZE);
    sendBuffer = malloc(BUFFER_SIZE);
    internalTransferMessage = malloc(BUFFER_SIZE);

    // Build strings
    strcpy(destinationAccountMessage, "\nEnter Destination Account Number (E/e to exit) ->");
    strcpy(transferAmountMessage, "\nEnter the Amount to Transfer (E/e to exit) -> $");
    strcpy(invalidAccountMessage, "\nInvalid Destination Account Number - Try Again\n");
    strcpy(invalidAmountMessage, "\nInvalid Amount - Enter a Valid Amount to Transfer\n");

    // Build 'Select Account' message
    strcpy(selectAccountMessage, "\n===========================================================\n\n\n");
    strcat(selectAccountMessage, "Select Account Type\n");

    if (client.savingsAccount != 0) {
        sprintf(intString, "%d", client.savingsAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Savings Account\n");
        i++;
    }

    if (client.creditAccount != 0) {
        sprintf(intString, "%d", client.creditAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Credit Account\n");
    }

    strcat(selectAccountMessage, "\nSelect Account to Transfer From (E/e to exit) ->");

    // Loop until client enters an account or exits
    while(1) {
        //Send the message back to client
        send(sock, selectAccountMessage, strlen(selectAccountMessage), 0);
        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);
        int input = validateInput(sock, client_message, SIX);

        if (input < 0) {
            // do nothing, repeat input loop
        }
        else {
            // Convert char to int by negating the ASCII character zero
            int clientSelection = client_message[0] - '0';
            // Store int from selected account
            fromAccount = atoi(account[clientSelection]);
            fromAccountBalance = getClosingBalance(fromAccount);
            // Get string and accounts for next menu
            selectToAccountMessage = getAvailableAccountsString(fromAccount, client);
            accounts = getAvailableAccounts(fromAccount, client);

            // Loop until client enters a destination account or exits
            while(1) {
                // Send 'to' account list
                send(sock, selectToAccountMessage, strlen(selectToAccountMessage), 0);
                // Read 'To' account
                memset(client_message, 0, BUFFER_SIZE);
                free(client_message);
                client_message = readInput(sock);
                int input = validateInput(sock, client_message, numberOfAccounts + 1);

                if (input < 0) {
                    // do nothing, repeat input loop
                }
                else {
                    // Convert char to int by negating the ASCII character zero
                    clientSelection = client_message[0] - '0';

                    // If external account
                    if (accounts[clientSelection-1] == 0) {
                        // Loop until the client has entered a destination account or exited
                        while (1) {
                            // Ask for destination account
                            send(sock, destinationAccountMessage, strlen(destinationAccountMessage), 0);
                            // Check destination account
                            client_message = readInput(sock);
                            destAccount = atoi(client_message);
                            // Exit for 'E' or 'e'
                            if ( client_message[0] == E || client_message[0] == e ) {
                                if (strlen(client_message) <= 1) {
                                        // Free memory
                                        memset(selectAccountMessage, 0, BUFFER_SIZE);
                                        memset(sendBuffer, 0, BUFFER_SIZE);
                                        memset(internalTransferMessage, 0, BUFFER_SIZE);
                                        memset(invalidAccountMessage, 0, BUFFER_SIZE);
                                        memset(destinationAccountMessage, 0, BUFFER_SIZE);
                                        memset(transferAmountMessage, 0, BUFFER_SIZE);
                                        memset(invalidAmountMessage, 0, BUFFER_SIZE);
                                        memset(intString, 0, BUFFER_SIZE);
                                        break;
                                    }
                                }
                            // Check is destination account belongs to client, and that the account exists
                            if ((destAccount != client.savingsAccount && destAccount != client.creditAccount && destAccount != client.loanAccount) && (j = accountExists(destAccount) > 0)) {
                                // Loop until the client enters an amount or exits
                                while(1) {
                                    // Ask for amount
                                    send(sock, transferAmountMessage, strlen(transferAmountMessage), 0);
                                    // Recieve amount
                                    client_message = readInput(sock);
                                    // Exit for 'E' or 'e'
                                    if ( client_message[0] == E || client_message[0] == e ) {
                                        if (strlen(client_message) <= 1) {
                                                // Free memory
                                                memset(selectAccountMessage, 0, BUFFER_SIZE);
                                                memset(sendBuffer, 0, BUFFER_SIZE);
                                                memset(internalTransferMessage, 0, BUFFER_SIZE);
                                                memset(invalidAccountMessage, 0, BUFFER_SIZE);
                                                memset(destinationAccountMessage, 0, BUFFER_SIZE);
                                                memset(transferAmountMessage, 0, BUFFER_SIZE);
                                                memset(invalidAmountMessage, 0, BUFFER_SIZE);
                                                memset(intString, 0, BUFFER_SIZE);
                                                break;
                                            }
                                        }
                                    // Check if input is a float or combination or chars and ints
                                    if (sscanf(client_message, "%f%c", &transferAmount, &garbage) != 1) {
                                        send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
                                    }
                                    else {
                                        // Check account boundaries, 0 for savings and -5000 for credit.
                                        if ((fromAccount == client.savingsAccount && fromAccountBalance - transferAmount >= 0) || (fromAccount == client.creditAccount && (fromAccountBalance - transferAmount) >= -5000)) {
                                            // Transfer funds
                                            closingBalance = appendClosingBalance(fromAccount, transferAmount, 1);
                                            destClosingBalance = appendClosingBalance(destAccount, transferAmount, 2);

                                            // Send the external transfer message
                                            sendBuffer = externalTransferMessage(transferAmount, fromAccount, destAccount, closingBalance);
                                            send(sock, sendBuffer, strlen(sendBuffer), 0);
                                            addTransaction(fromAccount, destAccount, 4, transferAmount);

                                            // Free memory
                                            memset(selectAccountMessage, 0, BUFFER_SIZE);
                                            memset(sendBuffer, 0, BUFFER_SIZE);
                                            memset(internalTransferMessage, 0, BUFFER_SIZE);
                                            memset(invalidAccountMessage, 0, BUFFER_SIZE);
                                            memset(destinationAccountMessage, 0, BUFFER_SIZE);
                                            memset(transferAmountMessage, 0, BUFFER_SIZE);
                                            memset(invalidAmountMessage, 0, BUFFER_SIZE);
                                            memset(intString, 0, BUFFER_SIZE);
                                            // End the loop
                                            break;
                                        }
                                        else {
                                            // send invalid input: destination account belongs to client
                                            send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
                                        }
                                    }
                                }
                                // End outer while
                                break;
                            } // end if
                            else {
                                // send invalid input: destination account belongs to client
                                send(sock, invalidAccountMessage, strlen(invalidAccountMessage), 0);
                            }
                        } // end while
                } // end if

            // Internal transfer
            else {
                destAccount = accounts[(atoi(client_message)-1)];
                // Loop until client enters amount or exits
                while (1) {
                    // Ask for amount
                    send(sock, transferAmountMessage, strlen(transferAmountMessage), 0);
                    // Recieve amount
                    client_message = readInput(sock);
                    // Exit for 'E' or 'e'
                    if ( client_message[0] == E || client_message[0] == e ) {
                        if (strlen(client_message) <= 1) {
                                break;
                            }
                        }
                    // Check if input is a float
                    if (sscanf(client_message, "%f%c", &transferAmount, &garbage) != 1) {
                        send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
                    }
                    else {
                            // Check account boundaries, 0 for saving and -5000 for credit.
                            if ((fromAccount == client.savingsAccount && fromAccountBalance - transferAmount >= 0) || (fromAccount == client.creditAccount && (fromAccountBalance - transferAmount) >= -5000)) {
                            // Do transfer
                            closingBalance = appendClosingBalance(fromAccount, transferAmount, 1);
                            destClosingBalance = appendClosingBalance(destAccount, transferAmount, 2);

                            // Send message
                            internalTransferMessage = getInternalTransferMessage(transferAmount, fromAccount, destAccount, closingBalance, destClosingBalance);
                            send(sock, internalTransferMessage, strlen(internalTransferMessage), 0);
                            addTransaction(fromAccount, destAccount, 4, transferAmount);

                            // Free memory
                            memset(selectAccountMessage, 0, BUFFER_SIZE);
                            memset(sendBuffer, 0, BUFFER_SIZE);
                            memset(internalTransferMessage, 0, BUFFER_SIZE);
                            memset(invalidAccountMessage, 0, BUFFER_SIZE);
                            memset(destinationAccountMessage, 0, BUFFER_SIZE);
                            memset(transferAmountMessage, 0, BUFFER_SIZE);
                            memset(invalidAmountMessage, 0, BUFFER_SIZE);
                            memset(intString, 0, BUFFER_SIZE);
                            break;
                        }
                        else {
                        printf("Send the invalid message");
                            // send invalid input: destination account belongs to client
                            send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
                        }
                    }
                }
            }
            break;
        }
        } // end While
        break;
     }
    }// end while
}

/**
*   brief   Prompts for account selection and displays the transactions for that account
*   pre     sock != NULL, client != NULL
*   post    Prints all transactions from the list to the client
**/
void listTransactions(int sock, ClientDetails client) {
    int i = 0, accountNumber = 0, numTransactions = 0;
    char selectAccountMessage[BUFFER_SIZE], transactionListingMessage[BUFFER_SIZE], transactionsString[BUFFER_SIZE], intString[BUFFER_SIZE], *client_message;
    char account[MAXIMUM_ACCOUNT_INDEX][LINE_LENGTH];
    float closingBalance;

    client_message = malloc(BUFFER_SIZE);
    node_t *ptr = head;

    // Build 'Select Account' message
    strcpy(selectAccountMessage, "\n\n===========================================================\n\n");
    strcat(selectAccountMessage, "Select Account Type\n");

    if ( client.savingsAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.savingsAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Savings Account\n");
    }
    if ( client.loanAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.loanAccount);
        strcpy(account[i], intString);
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Loan Account\n");
    }
    if ( client.creditAccount > 0 ) {
        i++;
        sprintf(intString, "%d", client.creditAccount);
        strcpy(account[i], intString);;
        sprintf(intString, "%d", i);
        strcat(selectAccountMessage, intString);
        strcat(selectAccountMessage, ". Credit Account\n");
    }

    strcat(selectAccountMessage, "\nSelect account type for transaction listing (E/e to exit) ->");

    // Build transaction list message
    strcpy(transactionListingMessage, "\n\n===========================================================\n\n");
    strcat(transactionListingMessage, "Account Owner: ");
    strcat(transactionListingMessage, client.firstName);
    strcat(transactionListingMessage, " ");
    strcat(transactionListingMessage, client.lastName);
    strcat(transactionListingMessage, "\n\n");

    // Loop until client selects account or exits
    while(1) {
        //Send the message back to client
        send(sock, selectAccountMessage, strlen(selectAccountMessage), 0);
        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);
        int input = validateInput(sock, client_message, i);

        if (input < 0) {
            // do nothing
        }
        else {
            // Convert char to int by negating the ASCII character zero
            int clientSelection = client_message[0] - '0';
            // Exit for 'E' or 'e'
            if ( client_message[0] == E || client_message[0] == e ) {
                if (strlen(client_message) <= 1) {
                        // Free memory
                        memset(selectAccountMessage, 0, BUFFER_SIZE);
                        memset(intString, 0, BUFFER_SIZE);
                        memset(transactionsString, 0, BUFFER_SIZE);
                        memset(transactionListingMessage, 0, BUFFER_SIZE);
                        break;
                    }
            }
            accountNumber = atoi(account[clientSelection]);
                // Iterate through linked list
                while(ptr != NULL) {
                    // Check if account number is present in 'FromAccount'
                    if (ptr->transaction->fromAccountNum == accountNumber) {
                        if (ptr->transaction->tranType == 2) {
                            numTransactions++;
                            sprintf(intString, "     %d", numTransactions);
                            strcat(transactionsString, intString);
                            strcat(transactionsString, "          Withdrawal");
                            sprintf(intString, "         %.2f\n", ptr->transaction->amount);
                            strcat(transactionsString, intString);
                        }
                        if (ptr->transaction->tranType == 3) {
                            numTransactions++;
                            sprintf(intString, "     %d", numTransactions);
                            strcat(transactionsString, intString);
                            strcat(transactionsString, "          Deposit");
                            sprintf(intString, "             %.2f\n", ptr->transaction->amount);
                            strcat(transactionsString, intString);
                        }
                        if (ptr->transaction->tranType == 4) {
                            numTransactions++;
                            sprintf(intString, "     %d", numTransactions);
                            strcat(transactionsString, intString);
                            strcat(transactionsString, "          Transfer");
                            sprintf(intString, "           -%.2f\n", ptr->transaction->amount);
                            strcat(transactionsString, intString);
                        }
                    }
                    // Check if account number is present in "ToAccount" for transfers
                    if (ptr->transaction->toAccountNum == accountNumber) {
                            if (ptr->transaction->tranType == 4) {
                            numTransactions++;
                            sprintf(intString, "     %d", numTransactions);
                            strcat(transactionsString, intString);
                            strcat(transactionsString, "          Transfer");
                            sprintf(intString, "            %.2f\n", ptr->transaction->amount);
                            strcat(transactionsString, intString);
                        }
                    }
                    // increment iterating pointer
                    ptr = ptr->next;
                } // end while

                // Get opening and closing balance to add to string
                int closingBalance = getClosingBalance(accountNumber);
                int openingBalance = getOpeningBalance(accountNumber);

                // Now that we know how many transactions there are, build the string
                strcat(transactionListingMessage, "Total number of transactions: ");
                sprintf(intString, "%d", numTransactions);
                strcat(transactionListingMessage, intString);
                strcat(transactionListingMessage, "\n\nOpening Balance - Account ");
                sprintf(intString, "%d", accountNumber);
                strcat(transactionListingMessage, intString);
                strcat(transactionListingMessage, " : $");
                sprintf(intString, "%d", openingBalance);
                strcat(transactionListingMessage, intString);
                strcat(transactionListingMessage, "\n\n===========================================================\n");
                strcat(transactionListingMessage, "Transaction         Type            Amount\n");
                strcat(transactionListingMessage, transactionsString);
                strcat(transactionListingMessage, "\nClosing Balance : $");
                sprintf(intString, "%d", closingBalance);
                strcat(transactionListingMessage, intString);
                strcat(transactionListingMessage, "\n===========================================================\n\n");
                // Send message to client
                send(sock, transactionListingMessage, strlen(transactionListingMessage), 0);
                // Free memory
                memset(selectAccountMessage, 0, BUFFER_SIZE);
                memset(intString, 0, BUFFER_SIZE);
                memset(transactionsString, 0, BUFFER_SIZE);
                memset(transactionListingMessage, 0, BUFFER_SIZE);
                break;
            }
        }
}
