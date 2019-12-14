// vim:ft=c:ts=2:sw=2:sts=2:et
//
//                      == hashing algorithms ==
//
//  x += x << n
//  x *= (x << n) + 1

#include <stdint.h>
#include <hash>

#define DECL_HASH(name, type)                          \
type hash_##name (const uint8_t* buf, size_t len) {    \
  type digest = hash_##name##_init();                  \
  for (size_t i = 0; i < len; i++)                     \
    digest = hash_##name##_write(digest, buf[i]);      \
  return hash_##name##_final(digest);               \
}                                                      \

//// Fowler-Noll-Vo
//
//  Glenn Fowler and Phong Vo took the idea for the hashing algorithm from a
//  reviewer comment of the IEEE POSIX P1003.2 commitee in 1991. landon curt
//  noll subsequently improved the algorithm.
//
//  FNV depends on a prime
//    for a given s:
//
//      p \mod (2^40 - 2^24 - 1) > (2^24 + 2^8 + 2^7)
//
//      s \in \mathbb{I}^*
//      4 < s < 11
//      n = 2^8
//      t = \left\lfloor\dfrac{5 + 2^8}{12}\right\rfloor
//      p = 256^t + 2^8 + b, 0 < b < 2^8
//                           number of one bits \in {4; 5}
//
//  Experimentally, FNV prime matching the above constraints tend to have
//  better dispersion properties. They improve the polynomial feedback char-
//  acteristic when an FNV prim multiplies an intermediate hash value. As
//  such, the hash values produced are more scattered throughout the n-bit
//  hash space.
//
//
//  The offset basis is computed by hashing
//  "chongo <Landon Curt Noll> /\../\"
//  with an initial value of 0
//
//
//  0x01000193
//  0x811c9dc5
//
//  0x00000100 000001B3
//  0xcbf29ce4 84222325
//
//  0x00000000 01000000 00000000 0000013B
//  0x6c62272e 07bb0142 62b82175 6295c58d
//
//  0x00000000 00000000 00000100 00000000 00000000 00000000 00000000 00000163
//  0xdd268dbc aac55036 2d98c384 c4e576cc c8b15368 47b6bbb3 1023b4c8 caee0535
//
//  0x00000000 00000000 00000000 00000000 00000000 01000000 00000000 00000000
//    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000157
//  0xb86db0b1 171f4416 dca1e50f 309990ac ac87d059 c9000000 00000000 00000d21
//    e948f68a 34c192f6 2ea79bc9 42dbe7ce 18203641 5f56e34b ac982aac 4afe9fd9
//
//  0x00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
//    00000000 00000000 00000100 00000000 00000000 00000000 00000000 00000000
//    00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
//    00000000 00000000 00000000 00000000 00000000 00000000 00000000 0000018D
//  0x00000000 00000000 005f7a76 758ecc4d 32e56d5a 591028b7 4b29fc42 23fdada1
//    6c3bf34e da3674da 9a21d900 00000000 00000000 00000000 00000000 00000000
//    00000000 00000000 00000000 00000000 00000000 00000000 00000000 0004c6d7
//    eb6e7380 2734510a 555f256c c005ae55 6bde8cc9 c6a93b21 aff4b16c 71ee90b3
//
//
//  uint32_t hash_fnv(const uint8_t* buf, size_t len) {
//    uint32_t offset_basis = 0x811c9dc5;
//    for (; len; buf++, len--) {
//      offset_basis ^= (uint64_t)*buf;
//      offset_basis *= 0x01000193;
//    }
//    return offset_basis;
//  }
//
DECL_HASH(fnv, uint32_t)

uint32_t hash_fnv_init(void) {
  return 0x01000193;
}

uint32_t hash_fnv_write(uint32_t ctx, const uint8_t byte) {
  return (ctx ^ (uint32_t)byte) * 0x811c9dc5;
}

uint32_t hash_fnv_final(uint32_t ctx) {
  return ctx;
}

//// Jenkins one-at-a-time
//
//  uint32_t hash_oaat(const uint8_t* buf, size_t len) {
//    uint32_t digest = 0;
//    for (size_t i = 0; i < len; i++) {
//      digest += buf[i++];
//      digest += digest << 10;
//      digest ^= digest >>  6;
//    }
//    digest += digest <<  3;
//    digest ^= digest >> 11;
//    digest += digest << 15;
//    return digest;
//  }
//
DECL_HASH(oat, uint32_t)

uint32_t hash_oat_init(void) {
  return 0;
}

uint32_t hash_oat_write(uint32_t ctx, const uint8_t byte) {
  ctx += (ctx + byte) << 10;
  return ctx ^ (ctx >> 6);
}

uint32_t hash_oat_final(uint32_t ctx) {
  ctx += ctx <<  3;
  ctx ^= ctx >> 11;
  return ctx + (ctx << 15);
}
