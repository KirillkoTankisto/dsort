struct config
{
  char *directory;
  unsigned int interval;
};

struct config parse_config(const char *path);
