#include <cstring>
#include "murmur3.h"

using namespace Engine;

static const u32 seed = 0xCAFEBABE;

static inline u32 rot132 ( u32 x, s8 r ) {
  return (x << r) | (x >> (32 - r));
}

static inline u32 fmix32 (u32 h) {
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

#define ROTL32(x,y) rot132(x,y)

#define getblock(p,i) (p[i])

void murmur3_32 (const void* key, int len, u32 seed, void* out) {
  const u8* data = (const u8*)key;
  const int nblocks = len / 4;

  u32 h1 = seed;
  u32 c1 = 0xcc9e2d51;
  u32 c2 = 0x1b873593;

  // body
  const u32* blocks = (const u32*)(data + nblocks*4);
  
  for (int i = -nblocks; i; i++) {
    u32 k1 = getblock(blocks, i);
    k1 *= c1;
    k1 = ROTL32(k1, 15);
  
    h1 ^= k1;
    h1 = ROTL32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }

  // tail
  const u8* tail = (const u8*)(data + nblocks*4);
  u32 k1 = 0;
  switch (len & 3) {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
  };

  h1 ^= len;
  h1 = fmix32(h1);

  *(u32*)out = h1;
}

u32 murmur3 (char str[]) {
  u32 out = 0;
  //void* string = reinterpret_cast<void*>(str);
  int len = strlen(str);
 
  const void* key = reinterpret_cast<void*>(str);
 
  murmur3_32(key, len, seed, &out);
  return out;
}
