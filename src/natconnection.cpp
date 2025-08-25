#include "natconnection.hpp"

#include <stdio.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <unistd.h>

#include <assert.h>
#include <string.h>

#include <string_view>

static constexpr std::string_view hello = "Hello!!!!!";

natconnection::natconnection() {
  struct sockaddr_in baddr = {0};
  struct in_addr lbaddr = {0};
  socklen_t l = sizeof(baddr);

  if ((this->psock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    throw std::exception();
  }

  if (bind(this->psock, (const struct sockaddr *)&baddr, sizeof(baddr)) == -1) {
    perror("bind");
    throw std::exception();
  }

  baddr.sin_addr.s_addr = INADDR_ANY;
  baddr.sin_port = 0;
  baddr.sin_family = AF_INET;

  if ((getsockname(this->psock, (struct sockaddr *)&baddr, &l)) != 0) {
    perror("getsockname");
    throw std::exception();
  }

  printf("Address: %s, Port: %d\n", connection::get_pubip_s().c_str(),
         ntohs(baddr.sin_port));
}

int natconnection::accept(const std::shared_ptr<linkcode> clink) {
  socklen_t l = 0;
  sockaddr_in addr = {0};

  addr.sin_addr.s_addr = clink->addr;
  addr.sin_port = clink->port;
  addr.sin_family = AF_INET;

  if (::connect(this->sock, (const struct sockaddr *)&addr, sizeof(addr)) ==
      -1) {
  }

  memset(&addr, 0x0, sizeof(addr));

  if (listen(this->sock, 5) == -1) {
    perror("listen");
    return 3;
  }

  while ((this->ssock = ::accept(this->sock, (struct sockaddr *)&addr, &l)) !=
             -1 &&
         addr.sin_addr.s_addr != clink->addr && close(this->ssock) == 0) {
    printf("Something\n");
  }

  assert(send(this->ssock, hello.data(), hello.size(), 0) == hello.size());
  return 1;
}

int natconnection::connect(const std::shared_ptr<linkcode> plink) {
  static uint8_t buffer[32] = {0};
  struct sockaddr_in addr = {0};

  addr.sin_addr.s_addr = plink->addr;
  addr.sin_port = plink->port;
  addr.sin_family = AF_INET;

  if (::connect(this->psock, (const struct sockaddr *)&addr, sizeof(addr)) ==
      -1) {
    return 4;
  }

  recv(this->psock, (void *)buffer, 32, 0);

  printf("recv: %s\n", buffer);

  return 1;
}

int natconnection::hp_con(const std::shared_ptr<linkcode> plink) {
  struct sockaddr_in addr = {0};

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = plink->addr;
  addr.sin_port = plink->port;

  if (::connect(this->psock, (const struct sockaddr *)&addr, sizeof(addr)) !=
      0) {
    perror("Connect");
    return 5;
  }
  return 1;
}
