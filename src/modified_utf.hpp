#ifndef NBT_SRC_MODIFIED_UTF_HPP_
#define NBT_SRC_MODIFIED_UTF_HPP_

#include <istream>
#include <ostream>

#include "byteswap.hpp"

namespace nbt::utf {

inline void writeUTF(std::ostream& out, const std::string_view& string) {

}

inline size_t getByteLength(const std::string_view& string) {
  size_t utflen = 0;
  for (char character : string) {
    int c = static_cast<uint16_t>(character);
    if ((c >= 0x0001) && (c <= 0x007F)) {
      utflen++;
    } else if (c > 0x07FF) {
      utflen += 3;
    } else {
      utflen += 2;
    }
  }

  return utflen + sizeof(uint16_t);
}

inline std::string readUTF(std::istream& in) {
  uint16_t utflen;
  in.read(reinterpret_cast<char*>(&utflen), sizeof(utflen));
  utflen = hostToNetwork16(utflen);

  auto buffer = std::make_unique<char[]>(utflen);
  in.read(buffer.get(), utflen);

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

}

#endif //NBT_SRC_MODIFIED_UTF_HPP_
