#pragma once

#ifndef PT_SERVER_HPP
#define PT_SERVER_HPP

#include <inttypes>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include "client.h"

template <uint32_t NC = 16> class server {
	private:
		SOCKET sock;
		
		uint16_t port;
		uint32_t addr;

		std::unordered_map<uint64_t, client *> out_clients;
		std::unordered_set<uint64_t> in_clients;

#warning "Fix this void *"
		std::queue<void *> msg_queue;
};

#endif
