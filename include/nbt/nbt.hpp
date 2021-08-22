#ifndef NBT_INCLUDE_NBT_NBT_HPP_
#define NBT_INCLUDE_NBT_NBT_HPP_

#include <limits>

static_assert(std::numeric_limits<float>::is_iec559);
static_assert(std::numeric_limits<double>::is_iec559);

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

#include "nbt_reader.hpp"
#include "nbt_type.hpp"
#include "nbt_writer.hpp"

#endif //NBT_INCLUDE_NBT_NBT_HPP_
