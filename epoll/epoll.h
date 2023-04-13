//
// Created by wrn on 4/13/23.
//

#ifndef LINUX_EPOLL_H
#define LINUX_EPOLL_H

#include <vector>
#include <sys/epoll.h> //epoll_ctl()
#include <unistd.h> // close()
#include "assert.h"

class Epoll {
public:
    Epoll(int maxEvent);

    ~Epoll();

//  将fd添加进epoll 并设立监听事件
    bool AddFd(int fd, uint32_t events);

private:
    int _epollFd;
//    用vector主要是考虑到动态扩容 数组不能动态
    std::vector<struct epoll_event> _events;
};

#endif //LINUX_EPOLL_H
