#pragma once

#include "allocator.h"
//#include "assert.h"
#include <cassert>
#include <new>
#include <type_traits>
#include <cstring>
#include <utility>

namespace Engine {
  template <typename T, bool is_trivially_copyable = std::is_trivial<T>::value> class Array;

// if T is not trivially copyable

template <typename T> class Array<T, false> {
  public:
    explicit Array(Allocator& allocator) :
      _allocator(allocator)
    {
      _data = nullptr;
      _capacity = 0;
      _size = 0;
    }

    explicit Array(const Array& rhs) :
      _allocator(rhs._allocator)
    {
      _data = nullptr;
      _capacity = 0;
      _size = 0;
      *this = rhs;
    }

    T* begin() const { return _data; }

    T* end() const { return _data + _size; }

    void operator = (const Array& rhs) {
      if (this != &rhs) {
        callDestructors(_data, _data + _size);
        _allocator.deallocate(_data);
        _data = (T*)_allocator.allocate(rhs._capacity * sizeof(T));
        _capacity = rhs._capacity;
        _size = rhs._size;
        for (int i=0; i < _size; ++i) {
          new ((char*)(_data + i)) T(rhs._data[i]);
        }
      }
    }

    ~Array() {
      callDestructors(_data, _data + _size);
      _allocator.deallocate(_data);
    }

    int indexOf(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          return i;
        }
      }
      return -1;
    }

    void eraseItemFast(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          eraseFast(i);
          return;
        }
      }
    }

    void eraseFast(int index) {
      if (index >= 0 && index < _size) {
        _data[index].~T();
        if (index != _size - 1) {
          new ((char*)(_data + index)) T(_data[_size - 1]);
        }
        --_size;
      }
    }

    void eraseItem(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          erase(i);
          return;
        }
      }
    }

    void insert(int index, const T& value) {
      if (_size == _capacity) {
        grow();
      }
      memmove(_data + index + 1, _data + index, sizeof(T) * (_size - index));
      new (&_data[index]) T(value);
      ++_size;
    }

    void erase(int index) {
      if (index >= 0 && index < _size) {
        _data[index].~T();
        if (index < _size - 1) {
          memmove(_data + index,
                  _data + index + 1,
                  sizeof(T) * (_size - index - 1));
        }
        --_size;
      }
    }

    void push(const T& value) {
      int size = _size;
      if (size == _capacity) {
        grow();
      }
      new ((char*)(_data + _size)) T(value);
      ++_size;
      _size = size;
    }

    template <typename... Params> T& emplace(Params&&... params) {
      if (_size == _capacity) {
        grow();
      }
      new ((char*)(_data + _size)) T(std::forward<Params>(params)...);
      ++_size;
      return _data[_size - 1];
    }

    bool empty() const { return _size == 0; }

    void clear() {
      callDestructors(_data, _data + _size);
      _size = 0;
    }

    T& pushEmpty() {
      if (_size == _capacity) {
        grow();
      }
      new ((char*)(_data + _size)) T();
      ++_size;
      return _data[_size - 1];
    }

    const T& back() const { return _data[_size - 1]; }

    T& back() { return _data[_size - 1]; }

    void pop() {
      if (_size > 0) {
        _data[_size - 1].~T();
        --_size;
      }
    }

    void resize(int size) {
      if (size > _capacity) {
        reserve(size);
      }
      for (int i = _size; i < size; ++i) {
        new ((char*)(_data + i)) T();
      }
      callDestructors(_data + size, _data + _size);
    }

    void reserve(int capacity) {
      if (capacity > _capacity) {
        T* newData = (T*)_allocator.allocate(capacity * sizeof(T));
        memcpy(newData, _data, sizeof(T) * _size);
        _allocator.deallocate(_data);
        _data = newData;
        _capacity = capacity;
      }
    }

    // array accessor operator is overloaded 
    // just for these assertions.

    const T& operator[](int index) const {
      assert(index >= 0 && index < _size);
      return _data[index];
    }
 
    T& operator[](int index) {
      assert(index >= 0 && index < _size);
      return _data[index];
    }

    int size() const { return _size; }
    int capacity() const { return _capacity; }

  private:
    void grow() {
      // if capacity is 0 then new capacity is 4, else double capacity
      int newCapacity = _capacity == 0 ? 4 : _capacity * 2;
      T* new_data = (T*)_allocator.allocate(newCapacity * sizeof(T));
      memcpy(new_data, _data, sizeof(T) * _size);
      _allocator.deallocate(_data);
      _data = new_data;
      _capacity = newCapacity;
    }

    void callDestructors(T* begin, T* end) {
      for(; begin < end; ++begin) {
        begin->~T();
      }
    }

  private:
    Allocator& _allocator;
    int _capacity;
    int _size;
    T* _data;
};

