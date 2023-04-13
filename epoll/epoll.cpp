//
// Created by wrn on 4/13/23.
//

#include "epoll.h"

Epoll::Epoll(int maxEvent) : _epollFd(epoll_create(512)), _events(maxEvent) {
//    _epollFd = epoll_create(512);
//    _events.resize(maxEvent);
    assert(_epollFd >= 0 && _events.size() > 0);
}

Epoll::~Epoll() {
    close(_epollFd);
}

bool Epoll::AddFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    return 0 == epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
}