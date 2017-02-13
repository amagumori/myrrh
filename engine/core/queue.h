#pragma once

#include <new>

namespace Engine {

  template<typename T, uint32_t count>
  class Queue {
    public:
      Queue(Allocator& allocator) :
        _allocator(allocator)
      {
        // implement this in a math utils file
        //ASSERT(Math::isPowerOf2(count));
        _buffer = (T*)(_allocator.allocate(sizeof(T) * count);
        _wr = _rd = 0;
      }

      ~Queue() {
        _allocator.deallocate(_buffer);
      }

      bool empty() const { return _rd == _wr; }
      uint32_t size() const { return _wr - _rd; }

      void push(const T& item) {
        ASSERT(_wr - _rd < count);
        uint32_t index = _wr & (count - 1);
        std::new(&buffer[index]) T(item);
        ++_wr;
      }

      void pop() {
        ASSERT(_wr != _rd);
        uint32_t index = _wr & (count - 1);
        (&_buffer[index])->~T();
        _rd++;
      }

      T& front() {
        uint32_t index = _rd & (count - 1);
        return _buffer[index];
      }

      const T& front() const {
        uint32_t index = _rd & (count - 1);
        return _buffer[index];
      }

      T& back() {
        ASSERT(!empty());
        uint32_t index = _wr & (count - 1);
        return _buffer[index - 1];
      }

      const T& back() const {
        ASSERT(!empty());
        uint32_t index = _wr & (count - 1);
        return _buffer[index - 1]; 
      }

    private:
      Allocator& _allocator;
      uint32_t _rd;
      uint32_t _wr;
      T* _buffer;




  };

}
