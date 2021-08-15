#include "../include/nbt/nbt_reader.hpp"

void test() {
  nbt::Compound compound;

  compound["float"] = 420.f;

  nbt::Compound subCompound;
  compound["compound"] = std::move(subCompound);
}