#include "jerror.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 80
#define MAX_CONNECTIONS 10
#define BUF_SIZE 4096
#define SA struct sockaddr

int end_http_request(char* request, size_t size);

int main(int argc, char* argv[])
{
  int listenfd;
  struct sockaddr_in servaddr;

  char recvbuf[BUF_SIZE];
  char sendbuf[BUF_SIZE];

  int n;

  if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("socket resource creation failed:");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVER_PORT);

  if (bind(listenfd, (SA*) &servaddr, sizeof(servaddr)) < 0)
    err_n_die("bind failed:");
  if (listen(listenfd, MAX_CONNECTIONS) < 0)
    err_n_die("listen failed:");
  
  for (;;)
  {
    int connfd;
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len = sizeof(peeraddr);
    
    fprintf(stdout, "waiting for a connection in port %d\n", SERVER_PORT);
    fflush(stdout);
    connfd = accept(listenfd, (SA*) &peeraddr, &peeraddr_len);

    memset(recvbuf, 0, BUF_SIZE);
    while ( (n=read(connfd, recvbuf, BUF_SIZE)) > 0 )
    {
      fprintf(stdout, "\n%s\n\n%s", bin2hex(recvbuf, n), recvbuf);
      if (end_http_request(recvbuf, n))
        break;
      memset(recvbuf, 0, BUF_SIZE);
    }
    if (n < 0)
      err_n_die("reading failed:");

    snprintf(sendbuf, BUF_SIZE, "HTTP/1.0 200 OK\r\n\r\nHello");
    if (write(connfd, sendbuf, strlen(sendbuf)) < 0)
      err_n_die("write failed:");

    if (close(connfd) < 0)
      err_n_die("close failed:");
  }
}
int end_http_request(char* request, size_t size) {
  if (request[size-1] == '\n' && 
      request[size-2] == '\r' && 
      request[size-3] == '\n' && 
      request[size-4] == '\r')
    return 1;
  return 0;
}

