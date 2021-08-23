#ifndef NBT_TESTS_TEST_HPP_
#define NBT_TESTS_TEST_HPP_

#include <fstream>

#include <gtest/gtest.h>

#include "nbt/nbt.hpp"

inline std::vector<char> readTestCompound() {
  std::ifstream in("read_test.nbt", std::ios_base::binary | std::ios_base::ate);
  if (!in.is_open()) throw std::runtime_error("failed to open read_test.nbt");
  std::vector<char> buffer(in.tellg());
  in.seekg(0);
  in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
  return std::move(buffer);
}

inline nbt::Compound createTestCompound() {
  nbt::Compound level;
  {
    nbt::Compound egg, ham;
    egg["name"] = "Eggbert";
    egg["value"] = 0.5f;
    ham["name"] = "Hampus";
    ham["value"] = 0.75f;

    nbt::Compound nestedCompound;
    nestedCompound["egg"] = std::move(egg);
    nestedCompound["ham"] = std::move(ham);

    level["nested compound test"] = std::move(nestedCompound);
  }

  level["intTest"] = static_cast<int32_t>(2147483647);
  level["byteTest"] = static_cast<int8_t>(127);
  level["stringTest"] = "HELLO WORLD THIS IS A TEST STRING \xc5\xc4\xd6!";

  {
    nbt::List listLong(nbt::Type::LONG);
    int64_t value = 11;
    for (size_t i = 0; i < 5; i++) {
      listLong.pushBack(value++);
    }

    level["listTest (long)"] = std::move(listLong);
  }

  level["doubleTest"] = 0.49312871321823148;
  level["floatTest"] = 0.49823147058486938F;
  level["longTest"] = 9223372036854775807LL;

  {
    nbt::List listCompound(nbt::Type::COMPOUND);

    size_t index = 0;
    for (size_t i = 0; i < 2; i++) {
      nbt::Compound elementCompound;
      elementCompound["created-on"] = 1264099775885LL;
      elementCompound["name"] = "Compound tag #" + std::to_string(index++);
      listCompound.emplaceBack(std::move(elementCompound));
    }

    level["listTest (compound)"] = std::move(listCompound);
  }

  {
    nbt::ByteArray byteArray(1000);
    for (size_t n = 0; n < byteArray.size(); n++) {
      byteArray[n] = static_cast<int8_t>((n * n * 255 + n * 7) % 100);
    }

    level["byteArrayTest (the first 1000 values of (n*n*255+n*7)%100, starting with n=0 (0, 62, 34, 16, 8, ...))"] = std::move(byteArray);
  };

  level["shortTest"] = static_cast<int16_t>(32767);

  return std::move(level);
}

#endif //NBT_TESTS_TEST_HPP_
