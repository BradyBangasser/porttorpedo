#pragma once

#ifndef PT_LINK_CODE_HPP
#define PT_LINK_CODE_HPP

#include <arpa/inet.h>
#include <exception>
#include <inttypes.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>

#include <format>
#include <string>

namespace pt {

typedef struct linkcode {
  linkcode(const char *ip, uint16_t port) : port(htons(port)) {
    struct in_addr addr = {0};
    if (inet_aton(ip, &addr) == 0) {
      perror("linkcode");
      throw std::exception();
    }

    this->addr = addr.s_addr;
  }

  std::string to_string() {
    struct in_addr a = {.s_addr = this->addr};
    return std::format("{}:{}", inet_ntoa(a), ntohs(this->port));
  }
  // IP address in network order
  uint32_t addr;
  // Port in network order
  uint16_t port;
  uint8_t pad : 2;
  uint32_t check : 20;
} linkcode;

} // namespace pt

#endif
