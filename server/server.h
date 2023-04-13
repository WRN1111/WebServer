//
// Created by wrn on 4/13/23.
//

#ifndef LINUX_SERVER_H
#define LINUX_SERVER_H

#include <netinet/in.h>
#include <cstdio>
#include "arpa/inet.h"
#include <unistd.h>

class Server {
public:
    Server(int port, bool OptLinger);

    ~Server();

private:
    bool InitSocket();

    bool _isClose;

    unsigned int _port;

    bool _isLinger;

    unsigned int _listenFd;


};

#endif //LINUX_SERVER_H
