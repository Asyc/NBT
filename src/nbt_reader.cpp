#include "nbt/nbt_reader.hpp"

#include <stdexcept>

#include "byteswap.hpp"
#include "primitive.hpp"
#include "modified_utf.hpp"

#include <sstream>

namespace nbt {

 class PointerInputBuffer : public std::basic_streambuf<char> {
  public:
   PointerInputBuffer(const char* data, size_t length) {
     char* dataBuffer = const_cast<char*>(data);
     setg(dataBuffer, dataBuffer, dataBuffer + length);
   }
 };

void readNextPair(Compound& compound, std::istream& in);

Compound Reader::parse(const void* data, size_t length) {
  PointerInputBuffer streamBuf(reinterpret_cast<const char*>(data), length);
  std::istream in(&streamBuf);
  return parse(in);
}

Compound Reader::parse(std::istream& in) {
  Compound compound;
  while (!in.eof()) {
    int peek = in.peek();
    if (peek == EOF) break;
    if (peek == 0) break; // TAG_End

    readNextPair(compound, in);
  }
  return std::move(compound);
}

std::istream& operator>>(std::istream& in, Type& type) {
  in.read(reinterpret_cast<char*>(&type), sizeof(Type));
  return in;
}

std::istream& operator>>(std::istream& in, std::string& string) {
  auto stringLength = Primitive<uint16_t>::readFrom(in);
  string.resize(stringLength);
  in.read(string.data(), stringLength);
  return in;
}

Value readValue(Type type, std::istream& in) {
  Value nbtValue;

  switch (type) {
    case Type::BYTE: {
      nbtValue = Primitive<int8_t>::readFrom(in);
      break;
    }
    case Type::SHORT: {
      nbtValue = Primitive<int16_t>::readFrom(in);
      break;
    }
    case Type::INT: {
      nbtValue = Primitive<int32_t>::readFrom(in);
      break;
    }
    case Type::LONG: {
      nbtValue = Primitive<int64_t>::readFrom(in);
      break;
    }
    case Type::FLOAT: {
      nbtValue = Primitive<float>::readFrom(in);
      break;
    }
    case Type::DOUBLE: {
      nbtValue = Primitive<double>::readFrom(in);
      break;
    }
    case Type::BYTE_ARRAY: {
      nbtValue = Array<int8_t>::readFrom(in);
      break;
    }
    case Type::INT_ARRAY: {
      nbtValue = Array<int32_t>::readFrom(in);
      break;
    }
    case Type::LONG_ARRAY: {
      nbtValue = Array<int64_t>::readFrom(in);
      break;
    }
    case Type::STRING: {
      nbtValue = utf::readUTF(in);
      break;
    }
    case Type::LIST: {
      Type listType;
      in >> listType;
      if (static_cast<Type>(0) == listType) {
        nbtValue = List();
        break;
      }

      auto arrayLength = Primitive<int32_t>::readFrom(in);

      List list(listType);

      for (int32_t i = 0; i < arrayLength; i++) {
        list.pushBack(readValue(listType, in));
      }

      nbtValue = std::move(list);
      break;
    }
    case Type::COMPOUND: {
      Compound elementCompound;
      int32_t peek = in.peek();
      while (peek != 0) {
        readNextPair(elementCompound, in);
        peek = in.peek();
      }
      in.ignore(1);

      nbtValue = std::move(elementCompound);
      break;
    }
    default:
      throw std::runtime_error("invalid nbt type");
  }

  return std::move(nbtValue);
}

void readNextPair(Compound& compound, std::istream& in) {
  Type type;
  in >> type;

  std::string key;
  in >> key;

  Value value = readValue(type, in);
  compound.insert(std::move(key), std::move(value));
}

}