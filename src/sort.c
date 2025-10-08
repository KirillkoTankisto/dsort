#include "../include/mime.h"
#include "../include/utils.h"

#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sort_dir(const char *root) 
{
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
