#pragma once

#ifndef PT_PEER_HPP
#define PT_PEER_HPP

#include <memory>

namespace pt {

class peer {
public:
  using id = std::shared_ptr<peer>;
};
} // namespace pt

#endif
