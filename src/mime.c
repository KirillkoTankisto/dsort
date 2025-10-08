#include "../include/def.h"
#include "../include/utils.h"

#include <magic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int try_load(magic_t magic, const char *magic_dirs[], size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    if (magic_load(magic, magic_dirs[i]) == 0) return 0;
  }

  return 1;
}

magic_t prepare_magic(void)
{
  magic_t magic = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);

  if (try_load(magic, MAGIC_DIRS, MAGIC_LEN)) return NULL;

  return magic;
}

const char *get_mimetype(const char *filepath, magic_t magic)
{
  const char *type = magic_file(magic, filepath);

  if (!type) return NULL;

  char *out = strdup(type);

  return out;
}

enum FileTypes detect_filetype(const char *mime)
{
  if (!mime) return Unknown;

  if (has_prefix(mime, "image/")) return Image;
  if (has_prefix(mime, "video/")) return Video;
  if (has_prefix(mime, "audio/")) return Audio;
  if (has_prefix(mime, "text/")) return Document;

  for (unsigned long i = 0; i < ARR_LEN(DOCUMENTS); i++) 
  {
    if (strcmp(mime, DOCUMENTS[i])) return Document;
  }

  for (unsigned long i = 0; i < ARR_LEN(ARCHIVES); i++)
  {
    if (strcmp(mime, ARCHIVES[i])) return Archive;
  }

  return Unknown;
}