#include "ManageClient.h"

/**
*   brief   Reads the input from the client
*   pre     sock != NULL
*   post    (return == the clients input)
*              OR (return == NULL)
**/
char * readInput (int sock) {
    int read_size;
    char *client_message = malloc (sizeof (char) * BUFFER_SIZE);
    fflush(stdout);
    memset(client_message, 0, BUFFER_SIZE);

    // Get Username from client
    if ( (read_size = recv(sock , client_message , BUFFER_SIZE , 0)) > 0 ) {
        return client_message;
    }

    // If the client has disconnected, read_size will be to the "NULL" character which is 0 on the ascii table:
    if (read_size == 0) {
        removeClient(sock);
        fflush(stdout);
        return client_message;
    }
    else if (read_size == -1) {
        perror("recv failed");
    }

    return client_message;
}

/**
*   brief   Authenticates the client's username and PIN
*   pre     sock != NULL
*   post    (return == the client's client number)
*              OR (return == -1)
**/
int authenticateClient(int sock) {
    FILE *fp = NULL;
    char line[LINE_LENGTH] = {0};
    int clientNumber = -1;
    char *username, *pin;
    // Get Username from client
    username = readInput(sock);

    // Get PIN from client
    pin = readInput(sock);

     // Open file for Reading
    if (NULL == (fp = fopen("Authentication(1).txt","r"))) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    // Go through each line in the file
    while (EOF != fscanf(fp, "%s", line)) {
        // Check if the line matches the username
        if (strcmp(username, line) == 0) {
            // If it does, advance 1 line to the PIN
            fscanf(fp, "%s", line);

            // Check if the PIN matches
            if (strcmp(pin, line) == 0) {
                fscanf(fp, "%s", line);
                clientNumber = atoi(line);
                break;
            }
        }
    }

    // Close the file:
    fclose(fp);
    return clientNumber;
}

/**
*   brief   Helper function to get the number of accounts available to the client
*   pre     accountNumber != NULL, client != NULL
*   post    (return == the accounts available)
**/
int * getAvailableAccounts(int accountNumber, ClientDetails client) {
    int i = 0;
    static int accounts[3];

    // Should only be adding either a savings account or a credit account:
    if (client.creditAccount == accountNumber) {
        accounts[i] = client.savingsAccount;
        i++;
    }
    else if (client.savingsAccount == accountNumber) {
        accounts[i] = client.creditAccount;
        i++;
    }

    // Add a loan account if the client has one:
    if (client.loanAccount != 0) {
        accounts[i] = client.loanAccount;
        i++;
    }

    // 0 for external account
    accounts[i] = 0;

    return accounts;
}

/**
*   brief   Validates the input from the client to check which menu option the client has selected.
*   pre     sock != NULL, client_message != NULL, numOptions != NULL
*   post    (return == -1) OR (return == 0) OR (return == 1)
**/
int validateInput(int sock, char * client_message, int numOptions) {
    // Convert int to Ascii value by adding the character zero
    numOptions += '0';

    // Make sure the client_message is not too long:
    if (strlen(client_message) > 1) {
        send(sock, invalidInputMessage, strlen(invalidInputMessage), 0);
        return -1;
    }
    // Return 0 for 'e' or 'E'
    else if ( client_message[0] == e || client_message[0] == E ) {
            return 0;
    }
    else if (client_message[0] < ONE || client_message[0] > numOptions) {
        send(sock, invalidInputMessage, strlen(invalidInputMessage), 0);
        return -1;
    }

    return 1;
}

/**
*   brief   Checks if the clients input is a mixture of characters and numbers
*   pre     sock != NULL, client_message != NULL
*   post    (return == -1) OR (return == the client's desired withdrawal amount)
**/
float checkFloat(int sock, char * client_message) {
    float withdrawalAmount = 0;
    char garbage;

    strcpy(invalidAmountMessage, "\nInvalid Input! Please enter a valid amount\n");

    if (sscanf(client_message, "%f%c", &withdrawalAmount, &garbage) != 1) {
        send(sock, invalidAmountMessage, strlen(invalidAmountMessage), 0);
        return -1;
    }
    else {
        return withdrawalAmount;
    }
}
