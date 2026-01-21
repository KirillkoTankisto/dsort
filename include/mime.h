#include "../include/def.h"

#include <magic.h>

enum FileTypes
{
  Unknown,
  Image,
  Video,
  Audio,
  Document,
  Archive,
};

magic_t prepare_magic(void);

const char *get_mimetype(const char *filepath, magic_t magic);

enum FileTypes detect_filetype(const char *mime);

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