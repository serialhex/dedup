#include <stdio.h>
#include "fibb-hash.h"

int main() {
  printf("Trying to hash a file\n");
  File_Hash hash;

  hash = hash_file("emty.file");
  printf("emty.file hash: 0x%016I64X\n", hash.hash);
  printf("\n\n");

  hash = hash_file("dedup.c");
  printf("dedup.c hash: 0x%016I64X\n", hash.hash);
  printf("\n\n");

  hash = hash_file("big-ass.file");
  printf("big-ass.file hash: 0x%016I64X\n", hash.hash);
  printf("\n\n");

  return 0;
}
