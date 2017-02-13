#pragma once

#include "array.h"

namespace Engine {
  template <typename T>
  class SortedArray {
    public:
      SortedArray(Allocator& allocator) :
        _data(allocator)
      {}
      
      int size() const { return _data.size(); }

      T& operator[] (int index) {
        return _data[index];
      }
  
      const T& operator[] (int index) const {
        return _data[index];
      }

      int insert(const T& value) {
        if (_data.empty()) {
          _data.push(value); 
          return 0;
        } else {
          int i = index(value);
          if (i >= 0 && ((i < _data.size() && _data[i] != value || i == _data.size()))) {
            _data.insert(i, value);
            return i;
          }
        }
        return -1;
      }

      bool contains(const T& value) {

      }

      void clear() { _data.clear(); }

      void reserve(int capacity) { _data.reserve(capacity); }

      void erase(int index) { _data.erase(index); }

    private:
      int index(const T& value) const {
        int l = 0;
        int h = _data.size() - 1;
        while (l < h) {
          int mid = (l + h) >> 1;
          if (_data[mid] < value) {
            l = mid + 1;
          } else {
            h = mid;
          }
        }
        if (l + 1 == _data.size() && _data[l] < value) {
          return l + 1;
        }
        return l;
      }

    private:
      Array<T> _data;
  };

  template <typename Key, typename Value> 
  class AssociativeArray {
    public:
      AssociativeArray(Allocator& allocator) :
        _data(allocator)
      {}
  
      int insert(const Key& key, const Value& value) {
        return _data.insert(Pair(key, value));
      }

      bool find(const Key& key, Value& value) const {
        int i = find(key);
        if (i < 0) {
          return false;
        }
        value = _data[i]._value;
        return true;
      }

      int find(const Key& key) const {
        int l = 0;
        int h = _data.size() - 1;
        while (l < h) {
          int mid = (l + h) >> 1;
          if (_data[mid]._key < key) {
            l = mid + 1;
          } else {
            h = mid;
          }
        }
        if (l == h && _data[l]._key == key) {
          return l;
        }
        return -1;
      }

      const Value& operator[](const Key& key) const {
        int index = find(key);
        if (index >= 0) {
          return _data[index]._value;
        } else {
          return _data[_data.insert(Pair(key, Value()))]._value; 
        }
      }

      Value& operator[](const Key& key) {
        int index = find(key);
        if (index >= 0) {
          return _data[index]._value;
        } else {
          return _data[_data.insert(Pair(key, Value()))]._value;
        }
      }

      int size() const {
        return _data.size();
      }

      Value& get(const Key& key) {
        int index = find(key);
        ASSERT(index >= 0);
        return _data[index]_value;
      }
  
      Value& at (int index) {
        return _data[index]._value;
      }

      const Value& at (int index) const {
        return _data[index]._value;
      }

      void clear() { _data.clear(); }

      void reserve(int capacity) { _data.reserve(capacity); }

      const Key& getKey(int index) {
        return _data[index]._key; 
      }

      void eraseAt(int index) {
        _data.erase(index);
      }

      void erase(const Key& key) {
        int i = find(key);
        if (i >= 0) {
          eraseAt(i);
        }
      }
    private:
      struct Pair {
        Pair() {}
        Pair(const Key& key, const Value& value) : _key(key), _value(value) {}
        bool operator < (const Pair& rhs) const { return _key < rhs._key; }
        bool operator > (const Pair& rhs) const { return _key > rhs._key; }
        bool operator == (const Pair& rhs) const { return _key == rhs._key; }
        bool operator != (const Pair& rhs) const { return _key != rhs._key; }

        Key _key;
        Value _value;
      };
    private:
      SortedArray<Pair> _data;
  };
}
