//
// Created by test on 12/21/23.
//

#ifndef SOCKET_SERVER_SOCKET_SERVER_H
#define SOCKET_SERVER_SOCKET_SERVER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 100
#define MAX_CLNT 100


using namespace std;




class socket_server {
public:
    static bool server(int argc, char* argv[]);
    

private:
    static void handleClient(int clientSocket);
    static void sendMessage(const char* message, int length);
    static void reportError(const char *errorMessage);
    static const char* serverStatus(int count);
    static void displayMenu(const char port[]);
};


#endif //SOCKET_SERVER_SOCKET_SERVER_H
