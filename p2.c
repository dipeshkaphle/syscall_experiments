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

  char str[501];
  for (int i = 0; i < 500; i++)
    str[i] = 'a';
  str[500] = 0;

  int epoll_fd = epoll_create1(0);

  ev.events = EPOLLIN | EPOLLHUP;
  ev.data.fd = efd1;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, efd1, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 100000; i++) {
    sprintf(numbuf, "%d", i);
    // write(2, numbuf,strlen(numbuf));
    eventfd_t val = 0;

    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    if (events[0].events & EPOLLIN) {
      eventfd_read(efd1, &val);
    }
    if (events[0].events & EPOLLHUP) {
      write(2, "P2 has died", strlen("P2 has died"));
      exit(1);
    }
    /* eventfd_read(efd1, &val); */
    char buf[501];
    read(0, buf, 500);
    write(1, str, 500);
    eventfd_write(efd2, 1);
  }
  write(2, "P2\n", 3);
}
