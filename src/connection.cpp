#include "connection.hpp"
#include <assert.h>
#include <sys/socket.h>

#include "pubip.h"

std::shared_ptr<pt::network::mgr> pt::network::mgr::instance = NULL;

pt::network::mgr::connection::connection() {}
