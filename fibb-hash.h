

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "asprintf.h"

#pragma once

typedef uint64_t hash_value;

typedef struct File_Hash {
  char* filename;
  char* filepath;
  hash_value hash;
  size_t filesize;
} File_Hash;

const File_Hash null_file = { .filename = "",
                              .filepath = "",
                              .hash = 0,
                              .filesize = 0};

// 1KB: 1024
// 1MB: 1048576
// 2MB: 2097152
// 16MB: 16777216
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 16777216
#endif

const uint64_t Fibb64 = 11400714819323198485llu;

inline hash_value fibbonacci_hash(hash_value hash) {
  return hash * Fibb64;
}

File_Hash hash_file(File_Hash fhash);

File_Hash fh_init(char* filepath, char* filename) {
  File_Hash fhash = { .filename = strdup(filename),
                      .filepath = strdup(filepath) };
  // printf("-->\t initializing with %s : %s\n.", fhash.filepath, fhash.filename);
  return hash_file(fhash);
}

File_Hash hash_file(File_Hash fhash) {

  uint8_t *buffer = NULL;
  char* fname;
  asprintf(&fname, "%s%s", fhash.filepath, fhash.filename);
  FILE* data = fopen(fname, "rb");

  if (data) {
    buffer = (uint8_t*)malloc(sizeof(uint8_t) * BUFFER_SIZE);
    if (buffer) {
      size_t bytes_read = 0;
      bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
      while (bytes_read) {
        fhash.filesize += bytes_read;
        for(size_t i = 0; i < bytes_read; i++) {
          fhash.hash = fibbonacci_hash(fhash.hash + buffer[i]);
        }
        bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, data);
      }
      free(buffer);
    }
    fclose(data);
  } else {
    printf("-->\tCan't open file %s\n", fhash.filename);
  }

  return fhash;
}

// print filehash stuff
char* str_file_hash(File_Hash fhash) {
  char* str;
  asprintf(&str,
    "#File_Hash{.filepath \"%s\", .filename \"%s\", .filesize 0x%016I64X, .hash 0x%016I64X}",
    fhash.filepath, fhash.filename, fhash.filesize, fhash.hash);
  return str;
}

void print_file_hash(File_Hash fhash) {
  printf("%s", str_file_hash(fhash));
}

// a check to see if two files may be equal, based on size and hash value
bool file_hash_eq(File_Hash one, File_Hash two) {
  return (one.filesize == two.filesize) &&
         (one.hash == two.hash);
}

// a more strict comparison that checks to see if the directory and filename are
// also the same.
bool files_eq(File_Hash one, File_Hash two) {
  return (one.filesize == two.filesize) &&
         (one.hash == two.hash) &&
         (strcmp(one.filename, two.filename) == 0) &&
         (strcmp(one.filepath, two.filepath) == 0);
}
