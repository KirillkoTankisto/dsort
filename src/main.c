#include "../include/config.h"
#include "../include/daemon.h"
#include "../include/mime.h"
#include "../include/sort.h"

#include <getopt.h>
#include <libgen.h>
#include <magic.h>
#include <stdio.h>
#include <string.h>

// Arguments //

static const struct option longopts[] =
{
  { "help", no_argument, 0, 'h' },
  { "version", no_argument, 0, 'v' },
  { "daemon", no_argument, 0, 'd' },
  { "config", required_argument, 0, 'c' },
  { 0, 0, 0, 0 }
};

static const char *SHORTOPTS = "hvdc:";

static const char *HELP_MESSAGE =
"Usage: dsort OPTION... [DIR]\n"
"Sort selected directory.\n"
"\n"
"When no DIR, or when DIR is '.', sorts the current directory.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -h, --help        display this help and exit\n"
"  -v, --version     output version information and exit\n"
"  -c, --config FILE select configuration file\n"
"  -d, --daemon      enter daemon mode\n"
"\n"
"Dsort's homepage: https://github.com/KirillkoTankisto/dsort"
;

static const char *VERSION_MESSAGE =
"Dsort %s\n"
"Magic version: %d.%02d\n"
"Copyright (C) 2025 KirillkoTankisto (https://github.com/KirillkoTankisto).\n"
"License GPLv3: (https://www.gnu.org/licenses/gpl-3.0.html).\n"
"This is free software: you are free to change and redistribute it.\n"
;

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
    puts("  Could not open magic file");
    return 1;
  }

  if (daemon_mode)
  {
    struct config cfg;

    if (config_path) cfg = parse_config(config_path);
    else cfg = parse_config(CONFIG_PATH);

    if (!cfg.directory || !cfg.interval)
    {
      magic_close(magic);

      puts("  Config is empty or invalid!");
      return 1;
    }
  
    if (daemon(cfg, magic))
    {
      magic_close(magic);

      puts("  Daemon failed");
      return 1;
    };
  }

  char *root;

  if (optind < argc) root = argv[optind++]; else root = ".";

  if (!strcmp(root, ".")  || !strcmp(root, "..") || !strcmp(root, "/"))
  {
    printf("  Are you sure that you want to sort this directory? [y/N] ");
    fflush(stdout);

    int c = fgetc(stdin);

    switch (c)
    {
      case 'Y':
      case 'y':
        break;
      default:
        magic_close(magic);
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
    printf(VERSION_MESSAGE, VERSION, MAGIC_VERSION / 100, MAGIC_VERSION % 100);
    return 0;
}
