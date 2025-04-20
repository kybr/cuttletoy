// This code is to prove to myself that we can send large UDP packets
// I know that these large packets will be fragmented by the kernel.
// But, still. They are useful.
//
//

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  int sockfd;
  struct sockaddr_in dest;
  char msg[100000];

  for (int i = 0; i < sizeof(msg); i++) {
    msg[i] = 'o';
  }

  // 1. Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  int send_buf_size = 0;
  {
    // Optional: verify what the kernel actually set
    socklen_t optlen = sizeof(send_buf_size);
    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &optlen) ==
        0) {
      printf("SO_SNDBUF is %d bytes\n", send_buf_size);
    }
  }

  // 2. Fill in destination address (127.0.0.1:12345)
  memset(&dest, 0, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(9999);
  if (inet_pton(AF_INET, "127.0.0.1", &dest.sin_addr) != 1) {
    perror("inet_pton()");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  for (int size = 1; size < 100000; size += 1) {
    ssize_t n = sendto(sockfd, msg, strnlen(msg, size), 0,
                       (struct sockaddr *)&dest, sizeof(dest));
    if (n < 0) {
      printf("Maximum size UDP packet is %d\n", size - 1);
      perror("sendto()");
      break;
    }
  }

  send_buf_size = 100000;
  {
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buf_size,
                   sizeof(send_buf_size)) < 0) {
      perror("setsockopt SO_SNDBUF");
    } else {
      // Optional: verify what the kernel actually set
      socklen_t optlen = sizeof(send_buf_size);
      if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &optlen) ==
          0) {
        printf("SO_SNDBUF is now %d bytes\n", send_buf_size);
      }
    }
  }

  for (int size = 1; size < 100000; size += 1) {
    ssize_t n = sendto(sockfd, msg, strnlen(msg, size), 0,
                       (struct sockaddr *)&dest, sizeof(dest));
    if (n < 0) {
      printf("Maximum size UDP packet is %d\n", size - 1);
      perror("sendto()");
      break;
    }
  }

  close(sockfd);
  return 0;
}
