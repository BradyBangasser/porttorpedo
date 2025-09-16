#pragma once

#ifndef PT_LANCONNECTION_HPP
#define PT_LANCONNECTION_HPP

#ifndef PT_ENUM
#define PT_ENUM(x) x,
#endif

#ifndef PT_ESTR
#define PT_ESTR(x) #x,
#endif

#ifdef PT_LAN_CON_TYPE_ENUM
#undef PT_LAN_CON_TYPE_ENUM
#endif

#define PT_LAN_CON_TYPE_ENUM(FN)                                               \
  FN(CLIENT)                                                                   \
  FN(SERVER)

#ifdef PT_LAN_PORT
#undef PT_LAN_PORT
#endif

#define PT_LAN_PORT 46575

#include <optional>
#include <string_view>
#include <thread>

#include "connection.hpp"
#include "linkcode.hpp"
#include "types.h"

namespace pt::network {

class lanconnection : pt::network::mgr::connection {
public:
  enum type { PT_LAN_CON_TYPE_ENUM(PT_ENUM) };
  static constexpr std::string_view type_s[] = {PT_LAN_CON_TYPE_ENUM(PT_ESTR)};

private:
  static SOCKET hsock;
  static constexpr uint16_t port = PT_LAN_PORT;

  static std::optional<std::thread> con_w;

  static int con_handler();

  template <bool S> static int handshake(SOCKET);

public:
  static int start();

  lanconnection(pt::linkcode);
  lanconnection(SOCKET);
};

} // namespace pt::network

#endif
