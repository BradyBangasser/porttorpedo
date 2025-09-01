#pragma once

#ifndef PUBIP_H
#define PUBIP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
uint16_t get_natport(uint16_t lport);
uint32_t get_pubip();

#ifdef __cplusplus
}
#endif

#endif
