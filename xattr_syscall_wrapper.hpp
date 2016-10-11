#include <errno.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <iostream>

constexpr size_t max_buf_size = 1024*1024*100; // 100MB

inline void fatal_errno(const std::string &file, const std::string &what,
                        size_t _err=errno, size_t ret_code=1) {
  std::cerr << file << " " << what << ": " << strerror(_err) << std::endl;
  exit(ret_code);
}
