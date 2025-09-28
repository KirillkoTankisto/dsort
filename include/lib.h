#include <stddef.h>
#include "../include/config.h"
#include "../include/def.h"

enum FileTypes detect_filetype(const char *mime);

const char *get_mimetype(const char *filepath);

const char *get_abs_path(const char *dir, const char *file);

char **read_dir(const char *path);

void free_dir(char **string);

int prepare_dirs(const char *path, const char *const dirs[], size_t ndirs);

int is_file(const char *path);
