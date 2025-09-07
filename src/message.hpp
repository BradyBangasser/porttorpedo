#pragma once

#ifndef PT_MESSAGE_HPP
#define PT_MESSAGE_HPP

#include <memory>

namespace pt {
template <typename P> class message {
  friend class connection;

private:
  std::unique_ptr<P> payload;
};
} // namespace pt

#endif
