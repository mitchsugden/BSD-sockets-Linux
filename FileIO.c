#include "FileIO.h"

/**
*   brief   Reads the information in the client details file and adds it to a data structure.
*   pre     clients == NULL
*   post    clients != NULL
**/
void readClientDetails() {
    FILE *fp = NULL;
    char line[LINE_LENGTH] = {0};
    const char delimiter[2] = ",";
    int accountNumber;

    // Open file for Reading
    if (NULL == (fp = fopen("Client_Details.txt", "r"))) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // Scan the whole first line of headings:
    fscanf(fp, "%s", line);     // First name
    fscanf(fp, "%s", line);     // Last name
    fscanf(fp, "%s", line);     // Client Number
    fscanf(fp, "%s", line);     // Accounts

    // Go through each line in the file
    for (int i = 0; EOF != fscanf(fp, "%s", line); i++) {
        // Copy the first name from the file to the clients array:
        strcpy(clients[i].firstName, line);
        fscanf(fp, "%s", line);

        // Copy the last name from the file to the clients array:
        strcpy(clients[i].lastName, line);
        fscanf(fp, "%s", line);

        // Copy the client number from the file to the clients array:
        int clientNumber = (int) strtol(line, (char **)NULL, 10);
        clients[i].clientNo = clientNumber;
        fscanf(fp, "%s", line);

        // Break the line up into a token using the delimiter:
        char *token = strtok(line, delimiter);

        while (token != NULL) {
            // Convert the token from a string to a int:
            accountNumber = (int) strtol(token, (char **)NULL, 10);

            // Check the type of account. Multiples of 11 are savings, multiples of 12 are credit and all others are loan:
            if (accountNumber % 11 == 0) {
                clients[i].savingsAccount = accountNumber;
            }
            else if (accountNumber % 12 == 0) {
                clients[i].creditAccount = accountNumber;
            }
            else {
                clients[i].loanAccount = accountNumber;
            }

            // Move on to the next account number:
            token = strtok(NULL, delimiter);
        }
    }

    // Close the file:
    fclose(fp);
}

/**
*   brief   Reads the information in the accounts file and adds it to a data structure.
*   pre     accounts == NULL
*   post    accounts != NULL
**/
void readAccountFile() {
    FILE *fp= NULL;
    char line[LINE_LENGTH] = {0};

    // Open file for Reading
    if (NULL == (fp = fopen("Accounts.txt","r")))
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // Scan the whole first line of headings:
    fscanf(fp, "%s", line); // Account number title
    fscanf(fp, "%s", line); // Opening Balance title
    fscanf(fp, "%s", line); // Closing balance title

    // Go through each line in the file
    for (int i = 0; EOF != fscanf(fp, "%s", line); i++) {
        // Read Account Number and add it to the accounts data structure:
        int accountNumber = (int) strtol(line, (char **)NULL, 10);
        accounts[i].accountNumber = accountNumber;

        // Read Opening Balance and add it to the accounts data structure:
        fscanf(fp, "%s", line);
        float openingBalance = (float) strtol(line, (char **)NULL, 10);
        accounts[i].openingBalance = openingBalance;

        // Read Closing Balance and add it to the accounts data structure:
        fscanf(fp, "%s", line);
        float closingBalance = (float) strtol(line, (char **)NULL, 10);
        accounts[i].closingBalance = closingBalance;
    }

    // Close the file:
    fclose(fp);
}

/**
*   brief   Writes the information in the accounts data structure to the accounts file.
*   pre     accounts != NULL
*   post    Accounts.txt != NULL
**/
void writeAccountsFile() {
    int i;
    FILE *accountsFile = fopen("Accounts.txt", "w");

    // Write titles:
    fprintf( accountsFile, "AccountNo ");
    fprintf( accountsFile, "OpeningBal ");
    fprintf( accountsFile, "ClosingBal \n");

    // Write file content:
    for(i = 0; i < NUM_ACCOUNTS; i++ ) {
        fprintf( accountsFile, "%d ", accounts[i].accountNumber);
        fprintf( accountsFile, "%.2f ", accounts[i].openingBalance);
        fprintf( accountsFile, "%.2f \n", accounts[i].closingBalance);
    }

    // Close the file:
    fclose(accountsFile);
}

/**
*   brief   Gets the closing balance for the given account number
*   pre     accountNumber != NULL
*   post    (return == account's closing balance)
*             OR (return == -1)
**/
float getClosingBalance(int accountNumber) {
    // Check all accounts to find the correct account number:
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        if (accountNumber == accounts[i].accountNumber) {
            return accounts[i].closingBalance;
        }
    }
    // Return -1 if no accounts exist with the given account number:
    return -1;
}

