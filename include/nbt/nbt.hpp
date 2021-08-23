#ifndef NBT_INCLUDE_NBT_NBT_HPP_
#define NBT_INCLUDE_NBT_NBT_HPP_

#include <limits>

static_assert(std::numeric_limits<float>::is_iec559);
static_assert(std::numeric_limits<double>::is_iec559);

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

/**
 * Configurable settings for the library. Define header "nbt.tweaks.hpp" to override settings.
 */
namespace nbt::config {

namespace defaults {
/**
 * @return Returns whether the writer will automatically insert a blank named compound first, as the root tag.
 */
constexpr bool writeRootTag() { return false; }

/**
 * @return Returns whether the reader will automatically return the root tag (blank named compound), if present when reading/parsing, instead of the document compound.
 */
constexpr bool omitRootTag() { return false; }
} // namespace defaults

using namespace defaults;

} // namespace nbt::config

#if __has_include(<nbt.tweaks.hpp>)
  #include <nbt.tweaks.hpp>
#endif

#include "nbt_type.hpp"

#include "nbt_reader.hpp"
#include "nbt_writer.hpp"

#endif //NBT_INCLUDE_NBT_NBT_HPP_
