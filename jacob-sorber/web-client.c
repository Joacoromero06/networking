#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include <signal.h>


#define // maxline
#define PORT 8080
#define SA struct sockaddr 

void err_n_die(const char* format, ...);
int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2) {
    err_n_die("error: usage %s <server-address>", argv[0]);
  }
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0 )
    err_n_die("error while creating socket");

  bzero(&servaddr, sizeof(servaddr));

  return 0;
}

void err_n_die(const char* format, ...)
{
  int errno_save;
  va_list va;

  errno_save = errno;

  va_start(va, format);
  vfprintf(stdout, format, va);
  fprintf(stdout, "\n");
  fflush(stdout);

  if (errno_save != 0) {
    fprintf(stdout, "errno=%d: %s", errno_save, strerror(errno_save));
    fprintf(stdout, "\n");
    fflush(stdout);
  }
  va_end(va);
  exit(1);
}
