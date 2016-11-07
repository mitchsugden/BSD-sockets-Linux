#include "Server.h"

/**
*   brief   Handles the interupt signal.
*   pre     Interupt signal has been hit (Ctrl + C).
*   post    Data structures are written to files and the server is shut down.
**/
void interuptHandler() {
    printf("\nSIGINT recieved...Server has been stopped...\n");

    // Server cleanup here
    writeAccountsFile();
    sortList(head);
    writeTransactionsFile();
    exit(0);
}

int main(int argc , char *argv[]) {
    // Create the linked list used to store all of the transactions:
    node_t *transactionLog = NULL;
    transaction_t *transactions;
    int numOfTransactions = 0;
    char tooManyConnectionsMessage[BUFFER_SIZE];

    strcpy(tooManyConnectionsMessage, "Too many connections, please try again later...\n");

    threadParameters_t threadArgs;
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    currentThreads = 0;

    signal(SIGINT, interuptHandler);

    // Handle command line arguements
    if (argc == 2) {
        server.sin_port = htons( *argv[1] );
    }
    else {
        server.sin_port = htons( 12345 );
    }

    // Read Files
    readAccountFile();
    readClientDetails();
    readTransactionsFile();

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    //Bind
    bind(socket_desc,(struct sockaddr *)&server , sizeof(server));
    // Listen
    listen(socket_desc , 3);
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while(client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) {
        puts("Connection accepted");
        new_sock = malloc(1);
        *new_sock = client_sock;
        threadArgs.newSock = new_sock;
        threadArgs.log = transactionLog;

        if (currentThreads < MAX_THREADS) {
            pthread_t new_thread;

            if ( pthread_create( &new_thread , NULL ,  connection_handler , (void*) &threadArgs) < 0) {
                perror("could not create thread");
                return 1;
            }
            currentThreads++;
            puts("Handler assigned");
        }
        else {
            send(*new_sock, tooManyConnectionsMessage, strlen(tooManyConnectionsMessage), 0);
        }
    }
    return 0;
}

/**
*   brief   Handles the connection for each client.
*   pre     A new thread has been created and sock != NULL
*   post    return == 0 when the client has disconnected.
**/
void *connection_handler(void *socket_desc) {
    threadParameters_t args = *(threadParameters_t *)socket_desc;

    //Get the socket descriptor
    //int sock = *(int*)socket_desc;
    int sock = *args.newSock;
    node_t *log = args.log;
    int clientNumber;
    char *welcomeMessage, *menuMessage, *client_message;
    ClientDetails client;

    client_message = malloc( sizeof (char) * BUFFER_SIZE);

    welcomeMessage = "===================================\n\n\n" \
    "Welcome to the Online ATM System\n\n\n" \
    "===================================\n\n\n" \
    "You are required to logon with your registered Username and PIN\n\n";

    menuMessage = "\nPlease enter a selection\n" \
    "<1> Account Balance\n" \
    "<2> Withdrawal\n" \
    "<3> Deposit\n" \
    "<4> Transfer\n" \
    "<5> Transaction Listing\n" \
    "<6> Exit\n" \
    "Select option 1-6 ->";

    //Send Welcome message to client
    write(sock , welcomeMessage , strlen(welcomeMessage));

    clientNumber = authenticateClient(sock);

    if (clientNumber != -1) {
        for (int i = 0; i < NUM_CLIENTS; i++) {
            if ( clients[i].clientNo == clientNumber) {
                client = clients[i];
            }
        }
    }
    else {
        close(sock);
    }

    //Receive a message from client
    while(1) {
        //Send the message back to client
        send(sock, menuMessage, strlen(menuMessage), 0);

        // Get input from client
        memset(client_message, 0, BUFFER_SIZE);
        free(client_message);
        client_message = readInput(sock);

        if(client_message[0] == 0) {
            removeClient(sock);
            break;
        }

        int input = validateInput(sock, client_message, SIX);

        if (input < 0) {
            // do nothing
        }
        else if (input == 0) {
            // TODO: exit gracefully somehow
            exit(0);
        }
        else {
            // TODO: switch->case leading to each menu option
            int clientSelection = client_message[0];

            switch(clientSelection) {
                case ONE :
                    // Account Balance
                    getAccountBalance(sock, client);
                    break;
                case TWO :
                    withdrawal(sock, client);
                    break;
                case THREE :
                    deposit(sock, client);
                    break;
                case FOUR :
                    transfer(sock, client);
                    break;
                case FIVE :
                    listTransactions(sock, client);
                    break;
                case SIX :
                    puts("Client is exiting...");
                    currentThreads--;
                    send(sock, "0", 1, 0);
                    pthread_exit(0);
                    exit(0);
            }
        }
    }
    //Free the socket pointer
    free(socket_desc);

    return 0;
}

void removeClient (int sock) {
    currentThreads--;
    send(sock, "0", 1, 0);
    pthread_exit(0);
}

