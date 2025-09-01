#include "natconnection.hpp"

#include <cerrno>
#include <stdio.h>

#include <fcntl.h>
#include <poll.h>

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
  socklen_t l = sizeof(baddr);

  if ((this->psock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    throw std::exception();
  }

  if (fcntl(this->psock, F_SETFL, O_NONBLOCK) == -1) {
    perror("fcntl");
    throw std::exception();
  }

  baddr.sin_addr.s_addr = INADDR_ANY;
  baddr.sin_port = 0;
  baddr.sin_family = AF_INET;

  if (bind(this->psock, (const struct sockaddr *)&baddr, sizeof(baddr)) == -1) {
    perror("bind");
    throw std::exception();
  }

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
  struct pollfd pfd = {this->psock, 0x0 | POLLOUT, 0x0};
  nfds_t nfds = 1;
  int err = 0, cerr = -1, serr;
  socklen_t serrl = sizeof(serr);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = plink->addr;
  addr.sin_port = plink->port;

  while (cerr == -1) {
    printf("Attempted connect\n");
    cerr = ::connect(this->psock, (const struct sockaddr *)&addr, sizeof(addr));

    if (cerr == 0)
      break;

    perror("fail");

    err = poll(&pfd, nfds, 5 * 1000);
    if (err == -1) {
      perror("POLL");
      return 2;
    } else if (err > 0) {
      // We are ready to read/write

      getsockopt(this->psock, SOL_SOCKET, SO_ERROR, &serr, &serrl);
      assert(pfd.revents & POLLOUT);
      break;
    }
  }

  printf("Connected %ld %d\n", write(this->psock, "hi", 3), cerr);
  perror("WRITE");
  return 1;
}

int natconnection::hp_con_udp(const std::shared_ptr<linkcode> plink) {
  static char buffer[128] = {0};
  struct sockaddr_in addr = {0}, iaddr = {0};
  socklen_t al = sizeof(iaddr);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = plink->addr;
  addr.sin_port = plink->port;

  static constexpr std::string_view msg = "Hi";

  printf("Sending %d\n", ntohs(plink->port));

  do {
    sendto(this->psock, msg.data(), msg.size(), 0,
           (const struct sockaddr *)&addr, sizeof(addr));

    recvfrom(this->psock, (void *)buffer, sizeof(buffer), 0,
             (struct sockaddr *)&iaddr, &al);
  } while ();

  printf("Sent\n");

  ;
}
