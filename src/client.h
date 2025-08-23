#pragma once
#ifndef PT_CLIENT_H
#define PT_CLIENT_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#warning "Add windows check"
// Stop Windows from bitching
typedef int SOCKET;

typedef struct client {
	uint32_t addr;
	uint16_t port;

	SOCKET sock;
} client;

#ifdef __cplusplus
}

int pt_client_
#endif
