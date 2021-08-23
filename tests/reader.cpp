#include <fstream>

#include <gtest/gtest.h>

#include "test.hpp"

TEST(Nbt, Reader) { //NOLINT
  std::vector<char> binary = readTestCompound();
  nbt::Compound value = nbt::Reader::parse(binary.data(), binary.size());

  EXPECT_TRUE(value["Level"].getCompound() == createTestCompound());
}