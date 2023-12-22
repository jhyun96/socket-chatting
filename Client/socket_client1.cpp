//
// Created by test on 12/21/23.
//

#include "socket_client1.h"

char name[NORMAL_SIZE]="[DEFALT]";     // name
char msg_form[NORMAL_SIZE];            // msg form
char serv_time[NORMAL_SIZE];        // server time
char msg[BUF_SIZE];                    // msg
char serv_port[NORMAL_SIZE];        // server port number
char clnt_ip[NORMAL_SIZE];            // client ip address

void* socket_client1::send_msg(void* arg)
{
    int sock = *((int*)arg);
    char name_msg[NORMAL_SIZE + BUF_SIZE];
    char myInfo[BUF_SIZE];

    /** send join message **/
    cout << " >> join the chat !! \n";
    sprintf(myInfo, "%s's join. IP_%s\n", name, clnt_ip);
    write(sock, myInfo, strlen(myInfo));

    while (true) {
        fgets(msg, BUF_SIZE, stdin);

        // menu_mode command -> !menu
        if (!strcmp(msg, "!menu\n")) {
            menuOptions();
            continue;
        } else if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }

        // send message
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return nullptr;
}

void* socket_client1::recv_msg(void* arg)
{
    int sock = *((int*)arg);
    char name_msg[NORMAL_SIZE + BUF_SIZE];
    int str_len;

    while (true) {
        str_len = read(sock, name_msg, NORMAL_SIZE + BUF_SIZE - 1);
        if (str_len == -1)
            return (void*)-1;
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return nullptr;
}

void socket_client1::menuOptions() {
    int select;
    // print menu
    cout << "\n\t**** menu mode ****\n";
    cout << "\t1. change name\n";
    cout << "\t2. clear/update\n\n";
    cout << "\tthe other key is cancel";
    cout << "\n\t*******************";
    cout << "\n\t>> ";
    cin >> select;
    getchar();
    switch (select) {
        // change user name
        case 1:
            changeName();
            break;
            // console update(time, clear chatting log)
        case 2:
            menu();
            break;
            // menu error
        default:
            cout << "\tcancel.";
            break;
    }
}

/** change user name **/
void socket_client1::changeName() {
    char nameTemp[100];
    cout << "\n\tInput new name -> ";
    cin >> nameTemp;
    sprintf(name, "[%s]", nameTemp);
    cout << "\n\tComplete.\n\n";
}

void socket_client1::menu() {
    system("clear");
    cout << " **** moon/sum chatting client ****\n";
    cout << " server port : " << serv_port << "\n";
    cout << " client IP   : " << clnt_ip << "\n";
    cout << " chat name   : " << name << "\n";
    cout << " server time : " << serv_time << "\n";
    cout << " ************* menu ***************\n";
    cout << " if you want to select menu -> !menu\n";
    cout << " 1. change name\n";
    cout << " 2. clear/update\n";
    cout << " **********************************\n";
    cout << " Exit -> q & Q\n\n";
}

void socket_client1::error_handling(const char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
bool socket_client1::client_first(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void* thread_return;

    if (argc != 4) {
        cout << "Usage : " << argv[0] << " <ip> <port> <name>\n";
        exit(1);
    }

    /** local time **/
    time_t timer = time(NULL);
    struct tm* t = localtime(&timer);
    sprintf(serv_time, "%d-%d-%d %d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);

    sprintf(name, "[%s]", argv[3]);
    sprintf(clnt_ip, "%s", argv[1]);
    sprintf(serv_port, "%s", argv[2]);
    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    /** call menu **/
    menu();

    pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(sock);
    return false;
}


