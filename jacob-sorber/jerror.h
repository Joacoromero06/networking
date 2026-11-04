#ifndef __JERROR_H__
#define __JERROR_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

void err_n_die(const char* fmt, ...);
char* bin2hex(char* s, size_t size);

void err_n_die(const char* fmt, ...) {
  va_list va;
  int errno_save;

  va_start(va, fmt);
  errno_save = errno;
  vfprintf(stdout, fmt, va);
  fprintf(stdout, "\n");
  fflush(stdout);

  if (errno_save != 0) 
  {
    fprintf(stdout, "errno=%d (%s)", errno_save, strerror(errno_save));
    fprintf(stdout, "\n");
    fflush(stdout);
  }
  va_end(va);
  exit(1);
}
char* bin2hex(char* s, size_t size) {
  assert(s != NULL && "bin string null in bin2hex");
  assert(size >= 0 && "size 0 en bin2hex");
  char* hex = (char*) malloc(size*3+1);
  bzero(hex, size*3+1);

  char* hexits = "0123456789ABCDEF";
  for (int i=0; i < size; ++i)
  {
    hex[i*3] = hexits[s[i] >> 4];
    hex[i*3+1] = hexits[s[i] & 0x0f];
    hex[i*3+2] = ' ';
  }
  return hex;
}
#endif
