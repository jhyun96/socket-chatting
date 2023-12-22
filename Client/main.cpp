#include <iostream>
#include "socket_client1.h"
int main(int argc, char* argv[]) {
    cout << "Hello, This is Socket Client 1!" << endl;
    socket_client1::client_first(argc, argv);
    return 0;
}
