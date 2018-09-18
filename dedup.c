#include <stdio.h>
#include <string.h>
#include "fibb-hash.h"
#include "tree.h"
#include "dirent.h"

static int find_directory(Binary_Tree* tree, const char *dirname);

int main() {
  printf("Making a binary tree to keep some files in...\n");
  // Binary_Tree* tree = bt_make();

  // printf("\n");
  // File_Hash dedup = fh_init(".", "dedup.c");
  // bt_add(tree, &dedup);
  // print_binary_tree(tree);

  // printf("\n");
  // File_Hash emty = fh_init(".", "emty.file");
  // bt_add(tree, &emty);
  // print_binary_tree(tree);

  // printf("\n");
  // File_Hash big = fh_init(".", "big-ass.file");
  // bt_add(tree, &big);
  // print_binary_tree(tree);

  printf("\n\n");
  printf("Doing the find directory thing with a new tree.\n");
  Binary_Tree* new_tree = bt_make();
  find_directory(new_tree, ".");
  print_binary_tree(new_tree);

  return 0;
}

// shamelessly stolen from the dirent example source code repo.
static int find_directory(Binary_Tree* tree, const char *dirname) {
  DIR *dir;
  char buffer[PATH_MAX + 2];
  char *p = buffer;
  const char *src;
  char *end = &buffer[PATH_MAX];
  int ok;

  /* Copy directory name to buffer */
  src = dirname;
  while (p < end  &&  *src != '\0') {
    *p++ = *src++;
  }
  *p = '\0';

  /* Open directory stream */
  dir = opendir (dirname);
  if (dir != NULL) {
    struct dirent *ent;

    /* Print all files and directories within the directory */
    while ((ent = readdir (dir)) != NULL) {
      char *q = p;
      char c;

      /* Get final character of directory name */
      if (buffer < q) {
        c = q[-1];
      } else {
        c = ':';
      }

      /* Append directory separator if not already there */
      if (c != ':'  &&  c != '/'  &&  c != '\\') {
        *q++ = '/';
      }

      /* Append file name */
      src = ent->d_name;
      while (q < end  &&  *src != '\0') {
        *q++ = *src++;
      }
      *q = '\0';

      /* Decide what to do with the directory entry */
      switch (ent->d_type) {
        case DT_LNK:
        case DT_REG:
          /* Output file name with directory */
          // printf("dirname: %s, src: %s, buffer: ", dirname, p);
          // printf("%s\n", buffer);
          File_Hash* dedup = (File_Hash*)malloc(sizeof(File_Hash));
          *dedup = fh_init(dirname, p);
          bt_add(tree, dedup);
          break;

        case DT_DIR:
          /* Scan sub-directory recursively */
          if (strcmp(ent->d_name, ".") != 0
              &&  strcmp(ent->d_name, "..") != 0) {
            find_directory(tree, buffer);
          }
          break;

        default:
          /* Ignore device entries */
          /*NOP*/;
      }

    }

    closedir (dir);
    ok = 1;

  } else {
    /* Could not open directory */
    printf ("Cannot open directory %s\n", dirname);
    ok = 0;
  }

  return ok;
}
