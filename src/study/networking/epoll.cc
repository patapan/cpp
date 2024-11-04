#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

bool is_fd_ready_for_reading(int fd) {
    int epoll_fd = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    struct epoll_event events[1];

    int ret = epoll_wait(epoll_fd, events, 1, 0);  // 0 ms timeout for non-blocking
    close(epoll_fd);

    return (ret > 0) && (events[0].events & EPOLLIN);
}

bool is_fd_ready_for_writing(int fd) {
    int epoll_fd = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLOUT;
    ev.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    struct epoll_event events[1];

    int ret = epoll_wait(epoll_fd, events, 1, 0);  // 0 ms timeout for non-blocking
    close(epoll_fd);

    return (ret > 0) && (events[0].events & EPOLLOUT);
}
