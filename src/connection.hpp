#pragma once

#ifndef PT_CONNECTION_HPP
#define PT_CONNECTION_HPP

#include <arpa/inet.h>
#include <netinet/in.h>

#include <memory>
#include <string>

#include "linkcode.hpp"
#include "types.h"

class connection {
protected:
  SOCKET sock = -1;
  const std::shared_ptr<linkcode> slink;

  static uint32_t pubip;

  connection();

public:
  inline const std::shared_ptr<linkcode> get_slink() const {
    return this->slink;
  }
  static uint32_t get_pubip();
  static const inline std::string get_pubip_s() {
    struct in_addr addr = {connection::get_pubip()};
    return std::string(inet_ntoa(addr));
  }
};

#endif