/**
*   brief   Gets the opening balance for the given account number
*   pre     accountNumber != NULL
*   post    (return == account's opening balance)
*              OR (return == -1)
**/
float getOpeningBalance(int accountNumber) {
    // Check all accounts to find the correct account number:
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        if (accountNumber == accounts[i].accountNumber) {
            return accounts[i].openingBalance;
        }
    }
    // Return -1 if no accounts exist with the given account number:
    return -1;
}

/**
*   brief   Helper function to check if an account exists
*   pre     accountNumber != NULL
*   post    (return == 1)
*              OR (return == -1)
**/
int accountExists(int accountNumber) {
    // Check all accounts and return 1 if the account is found, otherwise return -1:
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        if (accountNumber == accounts[i].accountNumber) {
            return 1;
        }
    }
    return -1;
}

// 1 for Withdrawal, 2 for Deposit
/**
*   brief   Updates the account number's closing balance
*   pre     accountNumber != NULL, adjustmentAmount != NULL, transactionType != NULL
*   post    (return == -1)
*              OR (return == updated closing balance)
**/
float appendClosingBalance(int accountNumber, float adjustmentAmount, int transactionType) {
    // Check all accounts to find the correct account number:
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        if (accountNumber == accounts[i].accountNumber) {
            // For a withdrawal:
            if (transactionType == 1) {
                if ((accounts[i].closingBalance - adjustmentAmount) < -5000) {
                    return -1;
                }
                accounts[i].closingBalance -= adjustmentAmount;
                return accounts[i].closingBalance;
            }
            // For a deposit:
            else if (transactionType == 2) {
                accounts[i].closingBalance += adjustmentAmount;
                return accounts[i].closingBalance;

            }
        }
    }
    return -1;
}

/**
*   brief   Reads the information in the transactions file and adds it to a linked list.
*   pre     transactionLog == NULL
*   post    transactionLog != NULL
**/
void readTransactionsFile() {
    FILE *fp= NULL;
    char line[LINE_LENGTH] = {0};
    head = NULL;

    // Open file for Reading
    if ( NULL == ( fp = fopen("Transactions.txt", "r") ) ) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%s", line); // From Account title
    fscanf(fp, "%s", line); // To Account title
    fscanf(fp, "%s", line); // TranType title
    fscanf(fp, "%s", line); // Amount title

    // Go through each line in the file
    for (int i = 0; EOF != fscanf(fp, "%s", line); i++) {
        transaction_t *new_transaction = (transaction_t *)malloc(sizeof(transaction_t));

        // Read From Account Number:
        int fromAccountNum = (int) strtol(line, (char **)NULL, 10);
        new_transaction->fromAccountNum = fromAccountNum;

        // Read To Account Number:
        fscanf(fp, "%s", line);
        int toAccountNum = (int) strtol(line, (char **)NULL, 10);
        new_transaction->toAccountNum = toAccountNum;

        // Read Transaction Type:
        fscanf(fp, "%s", line);
        int tranType = (int) strtol(line, (char **)NULL, 10);
        new_transaction->tranType = tranType;

        // Read Amount:
        fscanf(fp, "%s", line);
        float amount = (float) strtol(line, (char **)NULL, 10);
        new_transaction->amount = amount;

        // Add this new transaction the the linked list:
        add_node(new_transaction);
    }

    // Close the file:
    fclose(fp);
}

/**
*   brief   Writes the information in the transaction log linked list to the transactions file
*   pre     transactionLog != NULL
*   post    Transactions.txt != NULL
**/
void writeTransactionsFile() {
    node_t *ptr = head;
    FILE *transactionsFile = fopen("Transactions.txt", "w");

    // Write titles
    fprintf( transactionsFile, "FromAccount ");
    fprintf( transactionsFile, "ToAccount ");
    fprintf( transactionsFile, "TranType ");
    fprintf( transactionsFile, "Amount \n");

    // Write file content
    while(ptr != NULL) {
        fprintf( transactionsFile, "%d ", ptr->transaction->fromAccountNum);
        fprintf( transactionsFile, "%d ", ptr->transaction->toAccountNum);
        fprintf( transactionsFile, "%d ", ptr->transaction->tranType);
        fprintf( transactionsFile, "%.2f \n", ptr->transaction->amount);
        ptr = ptr->next;
    }
    fclose(transactionsFile);
}
