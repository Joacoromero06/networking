#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <signal.h>


#define BUF_SIZE 4096
#define PORT 80 // google port
#define SA struct sockaddr 

void err_n_die(const char* format, ...);
int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr;

  char recvbuf[BUF_SIZE];
  int recvbytes;

  char sendbuf[BUF_SIZE];
  int sendbytes;

  if (argc != 2) {
    err_n_die("error: usage %s <server-address>", argv[0]);
  }
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    err_n_die("error while creating socket");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  
  switch (inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) 
  {
  case 0: err_n_die("<servaddr> (%s) does not contain a valid network address", argv[1]);
  case -1: err_n_die("address family AF_INET is not working");
  }

  if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0)
    err_n_die("conection failed: look at man connect for errors");

  sprintf(sendbuf, "GET / /HTTP/1.1\r\n"
                   "Host: %s\r\n"
                   "Connection: close\r\n"
                   "\r\n",
                   argv[1]);
  sendbytes = strlen(sendbuf);

  if (write(sockfd, sendbuf, sendbytes) == -1)
    err_n_die("write failed: look at posix man write");
  
  memset(recvbuf, 0, BUF_SIZE);
  
  while ( (recvbytes = read(sockfd, recvbuf, BUF_SIZE)) > 0)
  {
    printf("%s", recvbuf);
    memset(recvbuf, 0, BUF_SIZE);
  }
  if (recvbytes < 0)
    err_n_die("read failed: look at posix man read");

  exit(0);
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
