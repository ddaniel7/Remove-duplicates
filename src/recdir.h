#ifndef RECDIR_H_
#define RECDIR_H_

#include <dirent.h>  

#define PATH_SEP "/"
#define PATH_SEP_LEN (sizeof(PATH_SEP) - 1)

#define DIRS_CAP 1024

char* join_path(const char* base, const char* file);

typedef struct {
  DIR* dir;
  char* path;
} RECDIR_Frame;

typedef struct {
  RECDIR_Frame stack[DIRS_CAP];
  size_t stack_size;
} RECDIR;

RECDIR_Frame* recdir_top(RECDIR* recdir);
int recdir_push(RECDIR* recdir, char* path);
void recdir_pop(RECDIR* recdir);
RECDIR* recdir_open(const char* dir_path);
struct dirent* recdir_read(RECDIR* recdirp);
int recdir_close(RECDIR* recdirp);


#endif //RECDIR_H_
