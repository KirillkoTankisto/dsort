#include "../include/config.h"
#include "../include/daemon.h"
#include "../include/sort.h"

#include <time.h>

int daemon(struct config cfg)
{

  const struct timespec interval = { .tv_sec = cfg.interval };

  while (1)
  {
    sort_dir(cfg.directory);
    nanosleep(&interval, NULL);
  }
  
  return 0;
}
