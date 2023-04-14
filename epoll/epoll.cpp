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

bool Epoll::ModFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event event;
    event.data.fd = fd;
    event.events = events;
    return 0 == epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
}

bool Epoll::DelFd(int fd) {
    if (fd < 0) return false;
    epoll_event event = {0};
    return 0 == epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, &event);
}

int Epoll::Wait() {
    return epoll_wait(_epollFd, &_events[0], sizeof _events, -1);
}

int Epoll::GetEventFd(size_t i) {
    assert(i < _events.size() && i >= 0);
    return _events[i].data.fd;
}

uint32_t Epoll::GetEvents(int i) {
    assert(i < _events.size() && i >= 0);
    return _events[i].events;
}
