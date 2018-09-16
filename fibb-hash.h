

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char* filename;
  char* filepath;
  uint64_t hash;
  size_t filesize;
} File_Hash;

// 1KB: 1024
// 1MB: 1048576
// 2MB: 2097152
// 16MB: 16777216
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 16777216
#endif

const uint64_t Fibb64 = 11400714819323198485llu;

size_t fibbonacci_hash(size_t hash) {
  return hash * Fibb64;
}

File_Hash hash_file(char* filename) {
  File_Hash hashdata = { .filename = filename, .hash = 0, .filesize = 0 };
  uint8_t *buffer = NULL;
  FILE* data = fopen(filename, "rb");

  if (data) {
    buffer = (uint8_t*)malloc(sizeof(uint8_t) * BUFFER_SIZE);
    if (buffer) {
      size_t bytes_read = 0;
      bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
      while (bytes_read) {
        hashdata.filesize += bytes_read;
        for(size_t i = 0; i < bytes_read; i++) {
          hashdata.hash = (hashdata.hash + buffer[i]) * Fibb64;
        }
        bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
      }
      free(buffer);
    }
    fclose(data);
  }

  return hashdata;
}
