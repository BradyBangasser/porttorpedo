#pragma once

#ifndef PT_NATCONNECTION_HPP
#define PT_NATCONNECTION_HPP

#include "connection.hpp"

class natconnection : connection {
private:
  const std::shared_ptr<linkcode> rslink;
  uint64_t slink_iter = 0;
  uint64_t rslink_iter = 0;

public:
  virtual int connect(const std::shared_ptr<linkcode> clink);
  virtual int accept(const std::shared_ptr<linkcode> clink);
};

#endif
