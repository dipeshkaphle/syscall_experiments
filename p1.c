#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#define MAX_EVENTS 1

struct epoll_event ev, events[MAX_EVENTS];
int listen_sock, conn_sock, nfds, epollfd;

int main(int argc, char **argv) {
  char numbuf[10];
  int efd1 = atoi(argv[1]);
  int efd2 = atoi(argv[2]);
  int epoll_fd = epoll_create1(0);

  ev.events = EPOLLIN | EPOLLHUP;
  ev.data.fd = efd2;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, efd2, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < 100000; i++) {
    sprintf(numbuf, "%d", i);
    // write(2, numbuf,strlen(numbuf));
    write(1, "ab", 2);
    eventfd_write(efd1, 1);
    eventfd_t val = 0;

    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    if (events[0].events & EPOLLIN) {
      eventfd_read(efd2, &val);
    }
    if (events[0].events & EPOLLHUP) {
      write(2, "P1 has died", strlen("P1 has died"));
      exit(1);
    }
    char buf[100];
    read(0, buf, 10);
  }
  write(2, "P1\n", 3);
}
