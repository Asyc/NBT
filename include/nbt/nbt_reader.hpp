#ifndef NBT_INCLUDE_NBT_NBT_READER_HPP_
#define NBT_INCLUDE_NBT_NBT_READER_HPP_

#include "nbt_type.hpp"

#include <ostream>

namespace nbt {

class Reader {
 public:
  static Compound parse(const void* data, size_t length);

  static Compound read(std::istream& in);
};

} // namespace nbt

#endif //NBT_INCLUDE_NBT_NBT_READER_HPP_
