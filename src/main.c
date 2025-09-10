#include "../include/lib.h"
#include <stdio.h>
#include <stdlib.h>


// Just tests
int main(void) {

  const char *path = "/home/kirill/.musl/dsort/appico.png";

  const char *mime_type = get_mimetype(path);

  enum FileTypes filetype = detect_filetype(mime_type);

  printf("Filetype: %s\n", mime_type);

  if (filetype == Image) {
    printf("It's a Image file!\n");
  } else {
    printf("It's not a Image file!\n");
  }

  const char *string = get_abs_path("/opt/zapret/", "config");

  printf("%s\n", string);

  char **str = read_dir("/etc/");

  for (int i = 0; str[i] != NULL; i++) {
    printf("%s\n", str[i]);
  }

  free_dir(str);

  static const char *const dirs[] =
  {
    "Images",
    "Videos",
    "Audio",
    "Documents",
    "Archives",
    "Unknown"
  };

  prepare_dirs(".", dirs, ARR_LEN(dirs));
  
  return 0;
}
