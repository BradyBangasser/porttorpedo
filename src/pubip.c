#include <arpa/inet.h>
#include <asm-generic/errno.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "types.h"

#warning "TODO: make this portable"

const static char *req =
    "GET / HTTP/1.1\r\nHost: checkip.amazonaws.com\r\nUser-Agent: "
    "curl/8.11.1\r\nAccept: */*\r\n\r\n";
static size_t reqlen = 0;

static char buffer[256] = {0};

uint32_t get_pubip() {
  int sock = 0;
  struct in_addr addr;
  struct addrinfo hints = {0}, *res = NULL, *curs = NULL;

  if (!reqlen)
    reqlen = strlen(req);

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;

  if (getaddrinfo("checkip.amazonaws.com", "80", &hints, &res) != 0) {
    return 0;
  }

  curs = res;

  while (curs != NULL) {
    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) ==
        -1) {
      continue;
    }

    if (connect(sock, curs->ai_addr, curs->ai_addrlen) == 0) {
      break;
    }

    close(sock);
    curs = curs->ai_next;
  }

  if (curs == NULL) {
    freeaddrinfo(res);
    return 0;
  }

  if (write(sock, req, reqlen) <= 0) {
    goto error;
  }

  if (recv(sock, buffer, sizeof(buffer), 0) <= 0) {
    goto error;
  }

  if (inet_aton(strstr(buffer, "\r\n\r\n") + 4, &addr) == 0) {
    goto error;
  }

  freeaddrinfo(res);
  close(sock);

  return addr.s_addr;

error:
  freeaddrinfo(res);
  close(sock);
  return 0;
}

// lport is in Network endian
uint16_t get_natport(uint16_t lport) {
  SOCKET sock = -1;
  int optval = 1;
  uint8_t req[20] = {0};
  uint8_t buf[128] = {0};

  struct in_addr addr = {0};
  struct sockaddr_in baddr = {0}, oaddr = {0};
  socklen_t oaddr_l = sizeof(oaddr);
  struct addrinfo hints = {0}, *res = NULL, *curs = NULL;

  *((short *)req) = htons(0x0001);
  *((short *)(req + 2)) = htons(0x0000);
  *((int *)(req + 4)) = htonl(0x2112A442);

  *((int *)(req + 8)) = htonl(0x2112A442);
  *((int *)(req + 12)) = htonl(0x2112A442);
  *((int *)(req + 16)) = htonl(0x2112A442);

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    return 0;
  }

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
    perror("fcntl");
    close(sock);
    return 0;
  }

  baddr.sin_addr.s_addr = INADDR_ANY;
  baddr.sin_port = lport;

  if (bind(sock, (const struct sockaddr *)&baddr, sizeof(baddr)) != 0) {
    perror("Bind");
    close(sock);
    return 0;
  }

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = 0;

  if (getaddrinfo("stun.l.google.com", "19302", &hints, &res) != 0) {
    return 0;
  }

  curs = res;

  while (curs != NULL) {
    sendto(sock, req, sizeof(req), 0, curs->ai_addr, curs->ai_addrlen);

    sleep(1);

    oaddr_l = sizeof(oaddr);
    bzero(&oaddr, oaddr_l);
    if (recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&oaddr,
                 &oaddr_l) != -1) {
      break;
    } else if (errno == EWOULDBLOCK)
      continue;
    perror("recvfrom");
    curs = curs->ai_next;
  }

  if (curs == NULL) {
    goto error;
  }

  if (ntohs(*(uint16_t *)buf) != 0x0101) {
    goto error;
  }

  freeaddrinfo(res);
  close(sock);

  // Just in case I want the IP addr
  *((int *)(buf + 28)) ^= htonl(0x2112A442);
  *((short *)(buf + 26)) ^= htons(0x2112A442 >> 16);

  return *(buf + 26);

error:
  freeaddrinfo(res);
  close(sock);
  return 0;
}
