#pragma once

#ifndef PT_LANCONNECTION_HPP
#define PT_LANCONNECTION_HPP

#include <memory>

#include "connection.hpp"
#include "linkcode.hpp"
#include "types.h"

class lanconnection : connection {
private:
  static std::shared_ptr<lanconnection> handshake(SOCKET sock,
                                                  std::unique_ptr<linkcode>);
};

#endif
