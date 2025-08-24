#pragma once

#ifndef PT_CONNECTION_HPP
#define PT_CONNECTION_HPP

#include <memory>

#include "link_code.h"
#include "types.h"

class connection {
protected:
  SOCKET sock = -1;
  const std::shared_ptr<linkcode> slink;

  connection();

public:
  inline const std::shared_ptr<linkcode> get_slink() const {
    return this->slink;
  }
  virtual int connect(const std::shared_ptr<linkcode> clink) = 0;
  virtual int accept(const std::shared_ptr<linkcode> clink) = 0;
};

#endif
