#include <fstream>

#include <gtest/gtest.h>

#include "nbt/nbt.hpp"

void inputTest(nbt::Compound& compound) {
  auto& level = compound["Level"].getCompound();

  {
    auto& nested = level["nested compound test"].getCompound();
    auto& egg = nested["egg"].getCompound();
    auto& ham = nested["ham"].getCompound();
    EXPECT_TRUE(strcmp(egg["name"].getString().c_str(), "Eggbert") == 0);
    EXPECT_TRUE(egg["value"].getFloat() == 0.5f);
    EXPECT_TRUE(strcmp(ham["name"].getString().c_str(), "Hampus") == 0);
    EXPECT_TRUE(ham["value"].getFloat() == 0.75f);
  }

  EXPECT_TRUE(level["intTest"].getInt() == 2147483647);
  EXPECT_TRUE(level["byteTest"].getByte() == 127);
  EXPECT_TRUE(strcmp(level["stringTest"].getString().c_str(), "HELLO WORLD THIS IS A TEST STRING \xc5\xc4\xd6!") == 0);

  {
    nbt::List& listLong = level["listTest (long)"].getList();
    int64_t expected = 11;
    for (const auto& value : listLong) {
      EXPECT_TRUE(value.getLong() == expected++);
    }
  }

  EXPECT_TRUE(level["doubleTest"].getDouble() == 0.49312871321823148);
  EXPECT_TRUE(level["floatTest"].getFloat() == 0.49823147058486938F);
  EXPECT_TRUE(level["longTest"].getLong() == 9223372036854775807LL);

  {
    nbt::List& listCompound = level["listTest (compound)"].getList();
    size_t index = 0;
    for (auto& value : listCompound) {
      auto& elementCompound = value.getCompound();
      EXPECT_TRUE(elementCompound["created-on"].getLong() == 1264099775885LL);
      std::string expected = "Compound tag #" + std::to_string(index++);
      EXPECT_TRUE(strcmp(elementCompound["name"].getString().c_str(), expected.c_str()) == 0);
    }
  }

  {
    nbt::ByteArray byteArray = level["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"].getByteArray();
    for (size_t n = 0; n < 1000; n++) {
      EXPECT_TRUE(byteArray[n] == (n*n*255+n*7)%100);
    }
  }

  EXPECT_TRUE(level["shortTest"].getShort() == 32767);
}

TEST(Nbt, Reader) {
  auto binary = std::ifstream("read_test.nbt", std::ios_base::binary);
  ASSERT_TRUE(binary.is_open());
  auto value = nbt::Reader::parse(binary);
  inputTest(value);
}