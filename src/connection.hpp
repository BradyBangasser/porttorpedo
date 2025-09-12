#pragma once

#include <mutex>
#include <thread>
#include <vector>
#ifndef PT_CONNECTION_HPP
#define PT_CONNECTION_HPP

#include <arpa/inet.h>
#include <netinet/in.h>

#include <future>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

#include "linkcode.hpp"
#include "message.hpp"
#include "peer.hpp"
#include "types.h"

namespace pt::network {

class mgr {
private:
  class connection {
  public:
    enum state {
      DISCONNECTED = 0,
      CONNECTED,
      ERROR,
      // Error and disconnected state
      DERROR,
    };

  private:
    static pt_conn_id c_ctr;
    static std::shared_ptr<mgr> mgr;

    connection();

    enum state state = DISCONNECTED;

  protected:
#warning "todo add state change notifications"
    inline int set_state(enum state ns) { return (this->state = ns); }

    SOCKET sock = -1;
    static uint32_t pubip;
    std::unique_ptr<linkcode> peer = NULL;

    connection(pt::peer::id);

  public:
    static uint32_t get_pubip();
    static const inline std::string get_pubip_s() {
      struct in_addr addr = {connection::get_pubip()};
      return std::string(inet_ntoa(addr));
    }

    virtual int disconnect() = 0;
  };

  static std::shared_ptr<mgr> instance;

  std::unordered_map<pt_conn_id, connection> conns;
  std::mutex conn_l;
  std::priority_queue<pt::message<std::string>> msg_q;
  std::mutex msg_q_l;

  std::thread poll_w;
  std::vector<std::thread> msg_w;

  int new_msg_w();

  mgr();

  int msg_w_f();

public:
  static inline std::shared_ptr<mgr> get_instance() {
    return instance == NULL ? (instance = std::make_shared<mgr>(mgr()))
                            : instance;
  }

  int pcs_msg(pt::message<std::string> msg);
  template <typename S, typename T>
  std::future<std::unique_ptr<pt::message<T>>>
  send_msg(std::unique_ptr<pt::message<T>> msg);
};

} // namespace pt::network

#endif
