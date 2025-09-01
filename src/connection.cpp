#include "connection.hpp"
#include <assert.h>
#include <exception>
#include <string.h>
#include <sys/socket.h>

#include "pubip.h"

#include <stdio.h>

uint32_t connection::pubip = 0;

connection::connection() {}

uint32_t connection::get_pubip() {
  if (connection::pubip)
    return connection::pubip;

  connection::pubip = ::get_pubip();
  assert(connection::pubip);
  return connection::pubip;
}
