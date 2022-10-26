#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

int main(int argc, char **argv) {
  char str[501];
  for (int i = 0; i < 500; i++)
    str[i] = 'a';
  str[500] = 0;
  for (int i = 0; i < 1000000; i++) {
    write(1, str, 500);
    char buf[501] = {0};
    read(0, buf, 500);
  }
  write(2, "P1\n", 3);
}
