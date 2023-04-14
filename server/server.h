//
// Created by wrn on 4/13/23.
//

#ifndef LINUX_SERVER_H
#define LINUX_SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include "arpa/inet.h"
#include <unistd.h>
#include <memory>
#include "epoll/epoll.h"
#include <fcntl.h>
#include "epoll/epoll.h"

class Server {
public:
    Server(int port, bool OptLinger);

    ~Server();

    void Start();

private:
    bool InitSocket();

    bool _isClose;

    unsigned int _port;

    bool _isLinger;

    int _listenFd;

    std::unique_ptr<Epoll> _epoll;

    uint32_t _listenEvent;

    uint32_t _connEvent;

//  处理accept
    void _Listen();

    void AddClientFdToEpoll(int fd, sockaddr_in addr);

    void _CloseConnection();

    void _Write();

    void _Read();
};

#endif //LINUX_SERVER_H
