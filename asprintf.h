
#ifndef HAVE_ASPRINTF
#define HAVE_ASPRINTF

#include <stdio.h>  // vsnprintf
#include <stdlib.h> //malloc
#include <stdarg.h> // va_start et al

/* The declaration, to put into a .h file. The __attribute___ tells the compiler to check printf-style type-compliance. It's not C-standard, but a lot of compilers support it; just remove it if yours doesn't. */

int asprintf(char **str, char* fmt, ...); //__attribute__ ((format (printf,2,3)));

int asprintf(char **str, char* fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  char one_char[1];

  int len = vsnprintf(one_char, 1, fmt, argp);
  if (len < 1) {
    fprintf(stderr, "An encoding error occurred. Setting the input pointer to NULL.\n");
    *str = NULL;
    return len;
  }
  va_end(argp);

  *str = malloc(len+1);
  if (!str) {
    fprintf(stderr, "Couldn't allocate %i bytes.\n", len+1);
    return -1;
  }
  va_start(argp,fmt);
  vsnprintf(*str, len+1, fmt, argp);
  va_end(argp);
  return len;
}

// this may not work at all...
// char* rsprintf(char* fmt, ...) {
//   va_list argp;
//   char** str;
//   va_start(argp, fmt);
//   asprintf(str, fmt, argp);
//   va_end(argp);
//   return str;
// }
#endif

#ifdef Test_asprintf
int main() {
  char *s;
  asprintf(&s, "Hello, %s", "--Reader--");
  printf("%s\n", s);

  asprintf(&s, "%c", '\0');
  printf("blank string: [%s]\n", s);

  int i = 0;
  asprintf(%s, "%i", i++);
  printf("Zero: %s\n", s);
}
#endif
