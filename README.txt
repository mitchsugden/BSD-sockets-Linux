|----COMMAND LINE ARGUEMENTS----|

Compile Server:
gcc Server.c FileIO.c ManageClient.c MenuFunctions.c StringBuilders.c ManageTransactions.c -o Server -lpthread [optional: port]
Run Server:
./Server

Compile Client:
gcc -o Client Client.c
Run Client:
./Client [server IP] [port]

