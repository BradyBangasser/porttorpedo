#pragma once

#ifndef PT_LINK_CODE
#define PT_LINK_CODE

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct linkcode {
  // only first 50 bits are used
  uint64_t code : 50;
  uint32_t check : 20;
} linkcode;

#ifdef __cplusplus
}
#endif

#endif