// if T is trivially copyable..

template <typename T> class Array<T, true> {
  public:
    explicit Array(Allocator& allocator) :
      _allocator(allocator)
    {
      _data = nullptr;
      _capacity = 0;
      _size = 0;
    }

    explicit Array(const Array& rhs) :
      _allocator(rhs._allocator)
    {
      _data = nullptr;
      _capacity = 0;
      _size = 0;
      *this = rhs;
    }

    T* begin() const { return _data; }

    T* end() const { return &_data[_size]; }

    void operator = (const Array& rhs) {
      if (this != &rhs) {
        _allocator.deallocate(_data);
        _data = (T*)_allocator.allocate(rhs._capacity * sizeof(T));
        _capacity = rhs._capacity;
        _size = rhs._size;
        ::memmove(_data, rhs._data, sizeof(T) * _size);
      }
    }

    ~Array() {
      _allocator.deallocate(_data);
    }

    void swap(Array<T, true>& rhs) {
      assert(&rhs._allocator == &_allocator);

      int i = rhs._capacity;
      rhs._capacity = _capacity;
      _capacity = i;
  
      i = _size;
      _size = rhs._size;
      rhs._size = i;

      T* p = rhs._data;
      rhs._data = _data;
      _data = p;
    }

    int indexOf(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          return i;
        }
      }
      return -1;
    }

    void eraseItemFast(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          eraseFast(i);
          return;
        }
      }
    }

    void eraseFast(int index) {
      if (index >= 0 && index < _size) {
        _data[index].~T();
        if (index != _size - 1) {
          memmove(_data + index, _data + _size - 1, sizeof(T));
        }
        --_size;
      }
    }

    void eraseItem(const T& item) {
      for (int i=0; i < _size; ++i) {
        if (_data[i] == item) {
          erase(i);
          return;
        }
      }
    }

    void insert(int index, const T& value) {
      if (_size == _capacity) {
        grow();
      }
      memmove(_data + index + 1, _data + index, sizeof(T) * (_size - index));
      _data[index] = value;
      ++_size;
    }

    void erase(int index) {
      if (index >= 0 && index < _size) {
        _data[index].~T();
        if (index < _size - 1) {
          memmove(_data + index,
                  _data + index + 1,
                  sizeof(T) * (_size - index - 1));
        }
        --_size;
      }
    }

    void push(const T& value) {
      int size = _size;
      if (size == _capacity) {
        grow();
      }
      _data[size] = value;
      ++_size;
      _size = size;
    }

    bool empty() const { return _size == 0; }

    void clear() {
      _size = 0;
    }

    T& pushEmpty() {
      if (_size == _capacity) {
        grow();
      }
      ++_size;
      return _data[_size - 1];
    }

    const T& back() const { return _data[_size - 1]; }

    T& back() { 
      assert(_size > 0);
      return _data[_size - 1]; 
    }

    void pop() {
      if (_size > 0) {
        --_size;
      }
    }

    void resize(int size) {
      if (size > _capacity) {
        reserve(size);
      }
      _size = size;
    }

    void reserve(int capacity) {
      if (capacity > _capacity) {
        T* newData = (T*)_allocator.allocate(capacity * sizeof(T));
        memcpy(newData, _data, sizeof(T) * _size);
        _allocator.deallocate(_data);
        _data = newData;
        _capacity = capacity;
      }
    }

    // array accessor operator is overloaded 
    // just for these assertions.

    const T& operator[](int index) const {
      assert(index >= 0 && index < _size);
      return _data[index];
    }
 
    T& operator[](int index) {
      assert(index >= 0 && index < _size);
      return _data[index];
    }

    int size() const { return _size; }
    int capacity() const { return _capacity; }

  private:
    void grow() {
      // if capacity is 0 then new capacity is 4, else double capacity
      int newCapacity = _capacity == 0 ? 4 : _capacity * 2;
      T* new_data = (T*)_allocator.allocate(newCapacity * sizeof(T));
      memcpy(new_data, _data, sizeof(T) * _size);
      _allocator.deallocate(_data);
      _data = new_data;
      _capacity = newCapacity;
    }

  private:
    Allocator& _allocator;
    int _capacity;
    int _size;
    T* _data;
};

}
