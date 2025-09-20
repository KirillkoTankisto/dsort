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
  [Unknown]  = "Unknown",
  [Image]    = "Images",
  [Video]    = "Videos",
  [Audio]    = "Audio",
  [Document] = "Documents",
  [Archive]  = "Archives",
};

static const unsigned int SUBDIRS_LEN = ARR_LEN(SUBDIRS);
