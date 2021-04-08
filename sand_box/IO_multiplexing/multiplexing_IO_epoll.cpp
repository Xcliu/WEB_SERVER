/*
 * the sysytem call epoll issepcified for linux
 */

/*
 *used for epoll related system call:
 *epoll_create; epoll_ctl; epoll_wait
 */
#if __linux__
#include <sys/epoll.h>
#elif __APPLE__
#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

/*
used for macro system_error_info
*/
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#define systemcall_error_info()                                     \
  fprintf(stderr, "*** ERROR %s, failed:%d(%s)\n", __func__, errno, \
          strerror(errno));
/*
used for exit
*/
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int epoll_fd = epoll_create(5);
  if (epoll_fd < 0) {
    systemcall_error_info();
    exit(EXIT_FAILURE);
  }
  printf("epoll_fd=%d\n", epoll_fd);

  epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = 1;  // stdin,stdout,stderr -->fd:0,1,2
  int temp = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 2, &ev);
  if (temp < 0) {
    systemcall_error_info();
    exit(EXIT_FAILURE);
  }

  epoll_event ev_result[5];
  temp = epoll_wait(epoll_fd, ev_result, 5, 1000);
  if (temp < 0) {
    systemcall_error_info();
    exit(EXIT_FAILURE);
  } else {
    printf("%d fd are ready\n", temp);
  }
  return 0;
}