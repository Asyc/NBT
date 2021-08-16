#include "nbt/nbt_type.hpp"

#include <stdexcept>

namespace nbt {

template<Type TYPE>
inline void typeCheck(Type type) {
  if (type != TYPE) {
    throw std::runtime_error("object type does not match requested type");
  }
}

Value::Value(Value&& rhs) noexcept : m_Type(Type::BYTE) {
  operator=(std::move(rhs));
}

Value::Value(const Value& rhs) noexcept : m_Type(Type::BYTE) {
  operator=(rhs);
}

Value& Value::operator=(const Value& rhs) noexcept {
  if (this == &rhs) return *this;
  switch (rhs.m_Type) {
    case Type::BYTE:operator=(rhs.m_Byte);
    break;
    case Type::SHORT:operator=(rhs.m_Short);
    break;
    case Type::INT:operator=(rhs.m_Int);
    break;
    case Type::LONG:operator=(rhs.m_Long);
    break;
    case Type::FLOAT:operator=(rhs.m_Float);
    break;
    case Type::DOUBLE:operator=(rhs.m_Double);
    break;
    case Type::BYTE_ARRAY:operator=(rhs.m_ByteArray);
    break;
    case Type::STRING:operator=(rhs.m_String);
    break;
    case Type::LIST:operator=(rhs.m_List);
    break;
    case Type::COMPOUND:operator=(rhs.m_Compound);
    break;
    case Type::INT_ARRAY:operator=(rhs.m_IntArray);
    break;
    case Type::LONG_ARRAY:operator=(rhs.m_LongArray);
    break;
  }
  return *this;
}

Value& Value::operator=(Value&& rhs) noexcept {
  if (this == &rhs) return *this;
  switch (rhs.m_Type) {
    case Type::BYTE:operator=(rhs.m_Byte);
    break;
    case Type::SHORT:operator=(rhs.m_Short);
    break;
    case Type::INT:operator=(rhs.m_Int);
    break;
    case Type::LONG:operator=(rhs.m_Long);
    break;
    case Type::FLOAT:operator=(rhs.m_Float);
    break;
    case Type::DOUBLE:operator=(rhs.m_Double);
    break;
    case Type::BYTE_ARRAY:operator=(std::move(rhs.m_ByteArray));
    break;
    case Type::STRING:operator=(std::move(rhs.m_String));
    break;
    case Type::LIST:operator=(std::move(rhs.m_List));
    break;
    case Type::COMPOUND:operator=(std::move(rhs.m_Compound));
    break;
    case Type::INT_ARRAY:operator=(std::move(rhs.m_IntArray));
    break;
    case Type::LONG_ARRAY:operator=(std::move(rhs.m_LongArray));
    break;
  }
  return *this;
}

Value::Value(int8_t value) {
  operator=(value);
}

Value::Value(int16_t value) {
  operator=(value);
}

Value::Value(int32_t value) {
  operator=(value);
}

Value::Value(int64_t value) {
  operator=(value);
}

Value::Value(float value) {
  operator=(value);
}

Value::Value(double value) {
  operator=(value);
}

Value::Value(std::vector<int8_t> value) {
  operator=(std::move(value));
}

Value::Value(std::vector<int32_t> value) {
  operator=(std::move(value));
}

Value::Value(std::vector<int64_t> value) {
  operator=(std::move(value));
}

Value::Value(std::string string) {
  operator=(std::move(string));
}

Value::Value(Compound value) {
  operator=(std::move(value));
}

Value::Value(List value) {
  operator=(std::move(value));
}

Value::~Value() {
  setType(Type::BYTE);  // Destructs complex type
}

Value& Value::operator=(int8_t value) {
  setType(Type::BYTE);
  m_Byte = value;
  return *this;
}

Value& Value::operator=(int16_t value) {
  setType(Type::SHORT);
  m_Short = value;
  return *this;
}

Value& Value::operator=(int32_t value) {
  setType(Type::INT);
  m_Int = value;
  return *this;
}

Value& Value::operator=(int64_t value) {
  setType(Type::LONG);
  m_Long = value;
  return *this;
}

Value& Value::operator=(float value) {
  setType(Type::FLOAT);
  m_Float = value;
  return *this;
}

Value& Value::operator=(double value) {
  setType(Type::DOUBLE);
  m_Double = value;
  return *this;
}

Value& Value::operator=(std::vector<int8_t> value) {
  setType(Type::BYTE_ARRAY);
  new(&m_ByteArray) std::vector<int8_t>(std::move(value));
  return *this;
}

Value& Value::operator=(std::vector<int32_t> value) {
  setType(Type::INT);
  new(&m_IntArray) std::vector<int32_t>(std::move(value));
  return *this;
}

Value& Value::operator=(std::vector<int64_t> value) {
  setType(Type::LONG);
  new(&m_LongArray) std::vector<int64_t>(std::move(value));
  return *this;
}

Value& Value::operator=(std::string string) {
  setType(Type::STRING);
  new(&m_String) std::string(std::move(string));
  return *this;
}

Value& Value::operator=(Compound value) {
  setType(Type::COMPOUND);
  new(&m_Compound) Compound(std::move(value));
  return *this;
}

Value& Value::operator=(List value) {
  setType(Type::LIST);
  new(&m_List) List(std::move(value));
  return *this;
}

void Value::setType(Type type) {
  switch (m_Type) {
    case Type::BYTE_ARRAY:m_ByteArray.~vector();
      break;
    case Type::STRING:m_String.~basic_string();
      break;
    case Type::LIST:m_List.~List();
      break;
    case Type::COMPOUND:m_Compound.~Compound();
      break;
    case Type::INT_ARRAY:m_IntArray.~vector();
      break;
    case Type::LONG_ARRAY:m_LongArray.~vector();
      break;
    default:break;
  }

  m_Type = type;
}

Type Value::getType() const {
  return m_Type;
}

int8_t Value::getByte() const {
  typeCheck<Type::BYTE>(m_Type);
  return m_Byte;
}

int16_t Value::getShort() const {
  typeCheck<Type::SHORT>(m_Type);
  return m_Short;
}

int32_t Value::getInt() const {
  typeCheck<Type::INT>(m_Type);
  return m_Int;
}

int64_t Value::getLong() const {
  typeCheck<Type::LONG>(m_Type);
  return m_Long;
}

float Value::getFloat() const {
  typeCheck<Type::FLOAT>(m_Type);
  return m_Float;
}

double Value::getDouble() const {
  typeCheck<Type::DOUBLE>(m_Type);
  return m_Double;
}

std::vector<int8_t>& Value::getByteArray() {
  typeCheck<Type::BYTE_ARRAY>(m_Type);
  return m_ByteArray;
}

const std::vector<int8_t>& Value::getByteArray() const {
  typeCheck<Type::BYTE_ARRAY>(m_Type);
  return m_ByteArray;
}

std::vector<int32_t>& Value::getIntArray() {
  typeCheck<Type::INT_ARRAY>(m_Type);
  return m_IntArray;
}

const std::vector<int32_t>& Value::getIntArray() const {
  typeCheck<Type::INT_ARRAY>(m_Type);
  return m_IntArray;
}

std::vector<int64_t>& Value::getLongArray() {
  typeCheck<Type::LONG_ARRAY>(m_Type);
  return m_LongArray;
}

const std::vector<int64_t>& Value::getLongArray() const {
  typeCheck<Type::LONG_ARRAY>(m_Type);
  return m_LongArray;
}

std::string& Value::getString() {
  typeCheck<Type::STRING>(m_Type);
  return m_String;
}

const std::string& Value::getString() const {
  typeCheck<Type::STRING>(m_Type);
  return m_String;
}

Compound& Value::getCompound() {
  typeCheck<Type::COMPOUND>(m_Type);
  return m_Compound;
}

const Compound& Value::getCompound() const {
  typeCheck<Type::COMPOUND>(m_Type);
  return m_Compound;
}

List& Value::getList() {
  typeCheck<Type::LIST>(m_Type);
  return m_List;
}

const List& Value::getList() const {
  typeCheck<Type::LIST>(m_Type);
  return m_List;
}

Value& Compound::operator[](const char* key) {
  return m_Values[key];
}

bool Compound::hasKey(const char* key) const {
  return m_Values.find(key) != m_Values.end();
}

bool Compound::remove(const char* key) {
  auto it = m_Values.find(key);
  if (it == m_Values.end()) return false;
  m_Values.erase(it);
  return true;
}

Value& List::operator[](size_t index) {
  return m_Values[index];
}

const Value& List::operator[](size_t index) const {
  return m_Values[index];
}

void List::pushBack(Value value) {
  m_Values.emplace_back(std::move(value));
}

List::Iterator List::begin() {
  return m_Values.begin();
}

List::ConstIterator List::begin() const {
  return m_Values.begin();
}

List::Iterator List::end() {
  return m_Values.end();
}

List::ConstIterator List::end() const {
  return m_Values.end();
}

size_t List::size() const {
  return m_Values.size();
}

}