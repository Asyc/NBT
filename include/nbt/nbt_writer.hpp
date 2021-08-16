#ifndef NBT_INCLUDE_NBT_NBT_WRITER_HPP_
#define NBT_INCLUDE_NBT_NBT_WRITER_HPP_

#include "nbt_type.hpp"

namespace nbt {

class Writer {
 public:
  static void serializeTo(std::function<void(const char*, size_t)> writeFunction, const Compound& compound);
  static std::vector<char> serialize(const Compound& compound);
};

}

#endif //NBT_INCLUDE_NBT_NBT_WRITER_HPP_
