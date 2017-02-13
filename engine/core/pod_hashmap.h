

namespace Engine {
  template<class K, class V>
  struct PODHashNode {
    typedef PODHashNode<K, V> my_node;
    K m_key;
    V m_value;
    my_node* m_next;
  };

  template<class Key>
  struct PODHashFunc {
    static uint32_t get(const Key& key);
  };

  template<>
  struct PODHashFunc<int32_t> {
    static uint32_t get(const int32_t& key) {
      uint32_t x = ((key >> 16) ^ key) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x);
      return x;
    }
  };

  template<>
  struct PODHashFunc<uint32_t> {
    static uint32_t get(const uint32_t& key) {
      uint32_t x = ((key >> 16) ^ key) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x);
      return x;
    }
  };


  template<>
  struct PODHashFunc<void*> {
    static size_t get(const void* key) {
      size_t x = ((int32_t)(key) >> 16 ^ int32_t(key)) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x);
      return x;
    }
  };
  

  template<>
  struct PODHashFunc<char*> {
    static uint32_t get(const char* key) {
      uint32_t result = 0x55555555;
      while (*key) {
        result ^= *key++;
        result = ((result << 5) | (result >> 27));
      }
      return result;
    }
  };


}
