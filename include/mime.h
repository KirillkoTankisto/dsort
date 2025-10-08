#include "../include/def.h"

#include <magic.h>

magic_t prepare_magic(void);

const char *get_mimetype(const char *filepath, magic_t magic);

enum FileTypes detect_filetype(const char *mime);