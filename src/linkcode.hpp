#pragma once

#include <exception>
#ifndef PT_LINK_CODE_HPP
#define PT_LINK_CODE_HPP

#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <netinet/ip.h>

typedef struct linkcode {
  linkcode(const char *ip, uint16_t port) : port(htons(port)) {
    struct in_addr addr = {0};
    if (inet_aton(ip, &addr) == 0) {
      throw std::exception();
    }

    this->addr = addr.s_addr;
  }
  // IP address in network order
  uint32_t addr;
  // Port in network order
  uint16_t port;
  uint8_t pad : 2;
  uint32_t check : 20;
} linkcode;

#endif
