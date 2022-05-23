#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define PROT_DEFAULT 0       // see /etc/protocol
#define BIND_PORT 32452  //

#define MAX_PATH_LENGTH 104

char unix_sock_path[MAX_PATH_LENGTH];
char unix_server_path[MAX_PATH_LENGTH];
char data_buffer[100000];

int main(void) {
  sprintf(unix_sock_path, "%s/usock_cli", getcwd(NULL, MAX_PATH_LENGTH));
  sprintf(unix_server_path, "%s/usock", getcwd(NULL, MAX_PATH_LENGTH));
  // 사용한 unix소켓은 사용이 끝나도 자동으로 unlink 되지 않는다.
  unlink(unix_sock_path);
  printf("%s\n", unix_sock_path);

  int sock = socket(PF_LOCAL, SOCK_STREAM, PROT_DEFAULT);
  if (sock == -1)
    err(EXIT_FAILURE, "socket: ");
  
  struct sockaddr_un addr;

  addr.sun_family = AF_UNIX;
  strlcpy(addr.sun_path, unix_server_path, MAX_PATH_LENGTH);
  addr.sun_len = strlen(unix_server_path) + 1;

  if (connect(sock, &addr, sizeof(addr)) == -1)
    err(EXIT_FAILURE, "connect: ");
  printf("Connected!\n");
  ssize_t send_length = send(sock, "ABCD", 4, 0);
  if (send_length == -1)
    err(EXIT_FAILURE, "send: ");
  printf("Sent!\n");
  ssize_t recv_length = recv(sock, data_buffer, 4, 0);
  if (recv_length == -1)
    err(EXIT_FAILURE, "recv: ");
  printf("=%4s=\n", data_buffer);
}