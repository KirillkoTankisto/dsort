#include "../include/lib.h"

#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
  const char *root = argc > 1 ? argv[1] : ".";

  char **files = read_dir(root);

  if (!files) return 1;

  prepare_dirs(root, SUBDIRS, SUBDIRS_LEN);

  char *path;

  for (int i = 0; (path = files[i]); i++)
  {
    if (!is_file(path)) continue;

    char *tmp = strdup(path);
    char *name = basename(tmp);

    if (!strcmp(name, ".") || !(strcmp(name, "..")))
    {
      free(tmp);
      continue;
    }

    int is_target_dir = 0;

    for (size_t j = 0; j < SUBDIRS_LEN; j++)
    {
      if (!strcmp(name, SUBDIRS[j])) { is_target_dir = 1; break; }
    }

    if (is_target_dir) { free(tmp); continue; }

    const char *mime = get_mimetype(path);
    enum FileTypes type = detect_filetype(mime);

    const char *dest_dir = get_abs_path(root, SUBDIRS[type]);
    const char *dest = get_abs_path(dest_dir, name);

    rename(path, dest);

    free(tmp);
    free((void*)mime);
    free((void*)dest_dir);
    free((void*)dest);
  }

  free_dir(files);

  return 0;
}
