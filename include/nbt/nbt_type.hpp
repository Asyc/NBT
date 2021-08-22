#ifndef NBT_INCLUDE_NBT_NBT_TYPE_HPP_
#define NBT_INCLUDE_NBT_NBT_TYPE_HPP_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace nbt {

using ByteArray = std::vector<int8_t>;
using IntArray = std::vector<int16_t>;
using LongArray = std::vector<int32_t>;
using String = std::string;

enum class Type : int8_t {
  BYTE = 1,
  SHORT = 2,
  INT = 3,
  LONG = 4,
  FLOAT = 5,
  DOUBLE = 6,
  BYTE_ARRAY = 7,
  STRING = 8,
  LIST = 9,
  COMPOUND = 10,
  INT_ARRAY = 11,
  LONG_ARRAY = 12
};

class Value;

class Compound {
 public:
  using Iterator = std::unordered_map<std::string, Value>::iterator;
  using ConstIterator = std::unordered_map<std::string, Value>::const_iterator;

  Value& operator[](const char*);
  const Value* operator[](const char*) const;

  void insert(std::string key, Value value);

  bool hasKey(const char*) const;
  bool remove(const char*);

  Iterator begin();
  [[nodiscard]] ConstIterator begin() const;

  Iterator end();
  [[nodiscard]] ConstIterator end() const;

  [[nodiscard]] size_t size() const;
 private:
  std::unordered_map<std::string, Value> m_Values;
};

class List {
 public:
  using Iterator = std::vector<Value>::iterator;
  using ConstIterator = std::vector<Value>::const_iterator;

  List() {}
  List(Type);

  Value& operator[](size_t index);
  const Value& operator[](size_t index) const;

  void pushBack(Value value);

  void setType(Type type);

  template <typename T>
  void emplaceBack(T&& value) {
    m_Values.emplace_back(std::forward<T>(value));
  }

  Iterator begin();
  [[nodiscard]] ConstIterator begin() const;

  Iterator end();
  [[nodiscard]] ConstIterator end() const;

  [[nodiscard]] Type getType() const;
  [[nodiscard]] size_t size() const;
 private:
  Type m_Type;
  std::vector<Value> m_Values;
};

class Value {
 public:
  Value();
  Value(Value&& rhs) noexcept;
  Value(const Value& rhs) noexcept;

  Value(int8_t);
  Value(int16_t);
  Value(int32_t);
  Value(int64_t);
  Value(float);
  Value(double);
  Value(std::vector<int8_t>);
  Value(std::vector<int32_t>);
  Value(std::vector<int64_t>);
  Value(std::string string);
  Value(Compound compound);
  Value(List list);
  ~Value();

  Value& operator=(const Value& rhs) noexcept;
  Value& operator=(Value&& rhs) noexcept;

  Value& operator=(int8_t);
  Value& operator=(int16_t);
  Value& operator=(int32_t);
  Value& operator=(int64_t);

  Value& operator=(float);
  Value& operator=(double);

  Value& operator=(std::vector<int8_t>);
  Value& operator=(std::vector<int32_t>);
  Value& operator=(std::vector<int64_t>);

  Value& operator=(std::string string);
  Value& operator=(Compound compound);
  Value& operator=(List list);

  template <typename T>
  void set(T value) {
    operator=(std::move(value));
  }

  [[nodiscard]] Type getType() const;

  [[nodiscard]] int8_t getByte() const;
  [[nodiscard]] int16_t getShort() const;
  [[nodiscard]] int32_t getInt() const;
  [[nodiscard]] int64_t getLong() const;

  [[nodiscard]] float getFloat() const;
  [[nodiscard]] double getDouble() const;

  [[nodiscard]] std::vector<int8_t>& getByteArray();
  [[nodiscard]] const std::vector<int8_t>& getByteArray() const;

  [[nodiscard]] std::vector<int32_t>& getIntArray();
  [[nodiscard]] const std::vector<int32_t>& getIntArray() const;

  [[nodiscard]] std::vector<int64_t>& getLongArray();
  [[nodiscard]] const std::vector<int64_t>& getLongArray() const;

  [[nodiscard]] std::string& getString();
  [[nodiscard]] const std::string& getString() const;

  [[nodiscard]] Compound& getCompound();
  [[nodiscard]] const Compound& getCompound() const;

  [[nodiscard]] List& getList();
  [[nodiscard]] const List& getList() const;
 private:
  void setType(Type type);

  Type m_Type;

  union {
    int8_t m_Byte;
    int16_t m_Short;
    int32_t m_Int;
    int64_t m_Long;

    float m_Float;
    double m_Double;

    std::vector<int8_t> m_ByteArray;
    std::vector<int32_t> m_IntArray;
    std::vector<int64_t> m_LongArray;

    std::string m_String;
    Compound m_Compound;
    List m_List;
  };
};

}

#endif //NBT_INCLUDE_NBT_NBT_TYPE_HPP_
