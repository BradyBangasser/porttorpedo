#pragma once

#ifndef PT_NATCONNECTION_HPP
#define PT_NATCONNECTION_HPP

#include "connection.hpp"

class natconnection : pt::network::mgr::connection {
private:
  // client peer link
  SOCKET psock = -1;
  // server peer link
  SOCKET ssock = -1;
  const std::shared_ptr<pt::linkcode> rslink;
  uint64_t slink_iter = 0;
  uint64_t rslink_iter = 0;

  uint16_t nat_port;
  uint16_t int_port;

public:
  natconnection();
  int hp_con_udp(const std::shared_ptr<pt::linkcode> plink);
};

#endif
