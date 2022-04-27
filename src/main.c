#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "./recdir.h"

int main(int argc, char** argv)
{ 
  (void) argc; (void) argv;

  errno = 0;
  RECDIR* recdir = recdir_open(".");
  struct dirent* ent = recdir_read(recdir);
  while (ent) {
    printf("recdir file: %s/%s\n", recdir_top(recdir)->path, ent->d_name);
    ent = recdir_read(recdir);
  }
  
  if (errno != 0) {
    fprintf(stderr, "ERROR: We could not read the directory: %s\n", recdir_top(recdir)->path);
    exit(1);
  }

  recdir_close(recdir);

  return 0;
}
