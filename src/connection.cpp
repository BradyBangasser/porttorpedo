#include "connection.hpp"
#include <assert.h>
#include <exception>
#include <string.h>
#include <sys/socket.h>

#include "pubip.h"

#include <stdio.h>

uint32_t connection::pubip = 0;

connection::connection() {
  struct sockaddr_in baddr = {0};
  struct in_addr lbaddr = {0};
  socklen_t l = sizeof(baddr);

  if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    throw std::exception();
  }

  if (inet_aton("127.0.0.1", &lbaddr) == 0) {
    // Not possible
    throw std::exception();
  }

  baddr.sin_addr = lbaddr;
  baddr.sin_port = htons(69);
  baddr.sin_family = AF_INET;

  if (::connect(this->sock, (const struct sockaddr *)&baddr, sizeof(baddr)) !=
      -1) {
    shutdown(this->sock, SHUT_RDWR);
  }

  memset((void *)&baddr, 0x0, sizeof(baddr));

  if ((getsockname(this->sock, (struct sockaddr *)&baddr, &l)) != 0) {
    perror("getsockname");
    throw std::exception();
  }

  /* printf("Address: %s, Port: %d\n", connection::get_pubip_s().c_str(),
         ntohs(baddr.sin_port)); */

  baddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(this->sock, (const struct sockaddr *)&baddr, sizeof(baddr)) == -1) {
    perror("bind");
    throw std::exception();
  }
}

uint32_t connection::get_pubip() {
  if (connection::pubip)
    return connection::pubip;

  connection::pubip = ::get_pubip();
  assert(connection::pubip);
  return connection::pubip;
}
