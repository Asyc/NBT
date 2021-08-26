#include <gtest/gtest.h>

#include "test.hpp"

TEST(Nbt, Writer) { //NOLINT
  nbt::Compound compound = createTestCompound();
  auto buffer = nbt::Writer::writeToBuffer(compound, "Level");
  auto parsed = nbt::Reader::parse(buffer.data(), buffer.size());
  EXPECT_TRUE(parsed["Level"].getCompound() == compound);
}