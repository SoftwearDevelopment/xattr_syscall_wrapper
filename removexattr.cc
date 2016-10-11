#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>

#include <vector>
#include <iostream>
#include <algorithm>

#include "xattr_syscall_wrapper.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "USAGE: removexattr [FILE] [ATTR]" << std::endl;
    return 1;
  }

  auto &file = argv[1], attr = argv[2];

  // return the data
  int r = removexattr(file, attr);
  if (r == -1)
    fatal_errno(file, "setxattr");

  return 0;
}



