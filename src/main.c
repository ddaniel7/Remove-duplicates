#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>  
#include <assert.h>

#define PATH_SEP "/"
#define PATH_SEP_LEN (sizeof(PATH_SEP) - 1)

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

void print_files_recursively(const char* dir_path)
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
    if (ent->d_type == DT_DIR) {
      char* child_path = join_path(dir_path, ent->d_name);
      print_files_recursively(child_path);
      free(child_path);
    } 
    else 
    {
      printf("file:  %s\n", ent->d_name);
    }
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
  const char *dir_path = ".";
  print_files_recursively(dir_path); 

  return 0;
}
