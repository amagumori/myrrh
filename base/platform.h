#ifndef _HAVE_PLATFORM_H
#define _HAVE_PLATFORM_H

#include <cstddef>
#include <cassert>
#include <stdint.h>

namespace Engine {

  // useful macros taken from ozz-animation
  // https://github.com/galek/ozz-animation

  // array size macro.
  #define ARRAY_SIZE(_array) (sizeof(_array) / sizeof(_array[0]))

  // alignment macro for VS or clang / g++
  #if defined(_MSC_VER)
  #define ALIGN(_alignment) __declspec(align(_alignment))
  #else
  #define ALIGN(_alignment) __attribute__((aligned(_alignment)))
  #endif

  // fuck what the compiler thinks, we're inlining this function
  #if defined(_MSC_VER)
  #define FORCE_INLINE __forceinline
  #else
  #define FORCE_INLINE inline __attribute__((always_inline))
  #endif

  // never inline this function
  #if defined(_MSC_VER)
  #define DONT_INLINE __declspec(noinline)
  #else
  #define DONT_INLINE __attribute__((noinline))
  #endif

  template<typename _T>
  _T* PointerStride(_T* t, size_t _stride) {
    return reinterpret_cast<_T*>(reinterpret_cast<uintptr_t>(t) + _stride);
  }

  template<typename _T>
  struct Range {
    Range() :
      begin(NULL),
      end(NULL) 
    {}

    Range(_T* _begin, const _T* _end) :
      begin(_begin),
      end(_end)
    {}

    Range(_T* _begin, ptrdiff_t _size) :
      begin(_begin),
      end(_begin + _size)
    {}

    template<size_t size>
    explicit Range(_T (&_array)[_size]) :
      begin(_array),
      end(_array + _size)
    {}
      
    template<size_t size>
    void operator = (_T (&_array)[_size]) {
      begin = _array;
      end = _array + _size;
    }

    // cast operator
    operator Range<const _T> () const {
      return Range<const _T>(begin, end);
    }

    // returns const & to element _i of range
    _T& operator[](size_t _i) const {
      assert(begin && &begin[_i] < end && "index out of range.");
      return begin[_i]; 
    }
  
    // return non-const & to element _i of range
    _T& operator[](size_t _i) {
      assert(begin && &begin[_i] < end && "index out of range.");
      return begin[_i];
    }


    size_t count() const {
      const ptrdiff_t count = end - begin;
      return count > 0 ? count : 0;
    }

    size_t size() const {
      const ptrdiff_t size = reinterpret_cast<uintptr_t>(end) - reinterpret_cast<uintptr_t>(begin);
      return size > 0 ? size : 0;
    }

    _T* begin;

    // this should never be dereferenced
    // hence why it's declared const
    const _T* end;

  };

}

#endif

