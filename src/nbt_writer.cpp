#include "nbt/nbt_writer.hpp"

#include "byteswap.hpp"
#include "primitive.hpp"
#include "modified_utf.hpp"

namespace nbt {

std::ostream& operator<<(std::ostream& out, Type type);
std::ostream& operator<<(std::ostream& out, const std::string_view& string);
std::ostream& operator<<(std::ostream& out, const Compound& compound);
std::ostream& operator<<(std::ostream& out, const List& list);
std::ostream& operator<<(std::ostream& out, const Value& value);

size_t getCompoundSize(const Compound& compound);
size_t getListSize(const List& list);
size_t getValueSize(const Value& value);

void Writer::serializeTo(std::ostream& out, const Compound& compound, const std::string_view& name) {
  out << Type::COMPOUND;
  out << name;
  out << compound;
}

std::vector<char> Writer::serialize(const Compound& compound) {
  std::vector<char> buffer(getCompoundSize(compound));
  return std::move(buffer);
}

std::ostream& operator<<(std::ostream& out, Type type) {
  out.write(reinterpret_cast<const char*>(&type), sizeof(type));
  return out;
}

std::ostream& operator<<(std::ostream& out, const std::string_view& string) {
  Primitive<uint16_t>::writeTo(out, static_cast<uint16_t>(string.length()));
  out.write(string.data(), static_cast<std::streamsize>(string.length()));
  return out;
}

std::ostream& operator<<(std::ostream& out, const Compound& compound) {
  for (const auto& pair : compound) {
    if (pair.second.getType() == static_cast<Type>(0)) continue;  // NULL-Pair
    out << pair.second.getType();
    out << std::string_view(pair.first);
    out << pair.second;
  }

  Type type = static_cast<Type>(0); //TAG_END
  out << type;

  return out;
}

std::ostream& operator<<(std::ostream& out, const List& list) {
  out << list.getType();
  Primitive<int32_t>::writeTo(out, static_cast<int32_t>(list.size()));
  for (const auto& element : list) {
    out << element;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Value& value) {
  switch (value.getType()) {
    case Type::BYTE:
      Primitive<int8_t>::writeTo(out, value.getByte());
      break;
    case Type::SHORT:
      Primitive<int16_t>::writeTo(out, value.getShort());
      break;
    case Type::INT:
      Primitive<int32_t>::writeTo(out, value.getInt());
      break;
    case Type::LONG:
      Primitive<int64_t>::writeTo(out, value.getLong());
      break;
    case Type::FLOAT:
      Primitive<float>::writeTo(out, value.getFloat());
      break;
    case Type::DOUBLE:
      Primitive<double>::writeTo(out, value.getDouble());
      break;
    case Type::BYTE_ARRAY:
      Array<int8_t>::writeTo(out, value.getByteArray().data(), value.getByteArray().size());
      break;
    case Type::STRING:
      utf::writeUTF(out, value.getString());
      break;
    case Type::LIST:
      out << value.getList();
      break;
    case Type::COMPOUND:
      out << value.getCompound();
      break;
    case Type::INT_ARRAY:
      Array<int32_t>::writeTo(out, value.getIntArray().data(), value.getIntArray().size());
      break;
    case Type::LONG_ARRAY:
      Array<int64_t>::writeTo(out, value.getLongArray().data(), value.getLongArray().size());
      break;
  }
  return out;
}

size_t getValueSize(const Value& value) {
  switch (value.getType()) {
    case Type::BYTE: return Primitive<int8_t>::getSize();
    case Type::SHORT: return Primitive<int16_t>::getSize();
    case Type::INT: return Primitive<int32_t>::getSize();
    case Type::LONG: return Primitive<int64_t>::getSize();
    case Type::FLOAT: return Primitive<float>::getSize();
    case Type::DOUBLE: return Primitive<double>::getSize();
    case Type::BYTE_ARRAY: return value.getByteArray().size() * Primitive<int8_t>::getSize();
    case Type::INT_ARRAY: return value.getIntArray().size() * Primitive<int32_t>::getSize();
    case Type::LONG_ARRAY: return value.getLongArray().size() * Primitive<int64_t>::getSize();
    case Type::STRING: return utf::getByteLength(value.getString());
    case Type::LIST: return getListSize(value.getList());
    case Type::COMPOUND: return getCompoundSize(value.getCompound());
  }
}

size_t getCompoundSize(const Compound& compound) {
  size_t totalSize = 0;
  for (const auto& pair : compound) {
    totalSize += getValueSize(pair.second);
  }

  return totalSize;
}

size_t getListSize(const List& list) {
  size_t totalSize = 0;
  for (const auto& element : list) {
    totalSize += getValueSize(element);
  }

  return totalSize;
}

}