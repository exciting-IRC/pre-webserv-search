#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// #include <sys/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <err.h>

// skip dns query
#define CERN_IP "188.184.21.108"
#define PORT 80

#define REQUEST (                                   \
  "GET /hypertext/WWW/TheProject.html HTTP/1.1\r\n" \
  "Host: info.cern.ch\r\n"                          \
  "User-Agent: webserv\r\n"                         \
  "Accept: */*\r\n\r\n"                             \
)                                                   \

char buf[4096];

int main(void) {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(CERN_IP);
  addr.sin_port = htons(PORT);

  int sock = socket(PF_INET, SOCK_STREAM, 0 /* default protocol */ );
  if (sock == -1)
    err(EXIT_FAILURE, "socket");
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    err(EXIT_FAILURE, "connect");
  ssize_t len = send(sock, REQUEST, strlen(REQUEST), 0);
  printf("send [%zd] byte\n", len);
  if (len == -1)
    err(EXIT_FAILURE, "send");
  len = recv(sock, buf, 4096, 0);
  if (len == -1)
    err(EXIT_FAILURE, "recv");
  printf("%s", buf);
  close(sock);
  return 0;
}