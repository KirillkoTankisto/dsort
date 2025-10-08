static const char *version = "1.0";
static const char *config_path_default = "/etc/dsort.conf";

#define ARR_LEN(a) (sizeof(a) / sizeof((a)[0]))

// Config parsing //
#define INTERVAL_SIZE 8
#define DIRECTORY_SIZE 32

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

// Application filetypes //

static const char *DOCUMENTS[] =
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

static const char *ARCHIVES[] =
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

// Arguments //

#include <getopt.h>

static const struct option longopts[] =
{
  { "help", no_argument, 0, 'h' },
  { "version", no_argument, 0, 'v' },
  { "daemon", no_argument, 0, 'd' },
  { "config", required_argument, 0, 'c' },
  { 0, 0, 0, 0 }
};

static const char *SHORTOPTS = "hvdc:";

static const char *HELP_MESSAGE =
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

static const char *VERSION_MESSAGE =
"Dsort %s\n"
"Magic version: %d.%02d\n"
"Copyright (C) 2025 KirillkoTankisto (https://github.com/KirillkoTankisto).\n"
"License GPLv3: (https://www.gnu.org/licenses/gpl-3.0.html).\n"
"This is free software: you are free to change and redistribute it.\n"
;

// Magic //

static const char *MAGIC_DIRS[] =
{
  "/usr/share/file/misc/magic.mgc",   // tested on Arch Linux
  "/usr/share/misc/magic.mgc",    // tested on Alpine Linux
};

static const unsigned int MAGIC_LEN = ARR_LEN(MAGIC_DIRS);