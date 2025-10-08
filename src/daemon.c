#include "../include/config.h"
#include "../include/daemon.h"
#include "../include/sort.h"

#include <stdio.h>
#include <time.h>

int daemon(struct config cfg, magic_t magic)
{

  puts("  Starting daemon...");
  const struct timespec interval = { .tv_sec = cfg.interval };

  while (1)
  {
    if (sort_dir(cfg.directory, magic)) return 1;
    nanosleep(&interval, NULL);
  }
  
  return 0;
}
