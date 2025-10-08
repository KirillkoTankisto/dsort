#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int has_prefix(const char *string, const char *prefix)
{
  if (!string || !prefix) return 0;

  size_t lp = strlen(prefix);

  if (strlen(string) < lp) return 0;

  return (strncmp(string, prefix, lp) == 0);
}

const char *get_abs_path(const char *dir, const char *file)
{
  if (!dir || !file) return NULL;

  const size_t dir_len = strlen(dir);
  const size_t file_len = strlen(file);

  int need_sep = (dir[dir_len - 1] != '/');

  char *string = malloc(dir_len + file_len + (need_sep ? 2 : 1));

  if (!string) return NULL;

  char *pointer = string;

  // Directory
  memcpy(pointer, dir, dir_len);
  pointer += dir_len;

  // Insert separator if needed
  if (need_sep) *pointer++ = '/';

  // Filename
  memcpy(pointer, file, file_len + 1);
  pointer += file_len;

  // Null terminator
  *pointer = '\0';

  return string;
}

char **read_dir(const char *path)
{
  DIR *dir = opendir(path);
  if (!dir) return NULL;

  struct dirent *dir_i;

  char **files = NULL;

  int counter = 0;

  while ((dir_i = readdir(dir)) != NULL)
  {
    const char *filepath = get_abs_path(path, dir_i->d_name);

    char **tmp = realloc(files, (counter + 1) * sizeof(char *));

    files = tmp;
    files[counter] = strdup(filepath);

    counter++;
  }

  closedir(dir);

  files = realloc(files, (counter + 1) * sizeof(char *));
  files[counter] = NULL;

  return files;
}

void free_dir(char **string)
{
  for (int i = 0; string[i] != NULL; i++)
  {
    free(string[i]);
  }

  free(string);

  return;
}

int prepare_dirs(const char *path, const char *const dirs[], size_t ndirs)
{
  int fd = open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  if (fd == -1) return 1;

  int error = 0;

  for (size_t i = 0; i < ndirs; i++)
  {
    if (mkdirat(fd, dirs[i], 0755) == -1 && errno != EEXIST)
    {
      error = 1;
    }
  }

  close(fd);
  return error;
}

int is_file(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);

  return S_ISREG(path_stat.st_mode);
}