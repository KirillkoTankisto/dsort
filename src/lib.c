#include "../include/lib.h"

#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <magic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *documents[] =
{
  "application/pdf",
  "application/msword",
  "application/vnd.oasis.opendocument.text",
  "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
  "application/vnd.ms-powerpoint",
  "application/vnd.openxmlformats-officedocument.presentationml.presentation",
  "application/vnd.ms-excel",
  "application/vnd.oasis.opendocument.spreadsheet"
};

const char *archives[] =
{
  "application/x-archive",
  "application/x-bzip1-compressed-tar",
  "application/x-bzip1",
  "application/x-bzip2-compressed-tar",
  "application/x-bzip2",
  "application/x-bzip3-compressed-tar",
  "application/x-bzip3",
  "application/x-compressed-tar",
  "application/x-lrzip",
  "application/x-lyx",
  "application/x-lz4",
  "application/x-lzip",
  "application/x-lzma",
  "application/x-7z-compressed",
  "application/x-lzop",
  "application/x-tar",
  "application/x-xz-compressed-tar",
  "application/x-xz",
  "application/zip",
  "application/zlib",
  "application/x-zstd-compressed-tar",
  "application/zstd"
};

enum FileTypes detect_filetype(const char *mime) {
  if (!mime) return Unknown;

  if (has_prefix(mime, "image/")) return Image;
  if (has_prefix(mime, "video/")) return Video;
  if (has_prefix(mime, "audio/")) return Audio;
  if (has_prefix(mime, "text/")) return Document;

  for (unsigned long i = 0; i < ARR_LEN(documents); i++) 
  {
    if (strcmp(mime, documents[i])) return Document;
  }

  for (unsigned long i = 0; i < ARR_LEN(archives); i++)
  {
    if (strcmp(mime, archives[i])) return Archive;
  }

  return Unknown;
}

int has_prefix(const char *string, const char *prefix) {
  if (!string || !prefix) return -1;

  if (strlen(string) < strlen(prefix)) return -1;

  return (strcmp(string, prefix));
}

const char *get_mimetype(const char *filepath) {
  magic_t magic = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
  magic_load(magic, NULL);

  const char *type = magic_file(magic, filepath);

  if (!type) return NULL;

  char *out = strdup(type);

  /* magic_t must be closed right after
    duping the string because *type
    is owned by the magic_t o_o
  */
  magic_close(magic);

  return out;
}

const char *get_abs_path(const char *dir, const char *file) {
  if (!dir || !file)
    return NULL;

  const size_t dir_len = strlen(dir);
  const size_t file_len = strlen(file);

  int need_sep = (dir[dir_len - 1] != '/');

  char *string = malloc(dir_len + file_len + (need_sep ? 2 : 1));

  if (!string)
    return NULL;

  char *pointer = string;

  // Directory
  memcpy(pointer, dir, dir_len);
  pointer += dir_len;

  // Insert separator if needed
  if (need_sep)
    *pointer++ = '/';

  // Filename
  memcpy(pointer, file, file_len + 1);
  pointer += file_len;

  // Null terminator
  *pointer = '\0';

  return string;
}

char **read_dir(const char *path) {
  DIR *dir = opendir(path);
  struct dirent *dir_i;

  char **files = NULL;

  int counter = 0;

  while ((dir_i = readdir(dir)) != NULL) {
    const char *filepath = get_abs_path(path, dir_i->d_name);

    char **tmp = realloc(files, (counter + 1) * sizeof(char *));

    files = tmp;
    files[counter] = strdup(filepath);

    counter++;
  }

  files = realloc(files, (counter + 1) * sizeof(char *));
  files[counter] = NULL;

  return files;
}

void free_dir(char **string) {
  for (int i = 0; string[i] != NULL; i++)
  {
    free(string[i]);
  }

  free(string);

  return;
}

int append_mime(struct mimes_list *mime, char *filepath, const char *mimetype)
{
  char *filepath_dup = strdup(filepath);
  char *mimetype_dup = strdup(mimetype);
  
  size_t new_length = mime->length+1;

  char **new_filepaths = realloc(mime->filepaths, new_length * sizeof(*new_filepaths));
  char **new_mimetypes = realloc(mime->mimetypes, new_length * sizeof(*new_mimetypes));

  mime->filepaths = new_filepaths;
  mime->mimetypes = new_mimetypes;

  mime->filepaths[mime->length] = filepath_dup;
  mime->mimetypes[mime->length] = mimetype_dup;

  mime->length = new_length;

  return 1;
}

struct mimes_list generate_mimelist(char **files)
{
  struct mimes_list mimes = {0};
  char *file;

  for (int i = 0; (file = files[i]) != NULL; i++)
  {
    const char *mimetype = get_mimetype(file);
    append_mime(&mimes, file, mimetype);
  }

  return mimes;
}

int prepare_dirs(const char *path, const char *const dirs[], size_t ndirs)
{
  int fd = open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
  if (fd == -1) return 0;

  int error = 1;

  for (size_t i = 0; i < ndirs; i++)
  {
    if (mkdirat(fd, dirs[i], 0755) == -1 && errno != EEXIST)
    {
      error = 0;
    }
  }

  close(fd);
  return error;
}
