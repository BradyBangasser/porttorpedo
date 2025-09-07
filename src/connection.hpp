#pragma once

#ifndef PT_CONNECTION_HPP
#define PT_CONNECTION_HPP

#include <arpa/inet.h>
#include <netinet/in.h>

#include <memory>
#include <string>

#include "linkcode.hpp"
#include "message.hpp"
#include "peer.hpp"
#include "types.h"

namespace pt::network {

class connection {
protected:
  enum state {
    DISCONNECTED = 0,
    CONNECTED,
    ERROR,
    // Error and disconnected state
    DERROR,
  };

private:
  static pt_conn_id c_ctr;

  connection();

  enum state state = DISCONNECTED;

protected:
#warning "todo add state change notifications"
  inline enum state set_state(enum state ns) { return (this->state = ns); }

  SOCKET sock = -1;
  static uint32_t pubip;
  std::unique_ptr<linkcode> peer = NULL;

  int on_message(std::unique_ptr<pt::message<const std::string>>) const;

  connection(pt::peer::id);

public:
  static uint32_t get_pubip();
  static const inline std::string get_pubip_s() {
    struct in_addr addr = {connection::get_pubip()};
    return std::string(inet_ntoa(addr));
  }

  virtual int disconnect() = 0;
};
} // namespace pt::network

#endif
