#ifndef NBT_INCLUDE_NBT_NBT_WRITER_HPP_
#define NBT_INCLUDE_NBT_NBT_WRITER_HPP_

#include <functional>
#include <ostream>

#include "nbt_type.hpp"

namespace nbt {

class Writer {
 public:
  static void write(std::ostream& out, const Compound& compound, const std::string_view& name = "");
  static std::vector<char> writeToBuffer(const Compound& compound, const std::string_view& key = "");
};

} // namespace nbt

#endif //NBT_INCLUDE_NBT_NBT_WRITER_HPP_
