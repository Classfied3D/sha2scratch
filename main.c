#include <stdint.h>
#include "sha-256.h"
#include "sb3api.h"

char convert_hex_nibble(uint8_t in) {
  if (in < 10) {
    return '0' + in;
  } else {
    return 'a' + (in - 10);
  }
}

int main(void) {
  uint8_t hash[32];
  char input[100];
  char output[65] = "Enter string to hash"; // 2 chars per byte + null terminator

  for (;;) {
    SB3_ask_str(input, output, 100);
    calc_sha_256(hash, input, strlen(input));

    for (int i = 0; i < 32; i++) {
      uint8_t byte = hash[i];
      output[i*2]   = convert_hex_nibble(byte >> 4);
      output[i*2+1] = convert_hex_nibble(byte & 0b1111);
    }
    output[64] = '\0';
  }
}
