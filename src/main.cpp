#include "linkcode.hpp"
#include "natconnection.hpp"

#include <cstdlib>
#include <memory>

#include "pubip.h"

#include <stdio.h>

int main(int argc, char **argv) {
  printf("Test: %x\n", ntohs(get_natport(htons(0x6967))));
  static char *buffer = NULL;
  static size_t blen = 0;

  static char *bufport = NULL;
  static size_t bplen = 0;

  natconnection nc;

  printf("Enter pubip:\n");
  getline((char **)&buffer, &blen, stdin);
  printf("Got: %ld %s\n", blen, buffer);

  printf("Enter port:\n");
  getline((char **)&bufport, &bplen, stdin);

  printf("Got %s\n", bufport);

  std::shared_ptr<pt::linkcode> plink =
      std::make_shared<pt::linkcode>(buffer, std::atoi(bufport));

  nc.hp_con_udp(plink);

  return 0;
}
