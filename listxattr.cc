#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#include <unistd.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "xattr_syscall_wrapper.hpp"

template<typename Itr, typename Os>
void print_with_newline(Itr s, Itr end, Os &out) {
  while (s < end) {
    char *next = std::find(s, end, '\0');
    size_t slen = next - s;

    if (next == end) {
      std::cerr << "error: Found string that is not \\0 terminated "
                << "at the end of the xattr list. Exiting confused.";
      exit(1);
    }

    out.write(s, slen);
    out << std::endl;

    s += slen + 1;
  }

}


int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    std::cerr << "USAGE: "
      << std::endl << "  getxattr [FILE] > data"
      << std::endl << "  getxattr -z [FILE] > data"
      << std::endl;
    return 1;
  }

  auto &file = argv[1];
  bool print_zeros = std::string(file) == "-z";
  if (print_zeros) file = argv[2];

  std::vector<char> payload;

  // load the data

  size_t alloc = 1024*4; // One page
  while (true) {
    payload.resize(alloc);
    ssize_t len = listxattr(file, payload.data(), payload.size());
    alloc *=2;

    if (len >= 0) { // success
      payload.resize(len);
      break;

    } else if (errno != ERANGE) { // unknown error
      fatal_errno(file, "listxattr");

    } else if (alloc > max_buf_size) { // to large
      std::cerr << "listxattr error: xattr list larger than "
                << max_buf_size << " bytes. "
                << "Refusing to allocate more.";
      return 1;
    }
  }

  if (print_zeros)
    std::cout.write(payload.data(), payload.size());
  else
    print_with_newline(payload.data(), payload.data() + payload.size(),
                       std::cout);;

  return 0;
}
