#include "lanconnection.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace pt::network;

std::optional<std::thread> lanconnection::con_w = std::nullopt;
SOCKET lanconnection::hsock = -1;

int lanconnection::start() {
  struct sockaddr_in addr;

  bzero(&addr, sizeof(addr));

  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(lanconnection::port);
  addr.sin_family = AF_INET;

  if (lanconnection::con_w == std::nullopt) {
    if ((lanconnection::hsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      // Error event
      return 2;
    }

    if (fcntl(lanconnection::hsock, F_SETFL,
              fcntl(lanconnection::hsock, F_GETFL, 0) | O_NONBLOCK) == -1) {
      // add Error
      return 3;
    }

    if (::bind(lanconnection::hsock, (const struct sockaddr *)&addr,
               sizeof(addr)) == -1) {
      // Add error
      return 4;
    }

    if (::listen(lanconnection::hsock, 5) == -1) {
      // Add error
      return 5;
    }

    // Add try catch and event log
    lanconnection::con_w = std::thread(&lanconnection::con_handler);
  } else {
    // Add warning event here
    return 1;
  }

  return 0;
}

int lanconnection::con_handler() {
  SOCKET icon = -1;
  struct pollfd pfd;
  struct sockaddr_in addr;
  socklen_t addr_l = sizeof(addr);

  std::string_view buffer = "Hello!";

  bzero(&addr, sizeof(addr));

  if (lanconnection::hsock == -1)
    return 1;

  pfd.fd = lanconnection::hsock;
  pfd.events = POLLIN;

  while (1) {
    switch (poll(&pfd, 1, 1000)) {
    case -1: {
      return 6;
    }
    case 0: {
      continue;
    }
    }

    if ((icon = accept(lanconnection::hsock, (struct sockaddr *)&addr,
                       &addr_l)) == -1) {
      return 7;
    }

    ::write(icon, buffer.data(), buffer.size());
    return 0;
  }

  return 0;
}

lanconnection::lanconnection(pt::linkcode lc) {
  struct sockaddr_in addr = {0};
  socklen_t addr_l = sizeof(addr);

  char buffer[32] = {0};

  if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    throw 1;
  }

  if (connect(this->sock, (const struct sockaddr *)&addr, addr_l) == -1) {
    throw 2;
  }

  recv(this->sock, buffer, sizeof(buffer), 0);

  printf("%s\n", buffer);
}
