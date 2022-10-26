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

  for (int i = 0; i < 1000000; i++) {
    char buf[501];
    read(0, buf, 500);
    write(1, str, 500);
  }
  write(2, "P2\n", 3);
}
