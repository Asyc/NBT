#ifndef NBT_SRC_MODIFIED_UTF_HPP_
#define NBT_SRC_MODIFIED_UTF_HPP_

#include <istream>
#include <ostream>

#include "byteswap.hpp"
#include "primitive.hpp"

namespace nbt::utf {

inline void writeUTF(std::ostream& out, const std::string_view& string) {
  auto strlen = static_cast<uint16_t>(string.length());

  int utflen = 0;
  int c, count = 0;

  /* use charAt instead of copying String to char array */
  for (int i = 0; i < strlen; i++) {
    c = static_cast<uint8_t>(string[i]);
    if ((c >= 0x0001) && (c <= 0x007F)) {
      utflen++;
    } else if (c > 0x07FF) {
      utflen += 3;
    } else {
      utflen += 2;
    }
  }

  if (utflen > 65535) throw std::runtime_error("encoded string too long");
  Primitive<uint16_t>::writeTo(out, utflen);

  auto bytearr = std::make_unique<char[]>(utflen);

  int i=0;
  for (i=0; i<strlen; i++) {
    c = static_cast<uint8_t>(string[i]);
    if (!((c >= 0x0001) && (c <= 0x007F))) break;
    bytearr[count++] = static_cast<char>(c);
  }

  for (;i < strlen; i++){
    c = static_cast<uint8_t>(string[i]);
    if ((c >= 0x0001) && (c <= 0x007F)) {
      bytearr[count++] = static_cast<char>(c);

    } else if (c > 0x07FF) {
      bytearr[count++] = static_cast<char>((0xE0 | (( c >> 12) & 0x0F)));
      bytearr[count++] = static_cast<char>((0x80 | ((c >>  6) & 0x3F)));
      bytearr[count++] = static_cast<char>((0x80 | ((c >>  0) & 0x3F)));
    } else {
      bytearr[count++] = static_cast<char>((0xC0 | ((c >>  6) & 0x1F)));
      bytearr[count++] = static_cast<char>((0x80 | ((c >>  0) & 0x3F)));
    }
  }
  out.write(bytearr.get(), utflen);
}

inline size_t getByteLength(const std::string_view& string) {
  auto strlen = static_cast<uint16_t>(string.length());

  int utflen = 0;
  int c, count = 0;

  for (int i = 0; i < strlen; i++) {
    c = static_cast<uint8_t>(string[i]);
    if ((c >= 0x0001) && (c <= 0x007F)) {
      utflen++;
    } else if (c > 0x07FF) {
      utflen += 3;
    } else {
      utflen += 2;
    }
  }

  return utflen + Primitive<uint16_t>::getSize();
}

inline std::string readUTF(std::istream& in) {
  uint16_t utflen;
  in.read(reinterpret_cast<char*>(&utflen), sizeof(utflen));
  utflen = hostToNetwork16(utflen);

  auto allocation = std::make_unique<char[]>(utflen * 2 + 1);

  char* buffer = allocation.get();
  char* output = allocation.get() + utflen;

  in.read(buffer, utflen);

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

  return {output, static_cast<size_t>(chararr_count)};
}

} // namespace nbt

#endif //NBT_SRC_MODIFIED_UTF_HPP_
