#pragma once

#ifndef PT_NATCONNECTION_HPP
#define PT_NATCONNECTION_HPP

#include "connection.hpp"

class natconnection : connection {
private:
  // client peer link
  SOCKET psock = -1;
  // server peer link
  SOCKET ssock = -1;
  const std::shared_ptr<linkcode> rslink;
  uint64_t slink_iter = 0;
  uint64_t rslink_iter = 0;

public:
  natconnection();
  int connect(const std::shared_ptr<linkcode> clink);
  virtual int accept(const std::shared_ptr<linkcode> clink);
};

#endif
