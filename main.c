// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sha3.h"

#define SHA_TYPE 32  // 32 bytes = 256 bits.

char *bytes_to_hex(unsigned char *bytes, int len) {
  char *hex = (char *)malloc(sizeof(char) * (2 * len + 1));

  for (int i = 0; i < len; i++) {
    sprintf(&hex[2 * i], "%02x", bytes[i]);
  }

  hex[2 * len + 1] = 0;

  return hex;
}

int main(int argc, char **argv) {
  /* Example */
  const char *msg = "Hola mundo!";
  uint8_t sha[SHA_TYPE];

  sha3(msg, strlen(msg), sha, sizeof(sha));

  printf("Mensaje: %s\n", msg);
  printf("SHA3-256: %s\n", bytes_to_hex(sha, sizeof(sha)));

  return 0;
}
