#include <string>
#include <linux/limits.h>
#include <libgen.h>
#include "../include/Platform.h"

static std::string GetProcessPath () {
  char exe_file[PATH_MAX + 1];
  int size;
  size = readlink("/proc/self/exe", exe_file, PATH_MAX);
  if ( size < 0 ) { 
    return "./";
  } else {
    exe_file[size] = '\0';
    return std::string(dirname(exe_file)) + "/";
  }
}

std::string ResourcePath ( std::string filename ) {
  return GetProcessPath() + "/resources/" + filename;
}
