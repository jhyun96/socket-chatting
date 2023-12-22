//
// Created by test on 12/21/23.
//

#include "socket_server.h"

vector<int> clientSockets;
mutex mutexLock;


bool socket_server::server(int argc, char* argv[]) {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;
    thread threadId;

    time_t timer = time(nullptr);
    struct tm *timeInfo = localtime(&timer);

    if (argc != 2) {
        cout << "Usage : " << argv[0] << " <port>\n";
        exit(1);
    }

    displayMenu(argv[1]);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        reportError("socket() error");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1)
        reportError("bind() error");
    if (listen(serverSocket, 5) == -1)
        reportError("listen() error");

    while (true) {
        timeInfo = localtime(&timer);
        clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrSize);

        mutexLock.lock();
        clientSockets.push_back(clientSocket);
        mutexLock.unlock();

        threadId = thread(handleClient, clientSocket);
        threadId.detach();
        cout << "Connected client IP : " << inet_ntoa(clientAddr.sin_addr);
        cout << " (" << timeInfo->tm_year + 1900 << "-" << timeInfo->tm_mon + 1 << "-" << timeInfo->tm_mday << " ";
        cout << timeInfo->tm_hour << ":" << timeInfo->tm_min << ")\n";
        cout << "chatter (" << clientSockets.size() << "/100)\n";
    }
    close(serverSocket);
}


void socket_server::handleClient(int clientSocket) {
    int strLength = 0;
    char message[BUF_SIZE];

    while ((strLength = read(clientSocket, message, sizeof(message))) != 0)
        sendMessage(message, strLength);

    // remove disconnected client
    mutexLock.lock();
    for (auto it = clientSockets.begin(); it != clientSockets.end(); ++it) {
        if (*it == clientSocket) {
            clientSockets.erase(it);
            break;
        }
    }
    mutexLock.unlock();
    close(clientSocket);
}

void socket_server::sendMessage(const char* message, int length) {
    mutexLock.lock();
    for (int clientSocket : clientSockets)
        write(clientSocket, message, length);
    mutexLock.unlock();
}

void socket_server::reportError(const char *errorMessage) {
    cerr << errorMessage << endl;
    exit(1);
}

const char* socket_server::serverStatus(int count) {
    if (count < 5)
        return "Good";
    else
        return "Bad";
}

void socket_server::displayMenu(const char port[]) {
    system("clear");
    cout << "### [jhyun chat server] ###" << endl;
    cout << "##\t\t\t ##" << endl;
    cout << "## server port   : " << port << "  ##" << endl;
    cout << "## server state  : " << serverStatus(clientSockets.size()) << "  ##" << endl;
    cout << "## max connection : " << MAX_CLNT << "  ##" << endl;
    cout << "##\t\t\t ##" << endl;
    cout << "###########################" <<endl;

    cout << "           [Log]            " << endl;
}