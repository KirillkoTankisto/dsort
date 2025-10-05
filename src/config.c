#include "../include/config.h"
#include "../include/def.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void append_the_value(int character, FILE *file, char *string, size_t string_size)
{
  while ((character = fgetc(file)) != '=' && character != '\n' && character != EOF) {}

  size_t position = 0;

  while ((character = fgetc(file)) != '\n' && character != EOF)
  {
    if (character == ' ') continue;

    if (position + 1 > string_size)
    {
      string_size *= 2;
      
      char *tmp = realloc(string, string_size);
      if (!string) return;
      
      string = tmp;
    }
    string[position] = character;

    position++;
  }
  string[position + 1] = '\0';

  puts(string);

  return;
}

#define INTERVAL_SIZE 8
#define DIRECTORY_SIZE 32

struct config parse_config(const char *path)
{
  struct config cfg = { .directory = NULL, .interval = 0 };

  FILE *file = fopen(path, "r");
  if (!file) goto exit;

  char *interval_string = malloc(INTERVAL_SIZE);
  char *directory = malloc(DIRECTORY_SIZE);

  if (!interval_string || !directory) goto exit;

  size_t interval_size = INTERVAL_SIZE;
  size_t directory_size = INTERVAL_SIZE;
  
  int character;
  int at_start = 1;

  while ((character = fgetc(file)) != EOF)
  {
    if (character == '\n') { at_start = 1; continue; };
    if (!at_start) continue;
    
    switch (character) {
      case 'i':
        append_the_value(character, file, interval_string, interval_size);
        break;
      case 'd':
        append_the_value(character, file, directory, directory_size);
        break;
      default: break;
    }
  }

  fclose(file);

  char *endptr;
  errno = 0;
  
  int interval = strtol(interval_string, &endptr, 10);
  free(interval_string);

  if (errno != 0)
  {
    free(directory);
    goto exit;
  }
  
  cfg.interval = interval;
  cfg.directory = directory;

  exit: return cfg;
}
