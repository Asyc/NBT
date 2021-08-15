#include "nbt/nbt_reader.hpp"

#include <stdexcept>

#include "byteswap.hpp"

namespace nbt {

class ByteReader {
 public:
  ByteReader(const void* data, size_t length) : m_Data(reinterpret_cast<const char*>(data)), m_Length(length), m_Index(0) {}
  void read(void* dst, size_t length) {
    if (m_Index + length >= m_Length) {
      throw std::runtime_error("index out of bounds");
    }

    memcpy(dst, m_Data + m_Index, length);
    m_Index += length;
  }

  void advance(size_t x) { m_Index += x; }

  [[nodiscard]] bool eof() const { return m_Index + 1 == m_Length; }

  [[nodiscard]] const char* getCurrentPosition() const {return m_Data + m_Index; }
 private:
  const char* m_Data;
  size_t m_Index;
  size_t m_Length;
};

void readInto(Compound& compound, ByteReader& reader) {
  Type type;
  reader.read(&type, sizeof(type));

  uint16_t stringLength;
  reader.read(&stringLength, sizeof(stringLength));
  stringLength = hostToNetwork16(stringLength);

  std::string key(reader.getCurrentPosition(), stringLength);
  reader.advance(stringLength);

  switch (type) {
    case Type::BYTE: {
      uint8_t value;
      reader.read(&value, sizeof(value));
      compound[key.c_str()] = value;
      break;
    }
    case Type::SHORT: {
      uint16_t value;
      reader.read(&value, sizeof(value));
      compound[key.c_str()] = static_cast<int16_t>(hostToNetwork16(value));
      break;
    }
    case Type::INT: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      compound[key.c_str()] = static_cast<int32_t>(hostToNetwork32(value));
      break;
    }
    case Type::LONG: {
      uint64_t value;
      reader.read(&value, sizeof(value));
      compound[key.c_str()] = static_cast<int64_t>(hostToNetwork64(value));
      break;
    }
    case Type::FLOAT: {
      uint32_t floatData;
      reader.read(&floatData, sizeof(floatData));
      floatData = hostToNetwork32(floatData);
      compound[key.c_str()] = *reinterpret_cast<float*>(&floatData);
      break;
    }
    case Type::DOUBLE: {
      uint64_t doubleData;
      reader.read(&doubleData, sizeof(doubleData));
      doubleData = hostToNetwork64(doubleData);

      compound[key.c_str()] = *reinterpret_cast<double*>(&doubleData);
      break;
    }
    case Type::BYTE_ARRAY: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      auto arrayLength = static_cast<int32_t>(hostToNetwork32(value));

      std::vector<int8_t> buffer(arrayLength);
      memcpy(buffer.data(), reader.getCurrentPosition(), sizeof(arrayLength));
      reader.advance(arrayLength);
      compound[key.c_str()] = std::move(buffer);
      break;
    }
    case Type::STRING: {
      uint16_t arrayLength;
      reader.read(&arrayLength, sizeof(arrayLength));
      stringLength = hostToNetwork16(arrayLength);

      std::string string(reader.getCurrentPosition(), arrayLength);
      reader.advance(stringLength);
      compound[key.c_str()] = std::move(string);
      break;
    }
    case Type::LIST: {

    }
    case Type::COMPOUND: {
      Compound elementCompound;

      int8_t nodeType = *reader.getCurrentPosition();
      while (nodeType != 0) {
        readInto(elementCompound, reader);
        nodeType = *reader.getCurrentPosition();
      }
      break;
    }
    case Type::INT_ARRAY: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      auto arrayLength = static_cast<int32_t>(hostToNetwork32(value));

      std::vector<int32_t> buffer(arrayLength);
      for (int32_t i = 0; i < arrayLength; i++) {
        uint32_t elementValue;
        reader.read(&elementValue, sizeof(elementValue));
        buffer[i] = static_cast<int32_t>(hostToNetwork32(elementValue));
      }

      compound[key.c_str()] = std::move(buffer);
      break;
    }
    case Type::LONG_ARRAY: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      auto arrayLength = static_cast<int32_t>(hostToNetwork32(value));

      std::vector<int64_t> buffer(arrayLength);
      for (int32_t i = 0; i < arrayLength; i++) {
        uint64_t elementValue;
        reader.read(&elementValue, sizeof(elementValue));
        buffer[i] = static_cast<int64_t>(hostToNetwork64(elementValue));
      }

      compound[key.c_str()] = std::move(buffer);
      break;
    }
  }
}

Compound Reader::parse(const void* data, size_t length, bool inlineRootTag) {
  Compound compound;

  ByteReader reader(data, length);
  while (!reader.eof()) {
    readInto(compound, reader);
  }

  if (inlineRootTag) {
    if (compound.hasKey("")) return std::move(compound[""].getCompound());
  }

  return std::move(compound);
}

}