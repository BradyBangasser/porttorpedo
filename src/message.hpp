#pragma once

#ifndef PT_MESSAGE_HPP
#define PT_MESSAGE_HPP

#ifndef PT_ENUM
#define PT_ENUM(x) x,
#endif

#ifndef PT_ESTR
#define PT_ESTR(x) #x,
#endif

#ifdef PT_MTYPE_ENUM
#undef PT_MTYPE_ENUM
#endif

#define PT_MTYPE_ENUM(F)                                                       \
  F(KEEP_ALIVE)                                                                \
  F(WASSUP)                                                                    \
  F(CYA)                                                                       \
  F(KYS)

#include <stdint.h>

#include <memory>
#include <string_view>

namespace pt {
template <typename P> class message {
  friend class connection;

public:
  enum type : uint8_t { PT_MTYPE_ENUM(PT_ENUM) };
  static constexpr std::string_view s_type[] = {PT_MTYPE_ENUM(PT_ESTR)};

  const char *encode() const;
  template <typename T>
  static std::unique_ptr<message<P>>
  create(T &dest, const char *hdr, std::unique_ptr<P> payload, const char &tlr);

  template <typename T>
  static std::unique_ptr<message<P>> from(std::unique_ptr<message<T>> otr);

  std::string_view to_string() const;

private:
  using id_t = struct id {};

  enum type type;
  std::unique_ptr<P> payload;
};
} // namespace pt

#endif
