// main.c
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sha3.h"

char *bytes_to_hex(unsigned char *bytes, int len) {
  char *hex = (char *)malloc(sizeof(char) * (2 * len + 1));

  for (int i = 0; i < len; i++) {
    sprintf(&hex[2 * i], "%02x", bytes[i]);
  }

  hex[2 * len + 1] = 0;

  return hex;
}

// read a hex string, return byte length or -1 on error.
static int test_hexdigit(char ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
  return -1;
}

static int test_readhex(uint8_t *buf, const char *str, int maxbytes) {
  int i, h, l;

  for (i = 0; i < maxbytes; i++) {
    h = test_hexdigit(str[2 * i]);
    if (h < 0) return i;
    l = test_hexdigit(str[2 * i + 1]);
    if (l < 0) return i;
    buf[i] = (h << 4) + l;
  }

  return i;
}

// returns zero on success, nonzero + stderr messages on failure

int test_sha3() {
  // message / digest pairs, lifted from ShortMsgKAT_SHA3-xxx.txt files
  // in the official package: https://github.com/gvanas/KeccakCodePackage

  // SHA3-256
  const char *testvec[][2] = {
      {"", "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a"},
      {"Hola",
       "e80b9c74112cae76f10f35ef86a6c2fbbe011d22c4d283addb4c4d3b0c513e36"},
      {"Mensaje de prueba #333.",
       "fe7c5fbbc1151490f3b185dbf3be0dd7345b82c837789aa2b7a78525375a7958"}};

  int i, fails, msg_len, sha_len;
  uint8_t sha[64], buf[64], msg[256];

  fails = 0;
  for (i = 0; i < 2; i++) {
    memset(sha, 0, sizeof(sha));
    memset(buf, 0, sizeof(buf));
    memset(msg, 0, sizeof(msg));

    msg_len = strlen(testvec[i][0]);
    memcpy(msg, testvec[i][0], msg_len);
    sha_len = test_readhex(sha, testvec[i][1], sizeof(sha));

    // printf("Len: %d %d\n", msg_len, sha_len);

    sha3(msg, msg_len, buf, sha_len);

    if (memcmp(sha, buf, sha_len) != 0) {
      fprintf(stderr, "[%d] SHA3-%d, len %d test FAILED. (%s)\n", i,
              sha_len * 8, msg_len, bytes_to_hex(buf, sha_len));
      fails++;
    }
  }

  return fails;
}

// main
int main(int argc, char **argv) {
  if (test_sha3() == 0) {
    printf("Test success!\n");
  }

  /* Example */
  const char *msg = "Hola mundo!";
  uint8_t sha[32];

  sha3(msg, strlen(msg), sha, 32);
  printf("Mensaje: %s\n", msg);
  printf("SHA3-256: %s\n", bytes_to_hex(sha, 32));

  return 0;
}
