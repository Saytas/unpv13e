#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buff[MAXLINE];
  time_t ticks;
  
  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY allows the server to accept a client connection on any interface, in case the server host has multiple interfaces.
  servaddr.sin_port = htons(13); /* daytime server */
  
  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr)); // SA * == sockaddr_in * == void *
  
  Listen(listenfd, LISTENQ); // constant LISTENQ is from unp.h header, and it specifies the maximum number of client connections that the kernel will queue for this listening descriptor.
  
  for (;;) {
    connfd = Accept(listenfd, (SA *) NULL, NULL);
      
    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    Write(connfd, buff, strlen(buff));

    Close(connfd);
  }
}
