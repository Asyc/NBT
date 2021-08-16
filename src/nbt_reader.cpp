#include "nbt/nbt_reader.hpp"

#include <stdexcept>

#include "byteswap.hpp"

namespace nbt {

class ByteReader {
 public:
  ByteReader(const void* data, size_t length) : m_Data(reinterpret_cast<const char*>(data)), m_Length(length), m_Index(0) {}
  void read(void* dst, size_t length) {
    if (m_Index + length > m_Length) {
      throw std::runtime_error("index out of bounds");
    }

    memcpy(dst, m_Data + m_Index, length);
    m_Index += length;
  }

  void advance(size_t x) { m_Index += x; }

  [[nodiscard]] bool eof() const { return m_Index + 1 == m_Length; }

  [[nodiscard]] const char* getCurrentPosition() const { return m_Data + m_Index; }
 private:
  const char* m_Data;
  size_t m_Index;
  size_t m_Length;
};

void readInto(Compound& compound, ByteReader& reader);

std::string readUTF(ByteReader& reader) {
  uint16_t utflen;
  reader.read(&utflen, sizeof(utflen));
  utflen = hostToNetwork16(utflen);

  const char* buffer = reader.getCurrentPosition();
  reader.advance(utflen);
  std::string output(utflen + 1, '\0');

  int c, char2, char3;
  int count = 0;
  int chararr_count = 0;

  while (count < utflen) {
    c = (int) buffer[count] & 0xff;
    if (c > 127) break;
    count++;
    output[chararr_count++] = (char) c;
  }

  while (count < utflen) {
    c = (int) buffer[count] & 0xff;
    switch (c >> 4) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:count++;
        output[chararr_count++] = (char) c;
        break;
      case 12:
      case 13:count += 2;
        if (count > utflen)
          throw std::runtime_error("malformed input: partial character at end");
        char2 = (int) static_cast<uint8_t>(buffer[count - 1]);
        if ((char2 & 0xC0) != 0x80)
          throw std::runtime_error("malformed input around byte " + std::to_string(count));
        output[chararr_count++] = (char) (((c & 0x1F) << 6) |
            (char2 & 0x3F));
        break;
      case 14:count += 3;
        if (count > utflen)
          throw std::runtime_error("malformed input: partial character at end");
        char2 = (int) static_cast<uint8_t>(buffer[count - 2]);
        char3 = (int) static_cast<uint8_t>(buffer[count - 1]);
        if (((char2 & 0xC0) != 0x80) || ((char3 & 0xC0) != 0x80)) throw std::runtime_error("malformed input around byte " + std::to_string(count - 1));
        output[chararr_count++] = (char) (((c & 0x0F) << 12) |
            ((char2 & 0x3F) << 6) |
            ((char3 & 0x3F) << 0));
        break;
      default:throw std::runtime_error("malformed input around byte " + std::to_string(count));
    }
  }

  output[output.size() - 1] = '\0';
  return std::move(output);
}

Value readValue(Type type, ByteReader& reader) {
  Value nbtValue;

  switch (type) {
    case Type::BYTE: {
      uint8_t value;
      reader.read(&value, sizeof(value));
      nbtValue = static_cast<int8_t>(value);
      break;
    }
    case Type::SHORT: {
      uint16_t value;
      reader.read(&value, sizeof(value));
      nbtValue = static_cast<int16_t>(hostToNetwork16(value));
      break;
    }
    case Type::INT: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      nbtValue = static_cast<int32_t>(hostToNetwork32(value));
      break;
    }
    case Type::LONG: {
      uint64_t value;
      reader.read(&value, sizeof(value));
      nbtValue = static_cast<int64_t>(hostToNetwork64(value));
      break;
    }
    case Type::FLOAT: {
      uint32_t floatData;
      reader.read(&floatData, sizeof(floatData));
      floatData = hostToNetwork32(floatData);
      nbtValue = *reinterpret_cast<float*>(&floatData);
      break;
    }
    case Type::DOUBLE: {
      uint64_t doubleData;
      reader.read(&doubleData, sizeof(doubleData));
      doubleData = hostToNetwork64(doubleData);

      nbtValue = *reinterpret_cast<double*>(&doubleData);
      break;
    }
    case Type::BYTE_ARRAY: {
      uint32_t value;
      reader.read(&value, sizeof(value));
      auto arrayLength = static_cast<int32_t>(hostToNetwork32(value));

      std::vector<int8_t> buffer(arrayLength);
      memcpy(buffer.data(), reader.getCurrentPosition(), arrayLength * sizeof(int8_t));
      reader.advance(arrayLength);
      nbtValue = std::move(buffer);
      break;
    }
    case Type::STRING: {
      nbtValue = readUTF(reader);
      break;
    }
    case Type::LIST: {
      Type listType;
      reader.read(&listType, sizeof(listType));

      uint32_t value;
      reader.read(&value, sizeof(value));
      auto arrayLength = static_cast<int32_t>(hostToNetwork32(value));

      std::vector<int32_t> buffer(arrayLength);
      List list;
      for (int32_t i = 0; i < arrayLength; i++) {
        list.pushBack(readValue(listType, reader));
      }

      nbtValue = std::move(list);
      break;
    }
    case Type::COMPOUND: {
      Compound elementCompound;
      readInto(elementCompound, reader);
      nbtValue = std::move(elementCompound);
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

      nbtValue = std::move(buffer);
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

      nbtValue = std::move(buffer);
      break;
    }
  }

  return std::move(nbtValue);
}

void readInto(Compound& compound, ByteReader& reader) {
  int8_t type;
  reader.read(&type, sizeof(type));

  while (type != 0) {
    uint16_t stringLength;
    reader.read(&stringLength, sizeof(stringLength));
    stringLength = hostToNetwork16(stringLength);

    std::string key(reader.getCurrentPosition(), stringLength);
    reader.advance(stringLength);

    compound[key.c_str()] = readValue(static_cast<Type>(type), reader);
    if (reader.eof()) break;
    reader.read(&type, sizeof(type));
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