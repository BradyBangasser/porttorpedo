#include <arpa/inet.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
