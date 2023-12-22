//
// Created by test on 12/21/23.
//

#ifndef CLIENT1_SOCKET_CLIENT1_H
#define CLIENT1_SOCKET_CLIENT1_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <ctime>

#define BUF_SIZE 100
#define NORMAL_SIZE 20


#define CLIENT_NAME "client1"
using namespace std;


class socket_client1 {
public:
    static bool client_first(int argc, char* argv[]);
private:
    static void receiveMessages(int clientSocket);
    static void* send_msg(void* arg);
    static void* recv_msg(void* arg);
    static void menuOptions();
    static void changeName();
    static void menu();
    static void error_handling(const char* msg);
};


#endif //CLIENT1_SOCKET_CLIENT1_H
