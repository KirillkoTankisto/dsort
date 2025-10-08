int has_prefix(const char *string, const char *prefix);

const char *get_abs_path(const char *dir, const char *file);

char **read_dir(const char *path);

void free_dir(char **string);

int prepare_dirs(const char *path, const char *const dirs[], unsigned long ndirs);

int is_file(const char *path);