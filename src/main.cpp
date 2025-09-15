#include "lanconnection.hpp"
#include "linkcode.hpp"

#include <cstdlib>
#include <memory>

#include "pubip.h"

#include <stdio.h>

int main(int argc, char **argv) {
  static char *buffer = NULL;
  static size_t blen = 0;
  if (argc > 1) {
    pt::network::lanconnection::start();
    while (1)
      ;
  } else {
    printf("Enter pubip:\n");
    getline((char **)&buffer, &blen, stdin);
    printf("Got: %ld %s\n", blen, buffer);

    pt::linkcode lc(buffer, 46575);

    pt::network::lanconnection test(lc);
  }

  return 0;
}
