#ifndef NBT_SRC_PRIMITIVE_HPP_
#define NBT_SRC_PRIMITIVE_HPP_

#include <cstddef>
#include <functional>
#include <istream>
#include <ostream>

namespace nbt {

template<typename T>
class Primitive {
 public:
  inline static void writeTo(std::ostream& out, T value);
  inline static T readFrom(std::istream& in);

  constexpr static size_t getSize() {
    return sizeof(T);
  }
};

template<typename T>
class Array {
 public:
  inline static void writeTo(std::ostream& out, const T* values, size_t length);
  inline static std::vector<T> readFrom(std::istream& in);
};

template<>
inline void Primitive<int8_t>::writeTo(std::ostream& out, int8_t value) {
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<>
inline int8_t Primitive<int8_t>::readFrom(std::istream& in) {
  int8_t value;
  in.read(reinterpret_cast<char*>(&value), getSize());
  return value;
}

template<>
inline void Primitive<int16_t>::writeTo(std::ostream& out, int16_t value) {
  value = hostToNetwork16(value);
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<>
inline int16_t Primitive<int16_t>::readFrom(std::istream& in) {
  uint16_t value;
  in.read(reinterpret_cast<char*>(&value), getSize());
  return static_cast<int16_t>(hostToNetwork16(value));
}

template<>
inline void Primitive<int32_t>::writeTo(std::ostream& out, int32_t value) {
  value = hostToNetwork32(value);
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<>
inline int32_t Primitive<int32_t>::readFrom(std::istream& in) {
  uint32_t value;
  in.read(reinterpret_cast<char*>(&value), getSize());
  return static_cast<int32_t>(hostToNetwork32(value));
}

template<>
inline void Primitive<int64_t>::writeTo(std::ostream& out, int64_t value) {
  value = hostToNetwork64(value);
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<>
inline int64_t Primitive<int64_t>::readFrom(std::istream& in) {
  uint64_t value;
  in.read(reinterpret_cast<char*>(&value), getSize());
  return static_cast<int64_t>(hostToNetwork64(value));}

template<>
inline void Primitive<float>::writeTo(std::ostream& out, float value) {
  uint32_t intBits = hostToNetwork32(*reinterpret_cast<const uint32_t*>(&value));
  out.write(reinterpret_cast<const char*>(&intBits), sizeof(intBits));
}

template<>
inline float Primitive<float>::readFrom(std::istream& in) {
  uint32_t intBits;
  in.read(reinterpret_cast<char*>(&intBits), getSize());
  intBits = hostToNetwork32(intBits);
  return *reinterpret_cast<const float*>(&intBits);
}

template<>
inline void Primitive<double>::writeTo(std::ostream& out, double value) {
  uint64_t intBits = hostToNetwork64(*reinterpret_cast<const uint64_t*>(&value));
  out.write(reinterpret_cast<const char*>(&intBits), sizeof(intBits));
}

template<>
inline double Primitive<double>::readFrom(std::istream& in) {
  uint64_t intBits;
  in.read(reinterpret_cast<char*>(&intBits), getSize());
  intBits = hostToNetwork64(intBits);
  return *reinterpret_cast<const double*>(&intBits);
}

template<> inline void Primitive<uint16_t>::writeTo(std::ostream& out, uint16_t value) {
  value = hostToNetwork16(*reinterpret_cast<const uint16_t*>(&value));
  out.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template<>
inline uint16_t Primitive<uint16_t>::readFrom(std::istream& in) {
  uint16_t value;
  in.read(reinterpret_cast<char*>(&value), getSize());
  return hostToNetwork16(value);
}

template<>
inline void Array<int8_t>::writeTo(std::ostream& out, const int8_t* values, size_t length) {
  Primitive<int32_t>::writeTo(out, static_cast<int32_t>(length));
  out.write(reinterpret_cast<const char*>(values), static_cast<std::streamsize>(length));
}

template<>
inline std::vector<int8_t> Array<int8_t>::readFrom(std::istream& in) {
  int32_t size = Primitive<int32_t>::readFrom(in);
  std::vector<int8_t> vector(size);
  in.read(reinterpret_cast<char*>(vector.data()), static_cast<std::streamsize>(vector.size()));
  return std::move(vector);
}

template<>
inline void Array<int32_t>::writeTo(std::ostream& out, const int* values, size_t length) {
  Primitive<int32_t>::writeTo(out, static_cast<int32_t>(length));

  for (size_t i = 0; i < length; i++) {
    Primitive<int32_t>::writeTo(out, values[i]);
  }
}

template<>
inline std::vector<int32_t> Array<int32_t>::readFrom(std::istream& in) {
  int32_t size = Primitive<int32_t>::readFrom(in);
  std::vector<int32_t> vector(size);
  for (int & i : vector) {
    i = Primitive<int32_t>::readFrom(in);
  }
  return std::move(vector);
}

template<>
inline void Array<int64_t>::writeTo(std::ostream& out, const int64_t* values, size_t length) {
  Primitive<int64_t>::writeTo(out, static_cast<int32_t>(length));

  for (size_t i = 0; i < length; i++) {
    Primitive<int64_t>::writeTo(out, values[i]);
  }
}

template<>
inline std::vector<int64_t> Array<int64_t>::readFrom(std::istream& in) {
  int32_t size = Primitive<int32_t>::readFrom(in);
  std::vector<int64_t> vector(size);
  for (int64_t& i : vector) {
    i = Primitive<int64_t>::readFrom(in);
  }
  return std::move(vector);
}

}

#endif //NBT_SRC_PRIMITIVE_HPP_
