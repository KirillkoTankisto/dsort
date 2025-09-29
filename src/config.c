#include "../include/config.h"
#include "../include/def.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void append_the_value(int character, FILE *file, char *string, int position) {
  while ((character = fgetc(file)) != '=' && character != '\n' && character != EOF) {}

  for (position = 0; (character = fgetc(file)) != '\n' && character != EOF; position++)
  {
    if (character == ' ') continue;
    string[position] = character;
  }

  string[position] = '\0';
  return;
}

struct config parse_config(const char *path)
{
  struct config cfg = { .directory = NULL, .interval = 0 };

  FILE *file = fopen(path, "r");
  if (!file) goto exit;

  char *interval_string = malloc(256);
  char *directory = malloc(256);
  if (!interval_string || !directory) goto exit;
  
  int character;
  int at_start = 1;
  int position = 0;

  while ((character = fgetc(file)) != EOF) {
    if (character == '\n') { at_start = 1; continue; };
    if (!at_start) continue;
    
    switch (character) {
      case 'i':
        append_the_value(character, file, interval_string, position);
        break;
      case 'd':
        append_the_value(character, file, directory, position);
        break;
      default: break;
    }
  }

  fclose(file);

  char *endptr;
  errno = 0;
  int interval = strtol(interval_string, &endptr, 10);
  if (errno != 0)
  {
    free(interval_string);
    free(directory);
    goto exit;
  }

  cfg.interval = interval;
  cfg.directory = directory;

  exit: return cfg;
}
