#include <iostream>
#include "socket_server.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Hello, This is Socket Server!" << endl;
    socket_server::server(argc, argv);
    return 0;
}
