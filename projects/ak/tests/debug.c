#include "ak/core/io.h"
#include "ak/os/file.h"
#include <ak/debug.h>

int
main()
{
  ak_iostream file =
    ak_fstream_make("./log.txt", "w");
  ak_debug_make(file);

  ak_log("Hello, %s", "World!");

  ak_iostream_close(file);

  return 0;
}
