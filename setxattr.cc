#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>

#include <vector>
#include <iostream>
#include <algorithm>

#include "xattr_syscall_wrapper.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "USAGE: setxattr [FILE] [XATTR] < data" << std::endl;
    return 1;
  }

  auto &file = argv[1], &attr = argv[2];
  std::vector<char> payload;

  // load the data

  size_t have = 0, alloc = 1024*4; // One page
  while (true) {
    payload.resize(alloc);
    std::cin.read(payload.data()+have, payload.size() - have);

    alloc *=2;
    have += std::cin.gcount();

    if (std::cin.eof()) { // success
      payload.resize(have);
      break;

    } else if (alloc > max_buf_size) { // to large
      std::cerr << "read error: data larger than " << max_buf_size << " bytes. "
                << "Refusing to allocate more.";
      return 1;
    }
  }

  // return the data
  int r = setxattr(file, attr, payload.data(), payload.size(), 0);
  if (r == -1)
    fatal_errno(file, "setxattr");

  return 0;
}



