#include "../include/config.h"
#include "../include/daemon.h"
#include "../include/mime.h"
#include "../include/sort.h"

#include <getopt.h>
#include <libgen.h>
#include <magic.h>
#include <stdio.h>
#include <string.h>

void help(void)
{
  puts(HELP_MESSAGE);
  return;
}

int main(int argc, char **argv)
{
  int c;
  int daemon_mode = 0;
  char *config_path;

  while ((c = getopt_long(argc, argv, SHORTOPTS, longopts, NULL)) != -1)
  {
    // Note: gotos are on the bottom
    switch (c)
    {
      case 'h':
        goto help_msg;
      case 'v':
        goto version_msg;
      case 'd':
        daemon_mode = 1;
        break;
      case 'c':
        config_path = optarg;
        break;
      default:
        break;
    }
  }

  magic_t magic = prepare_magic();

  if (!magic)
  {
    perror("  Could not open magic file");
    return 1;
  }

  if (daemon_mode)
  {
    struct config cfg;

    if (config_path) cfg = parse_config(config_path);
    else cfg = parse_config(config_path_default);

    if (!cfg.directory || !cfg.interval)
    {
      puts("Config is empty or invalid!");
      return 1;
    }
  
    return daemon(cfg, magic);
  }

  char *root;

  if (optind < argc) root = argv[optind++]; else root = ".";

  if (!strcmp(root, ".")  || !strcmp(root, "..") || !strcmp(root, "/"))
  {
    printf("Are you sure that you want to sort this directory? [Y/n] ");
    fflush(stdout);

    int c = fgetc(stdin);

    switch (c)
    {
      case 'Y':
      case 'y':
        break;
      default:
        return 0;
    }
  }

  sort_dir(root, magic);

  magic_close(magic);
  return 0;

  help_msg:
    puts(HELP_MESSAGE);
    return 0;

  version_msg:
    printf(VERSION_MESSAGE, version);
    return 0;
}
