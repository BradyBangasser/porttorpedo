#include "linkcode.hpp"
#include "natconnection.hpp"

#include <cstdlib>
#include <memory>

#include <stdio.h>

int main(int argc, char **argv) {
  static char *buffer = NULL;
  static size_t blen = 0;

  static char bufport[32] = {0};
  static size_t bplen = sizeof(bufport);

  natconnection nc;

  printf("Enter pubip:\n");
  getline((char **)&buffer, &blen, stdin);
  printf("Got: %ld %s\n", blen, buffer);

  printf("Enter port:\n");
  getline((char **)&bufport, &bplen, stdin);

  std::shared_ptr<linkcode> plink =
      std::make_shared<linkcode>(buffer, std::atoi(bufport));

  /*
  if (argc == 2) {
    printf("Client\n");
    nc.connect(plink);
  } else {
    printf("Server\n");
    nc.accept(plink);
  }
*/
  nc.hp_con(plink);

  return 0;
}
