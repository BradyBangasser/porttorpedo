#include "natconnection.hpp"
#include "pubip.h"

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
  int sockopt = 1;

  if ((this->psock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    throw std::exception();
  }

  if (fcntl(this->psock, F_SETFL, O_NONBLOCK) == -1) {
    perror("fcntl");
    throw std::exception();
  }

  setsockopt(this->psock, SOL_SOCKET, SO_REUSEPORT, &sockopt, sizeof(sockopt));
  setsockopt(this->psock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));

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

  this->int_port = baddr.sin_port;
  this->nat_port = get_natport(baddr.sin_port);

  printf("Address: %s, Port: %d\n", connection::get_pubip_s().c_str(),
         nat_port);
}

int natconnection::hp_con_udp(const std::shared_ptr<linkcode> plink) {
  printf("Connecting from %s:%d:%d to %s\n", this->get_pubip_s().c_str(),
         ntohs(this->int_port), ntohs(this->nat_port),
         plink->to_string().c_str());
  static char buffer[128] = {0};
  struct sockaddr_in addr = {0}, iaddr = {0};
  struct pollfd pfd = {this->psock, POLLIN, 0};
  socklen_t al = sizeof(iaddr);
  int res = 0;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = plink->addr;
  addr.sin_port = plink->port;

  static constexpr std::string_view msg = "Hi";

  printf("Sending %d\n", ntohs(plink->port));

  do {
    sendto(this->psock, msg.data(), msg.size(), 0,
           (const struct sockaddr *)&addr, sizeof(addr));

    printf("Send\n");
    sleep(1);

    res = recvfrom(this->psock, (void *)buffer, sizeof(buffer), 0,
                   (struct sockaddr *)&iaddr, &al);
    perror("recvfrom");
  } while (res == -1 && errno == EWOULDBLOCK);

  printf("Receive %d bytes\n", res);

  return 0;
}
