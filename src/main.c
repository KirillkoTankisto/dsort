#include "../include/lib.h"

#include <getopt.h>
#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help(void)
{
  puts(help_message);
  return;
}

int main(int argc, char **argv)
{
  int c;
  while ((c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1)
  {
    // Note: gotos are on the bottom
    switch (c) {
      case 'h':
      case '?':
        goto help_msg;
      case 'v':
        goto version_msg;
      default:
        break;
    }
  }

  char *root;

  if (optind < argc) root = argv[optind++]; else root = ".";

  if (!strcmp(root, ".")  || !strcmp(root, "..") || !strcmp(root, "/")) {
    printf("Are you sure that you want to sort this directory? [Y/n] ");
    fflush(stdout);

    int c = fgetc(stdin);

    switch (c) {
      case 'Y':
      case 'y':
        break;
      default:
        return 0;
    }
  }

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

  help_msg:
    puts(help_message);
    return 0;

  version_msg:
    printf(version_message, version);
    return 0;
}
