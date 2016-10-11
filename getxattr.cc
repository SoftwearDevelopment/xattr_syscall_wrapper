#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include <unistd.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "xattr_syscall_wrapper.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "USAGE: getxattr [FILE] [XATTR] > data" << std::endl;
    return 1;
  }

  auto &file = argv[1], &attr = argv[2];
  std::vector<char> payload;

  // load the data

  size_t alloc = 1024*4; // One page
  while (true) {
    payload.resize(alloc);
    ssize_t len = getxattr(file, attr, payload.data(), payload.size());
    alloc *=2;

    if (len >= 0) { // success
      payload.resize(len);
      break;

    } else if (errno != ERANGE) { // unknown error
      fatal_errno(file, "getxattr");

    } else if (alloc > max_buf_size) { // to large
      std::cerr << "getxattr error: Xattr larger than " << max_buf_size << " bytes. "
                << "Refusing to allocate more.";
      return 1;
    }
  }

  // return the data

  std::cout.write(payload.data(), payload.size());
  return 0;
}


