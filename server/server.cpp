//
// Created by wrn on 4/13/23.
//


#include "server.h"

Server::Server(int port, bool OptLinger) {
    if (!InitSocket()) { _isClose = true; }

}

Server::~Server() {
    close(_listenFd);
    _isClose = true;
}

//启动
void Server::Start() {
    if (!_isClose) printf("==================Start==========");
    int eventCount = _epoll->Wait();
    for (int i = 0; i < eventCount; i++) {
        int fd = _epoll->GetEventFd(i);
        uint32_t events = _epoll->GetEvents(i);
        if (fd == _listenFd) {
            _Listen();
        } else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
            _CloseConnection();
        } else if (events | EPOLLIN) {
            _Read();
        } else if (events | EPOLLOUT) {
            _Write();
        } else {
            perror("error!");
        }
    }
}

//创建监听FD
bool Server::InitSocket() {
    int ret;
    struct sockaddr_in addr;
    if (_port > 65535 || _port < 1024) {
        perror("port error");
        return false;
    }
//    初始化地址信息
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

//    创建监听fd
    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd < 0) {
        perror("socket failed");
        return false;
    }
//    设置优雅关闭
    struct linger optlinger;
    if (_isLinger) {
        optlinger.l_onoff = 1;
        optlinger.l_linger = 1;
    }
    ret = setsockopt(_listenFd, SOL_SOCKET, SO_LINGER, &optlinger, sizeof optlinger);
    if (ret == -1) {
        perror("linger failed");
        close(_listenFd);
        return false;
    }
//    绑定与监听
    ret = bind(_listenFd, (struct sockaddr *) &addr, sizeof addr);
    if (ret < 0) {
        perror("bind error");
        close(_listenFd);
        return false;
    }
    ret = listen(_listenFd, 6);
    if (ret < 0) {
        perror("listen failed");
        close(_listenFd);
        return false;
    }
//    设置端口复用
    int optval = 1;
    ret = setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    if (ret < 0) {
        perror("reuse error");
        close(_listenFd);
        return false;
    }
//    将_listenFd 加入epoll中
    ret = _epoll->AddFd(_listenFd, EPOLLIN | EPOLLHUP);
    if (ret == 0) {
        perror("add listen error");
        close(_listenFd);
        return false;
    }
//    设置非阻塞 以便accept时不阻塞
    fcntl(_listenFd, F_SETFL, fcntl(_listenFd, F_GETFD, 0) | O_NONBLOCK);
    return true;
}

void Server::AddClientFdToEpoll(int fd, sockaddr_in addr) {
    _epoll->AddFd(fd, EPOLLIN | _connEvent);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

void Server::_Listen() {
    struct sockaddr_in addr;
    socklen_t len = sizeof addr;
    do {
        int fd = accept(_listenFd, (struct sockaddr *) &addr, &len);
        AddClientFdToEpoll(fd, addr);
    } while (_listenEvent & EPOLLET);
}

void Server::_Read() {

}

void Server::_Write() {

}

void Server::_CloseConnection() {
    _epoll->DelFd()

}