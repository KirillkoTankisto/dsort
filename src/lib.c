#include "../include/lib.h"

#include <dirent.h>
#include <magic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

void prepare_dirs(const char *path) {
  char *img = "Images";
  char *vid = "Videos";
  char *aud = "Audio";
  char *doc = "Documents";
  char *arc = "Archives";
  char *unk = "Unknown";

  size_t img_len = strlen(img);
  size_t vid_len = strlen(vid);
  size_t aud_len = strlen(aud);
  size_t doc_len = strlen(doc);
  size_t arc_len = strlen(arc);
  size_t unk_len = strlen(unk);
  
  size_t path_len = strlen(path);

  char *image_dir = strdup(path);
  char *video_dir = strdup(path);
  char *audio_dir = strdup(path);
  char *document_dir = strdup(path);
  char *archive_dir = strdup(path);
  char *unknown_dir = strdup(path);

  image_dir = realloc(image_dir, path_len + img_len + 1);
  video_dir = realloc(video_dir, path_len + vid_len + 1);
  audio_dir = realloc(audio_dir, path_len + aud_len + 1);
  document_dir = realloc(document_dir, path_len + doc_len + 1);
  archive_dir = realloc(archive_dir, path_len + arc_len + 1);
  unknown_dir = realloc(unknown_dir, path_len + unk_len + 1);

  image_dir[path_len] = '/';
  video_dir[path_len] = '/';
  audio_dir[path_len] = '/';
  document_dir[path_len] = '/';
  archive_dir[path_len] = '/';
  unknown_dir[path_len] = '/';
  
  memcpy(image_dir + path_len + 1, img, img_len);
  memcpy(video_dir + path_len + 1, vid, vid_len);
  memcpy(audio_dir + path_len + 1, aud, aud_len);
  memcpy(document_dir + path_len + 1, doc, doc_len);
  memcpy(archive_dir + path_len + 1, arc, arc_len);
  memcpy(unknown_dir + path_len + 1, unk, unk_len);

  mkdir(image_dir, 0755);
  mkdir(video_dir, 0755);
  mkdir(audio_dir, 0755);
  mkdir(document_dir, 0755);
  mkdir(archive_dir, 0755);
  mkdir(unknown_dir, 0755);

  return;
}