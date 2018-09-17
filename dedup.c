#include <stdio.h>
#include "fibb-hash.h"
#include "tree.h"
#include "dirent.h"

static int find_directory (const char *dirname);

int main() {
  printf("Trying to hash a file\n");

  printf("\n");
  File_Hash dedup = hash_file("dedup.c");
  Binary_Tree* tree = bt_make(dedup);
  print_binary_tree(tree);

  printf("\n");
  File_Hash emty = hash_file("emty.file");
  bt_add(tree, &emty);
  print_binary_tree(tree);

  printf("\n");
  File_Hash big = hash_file("big-ass.file");
  bt_add(tree, &big);
  print_binary_tree(tree);

  find_directory(".");

  return 0;
}


static int
find_directory(
    const char *dirname)
{
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
                printf ("%s\n", buffer);
                break;

            case DT_DIR:
                /* Scan sub-directory recursively */
                if (strcmp (ent->d_name, ".") != 0
                        &&  strcmp (ent->d_name, "..") != 0) {
                    find_directory (buffer);
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
