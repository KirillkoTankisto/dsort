#include <stddef.h>

#define ARR_LEN(a) (sizeof(a) / sizeof((a)[0]))

enum FileTypes {
  Image,
  Video,
  Audio,
  Document,
  Archive,
  Unknown,
};

enum FileTypes detect_filetype(const char *mime);

const char *get_mimetype(const char *filepath);

int has_prefix(const char *string, const char *prefix);

const char *get_abs_path(const char *dir, const char *file);

char **read_dir(const char *path);

void free_dir(char **string);

struct mimes_list
{
  char **filepaths;
  char **mimetypes;
  size_t length;
};

int append_mime(struct mimes_list *mime, char *filepath, const char *mimetype);

struct mimes_list generate_mimelist(char **files);

int prepare_dirs(const char *path, const char *const dirs[], size_t ndirs);
