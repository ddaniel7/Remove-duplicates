#define _DEFAULT_SOURCE
#include "recdir.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

char* join_path(const char* base, const char* file);

int recdir_push(RECDIR* recdir, char* path)
{
  assert(recdir->stack_size < DIRS_CAP);
  RECDIR_Frame* top = &recdir->stack[recdir->stack_size];
  top->path = path;
  top->dir = opendir(top->path);
  if (top->dir == NULL)
  {
    free(top->path);
    return -1;
  }
  recdir->stack_size++;
  return 0;
}

void recdir_pop(RECDIR* recdir)
{ 
  assert(recdir->stack_size > 0);
  RECDIR_Frame* top = &recdir->stack[--recdir->stack_size];
  int ret = closedir(top->dir);
  assert(ret == 0);
  free(top->path);
}

RECDIR* openrecdir(const char* dir_path)
{
  RECDIR* recdir = malloc(sizeof(RECDIR));
  assert(recdir != NULL);
  memset(recdir, 0, sizeof(RECDIR));
  
  if (recdir_push(recdir, strdup(dir_path)) < 0)
  {
    free(recdir);
    return NULL;
  }

  return recdir;
}

struct dirent* readrecdir(RECDIR* recdirp)
{
  while (recdirp->stack_size > 0) {
    RECDIR_Frame* top = &recdirp->stack[recdirp->stack_size - 1];

    struct dirent* ent = readdir(top->dir);

    if (ent) {
      if (ent->d_type == DT_DIR) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
          continue;
        } else {
          recdir_push(recdirp, join_path(top->path, ent->d_name));
          continue;
        }
      } else {
        return ent;
      }
    } else {
      if (errno != 0) {
        return NULL;
      } else {
        recdir_pop(recdirp);
        continue;
      }
    }
  } 
  return NULL;
}

int closerecdir(RECDIR* recdirp)
{
  while (recdirp->stack_size > 0) {
    recdir_pop(recdirp);
  }
  free(recdirp);
  return 0;
}


char* join_path(const char* base, const char* file)
{
  size_t base_len = strlen(base);
  size_t file_len = strlen(file);
  char* result = malloc(base_len + file_len + PATH_SEP_LEN + 1);
  assert(result != NULL);

  char* end = result;
  memcpy(end, base, base_len);
  end += base_len;
  memcpy(end, PATH_SEP, PATH_SEP_LEN);
  end += PATH_SEP_LEN;
  memcpy(end, file, file_len);
  end += file_len;
  *end = '\0';

  return result;
}

char* recdir_path(RECDIR* recdir) 
{
  assert(recdir->stack_size > 0);
  return recdir->stack[recdir->stack_size - 1].path;
}
