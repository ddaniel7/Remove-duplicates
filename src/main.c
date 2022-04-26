#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "./recdir.h"


void visit_files(const char* dir_path)
{
  DIR *dir = opendir(dir_path);
  if (dir == NULL) {
    fprintf(stderr, "ERROR: Could not open directory %s: %s\n",
      dir_path, strerror(errno));
      exit(1);
  }

  errno = 0;
  struct dirent *ent = readdir(dir);
  while (ent != NULL) 
  {
    char* child_path = join_path(dir_path, ent->d_name);
    if (ent->d_type == DT_DIR) {
      if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) 
      {
        visit_files(child_path);
      }
    } 
    else 
    {
      printf("file: %s\n", child_path);
    }
    free(child_path);
    ent = readdir(dir);
  }

  if (errno != 0) {
    fprintf(stderr, "ERROR: could not read directory %s: %s\n",
      dir_path, strerror(errno));
      exit(1);
  }
  
  closedir(dir); 
}

int main(int argc, char** argv)
{ 
  (void) argc; (void) argv;

  errno = 0;
  RECDIR* recdir = openrecdir(".");
  struct dirent* ent = readrecdir(recdir);
  while (ent) {
    printf("recdir file: %s/%s\n", recdir_path(recdir), ent->d_name);
    ent = readrecdir(recdir);
  }
  
  if (errno != 0) {
    fprintf(stderr, "ERROR: We could not read the directory: %s\n", recdir_path(recdir));
    exit(1);
  }

  closerecdir(recdir);

  return 0;
}
