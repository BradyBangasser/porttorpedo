#include "linkcode.hpp"
#include "natconnection.hpp"

#include <cstdlib>
#include <memory>

#include <stdio.h>

int main(int argc, char **argv) {
  static char buffer[32] = {0};
  static size_t blen = sizeof(buffer);

  static char bufport[32] = {0};
  static size_t bplen = sizeof(bufport);

  natconnection nc;

  printf("Enter pubip:\n");
  getline((char **)&buffer, &blen, stdin);

  printf("Enter port:\n");
  getline((char **)&bufport, &bplen, stdin);

  std::shared_ptr<linkcode> plink =
      std::make_shared<linkcode>(buffer, std::atoi(bufport));

  if (argc == 2) {
    nc.connect(plink);
  } else {
    nc.accept(plink);
  }

  return 0;
}
