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

//  修改epoll中的事件
    bool ModFd(int fd, uint32_t events);

//  删除epoll中的事件
    bool DelFd(int fd);

//  epoll_wait()
    int Wait();

//  获取eventFd 用来判断新请求还是读写事件
    int GetEventFd(size_t i);

//    获取事件
    uint32_t GetEvents(int i);

private:
    int _epollFd;
//    用vector主要是考虑到动态扩容 数组不能动态
    std::vector<struct epoll_event> _events;
};

#endif //LINUX_EPOLL_H
