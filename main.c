#include <stdint.h>
#include "sha-256.h"
#include "sb3api.h"

#define NUM_EXECS 100
const char* TEST_STR = "hello world";

char convert_hex_nibble(uint8_t in) {
  if (in < 10) {
    return '0' + in;
  } else {
    return 'a' + (in - 10);
  }
}

int make_output_str(char *out, int size, const char *start, unsigned int num, const char *end) {
  if (!out || size <= 0) return -1;
  int pos = 0;

  for (int i = 0; start[i] != '\0'; i++) {
    if (pos >= size - 1) goto truncated;
    out[pos++] = start[i];
  }

  char tmp[20];
  int tlen = 0;

  if (num == 0) {
    tmp[tlen++] = '0';
  } else {
    while (num > 0) {
      tmp[tlen++] = '0' + (num % 10);
      num /= 10;
    }
  }

  for (int i = tlen - 1; i >= 0; i--) {
    if (pos >= size - 1) goto truncated;
    out[pos++] = tmp[i];
  }

  for (int i = 0; end[i] != '\0'; i++) {
    if (pos >= size - 1) goto truncated;
    out[pos++] = end[i];
  }

truncated:
  out[pos] = '\0';
  return pos;
}

int main(void) {
  uint8_t hash[32];
  char input[300];
  // 2 chars per byte + null terminator
  char output[65] = "Enter string to SHA256 hash (or type \"perf\" to test performance)";

  for (;;) {
    SB3_ask_str(input, output, 100);

    if (strcmp(input, "perf")) {
      calc_sha_256(hash, input, strlen(input));

      for (int i = 0; i < 32; i++) {
        uint8_t byte = hash[i];
        output[i*2]   = convert_hex_nibble(byte >> 4);
        output[i*2+1] = convert_hex_nibble(byte & 0b1111);
      }
      output[64] = '\0';

    } else {
      SB3_say_str("Running...");

      double start = SB3_days_since_2000();
      volatile uint8_t sink = 0;
      for (int i = 0; i < NUM_EXECS; i++) {
        calc_sha_256(hash, TEST_STR, strlen(TEST_STR));
        sink ^= hash[0];
      }

      double length = (SB3_days_since_2000() - start) * 24 * 60 * 60;
      unsigned int us = (length / NUM_EXECS) * 1000 * 1000;
      unsigned int hashes_per_sec = (NUM_EXECS / length);

      make_output_str(output, sizeof(output), "Time per SHA256 hash: ", us, " microseconds");
      SB3_ask_str(NULL, output, 0);
      make_output_str(output, sizeof(output), "Hashes per second: ", hashes_per_sec, "");
    }
  }
}
