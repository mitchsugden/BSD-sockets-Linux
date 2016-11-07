#include<stdio.h> // printf
#include <stdlib.h> // atoi
#include<string.h> // strlen
#include<sys/socket.h> // socket
#include<arpa/inet.h> // inet_addr
#include <unistd.h> // close

#define BUFFER_SIZE 2000
#define ZERO 48
#define T 84

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE] , server_reply[BUFFER_SIZE];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return -1;
    }

    // Non-optional command line arguements
    if (argc == 3) {
        server.sin_addr.s_addr = inet_addr(argv[1]);
        server.sin_family = AF_INET;
        server.sin_port = htons( atoi(argv[2]) );
    }
    else {
        printf("Incorrect arguements - please enter the server IP address and port\ni.e ./Client [server address] [port]\n");
        return -1;
    }

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    char username[BUFFER_SIZE], pin[BUFFER_SIZE];

    memset(server_reply, 0, BUFFER_SIZE);

    //Receive the Welcome message from the server
    if( recv(sock , server_reply , BUFFER_SIZE , 0) < 0)
    {
            puts("recv failed");
            return -1;
    }

    printf("%s", server_reply);

    // Check if the server has replied with the letter T. This is the first letter of the message that there are" Too many connections...":
    if (server_reply[0] == T) {
        // Stop the program if the server has too many connections:
        return -1;
    }

    // Get Username
    printf("Please enter your Username: ");
    scanf("%s" , username);

    // Send Username
    if( send(sock , username , strlen(username) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    // Get PIN
    printf("Please enter your PIN: ");
        scanf("%s" , pin);

    // Send PIN
    if( send(sock , pin , strlen(pin) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    while(1)
    {
        //clear server_reply each time
        memset(server_reply ,0 , BUFFER_SIZE);

        // Receive a reply from the server
        if( recv(sock , server_reply , BUFFER_SIZE , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        // Check if client entered 'e' or 'E' for exit
        if (strlen(server_reply) == 1 && server_reply[0] == ZERO) {
            return 1;
        }

        printf("%s", server_reply);

        // Get user input
        scanf("%s" , message);

        // Send user input
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
    }

    // Close the socket:
    close(sock);
    return 0;
}
