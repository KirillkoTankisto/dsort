static const char *version = "1.0";

#define ARR_LEN(a) (sizeof(a) / sizeof((a)[0]))

enum FileTypes
{
  Unknown,
  Image,
  Video,
  Audio,
  Document,
  Archive,
};

static const char *SUBDIRS[] =
{
  "Unknown",
  "Images",
  "Videos",
  "Audio",
  "Documents",
  "Archives",
};

static const unsigned int SUBDIRS_LEN = ARR_LEN(SUBDIRS);

// Arguments //

#include <getopt.h>

static const struct option longopts[] =
{
  { "help", no_argument, 0, 'h' },
  { "version", no_argument, 0, 'v' },
  { 0, 0, 0, 0 }
};

static const char *shortopts = "hv";

static const char *help_message =
"Usage: dsort OPTION... [DIR]\n"
"Sort selected directory.\n"
"\n"
"When no DIR, or when DIR is '.', sorts the current directory.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n"
"  -h, --help        display this help and exit\n"
"  -v, --version     output version information and exit\n"
"\n"
"Dsort's homepage: https://github.com/KirillkoTankisto/dsort"
;

static const char *version_message =
"Dsort %s\n"
"Copyright (C) 2025 KirillkoTankisto.\n"
"License GPLv3: https://www.gnu.org/licenses/gpl-3.0.html\n"
"This is free software: you are free to change and redistribute it.\n"
;
