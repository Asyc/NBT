#ifndef NBT_SRC_BYTESWAP_HPP_
#define NBT_SRC_BYTESWAP_HPP_

#ifdef _MSC_VER

#include <cstdlib>
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else

#error "no byteswap function"

#endif

#ifdef NBT_BIG_ENDIAN
#define hostToNetwork16(x) (x)
#define hostToNetwork32(x) (x)
#define hostToNetwork64(x) (x)
#else
#define hostToNetwork16(x) bswap_16(static_cast<uint16_t>(x))
#define hostToNetwork32(x) bswap_32(static_cast<uint32_t>(x))
#define hostToNetwork64(x) bswap_64(static_cast<uint64_t>(x))
#endif

#endif //NBT_SRC_BYTESWAP_HPP_
