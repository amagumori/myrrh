#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include "types.h"
#include <utility>

namespace Engine {

  class Allocator {
    public:
      Allocator(std::size_t size, void* start) {
        _start = start;
        _size = size;
        _used_memory = 0;
        _num_allocations = 0;
      }

      // memory allocation is expensive enough anyway where virtuals
      // dont have a performance ding
      // not templated so you can make >1 of these objects..
      virtual void *allocate(size_t size, u8 align = 4) = 0;
      virtual void deallocate(void *p) = 0;
      virtual size_t allocated_size(void *p) = 0;

      virtual ~Allocator() {
        // figure out how to do assertions
        // either with yo own macro or cassert or something
        //assert(_num_allocations == 0 && _used_memory == 0);
          
        _start = nullptr;
        _size = 0;
      }

      // getters
      inline size_t getUsedMemory() const {
        return _used_memory;
      }

      inline size_t getNumAllocations() const {
        return _num_allocations;
      }

      inline size_t getSize() const {
        return _size;
      }

      inline void* getStart() const {
        return _start;
      }

      // align to nearest alignment byte boundary
      // reinterpret_cast means "cast something to something it's not"
      // static_cast means "cast something to something it conceivably could be i.e. int -> char* etc.
      // use static_cast when possible, reinterpret_cast only when needed. 
      
      // to n-byte align a memory address X you need to mask off the
      // log2(n) least significant bits from X.
      inline void* alignForward(void* address, u8 alignment) {
        return (void*)( ( reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1) ) & static_cast<uintptr_t>(~(alignment - 1)) );
      }

      // this calculates the adjustment needed to align the address.
      // doesn't actually do it, the above does.
      inline u8 alignForwardAdjustment(const void* address, u8 alignment) {
        size_t adjustment = alignment - ( reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1) );

        if (adjustment == alignment)
          return 0; // already aligned.
  
        return adjustment;
      }       
     
      // ALIGNMENT MUST BE A POWER OF 2, OBVIOUSLY
 
      inline u8 alignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize) {
        // first scoot address forward by <align>
        u8 adjustment = alignForwardAdjustment(address, alignment);
        u8 neededSpace = headerSize;
        
        // if moving it by <align> wasn't enough..
        if (adjustment < neededSpace) {
          // subtract the adjustment you already did from the space you need
          neededSpace -= adjustment;
          // increase the adjustment to fit the header.
          adjustment += alignment * (neededSpace / alignment);

          // if it still aint aligned...
          if (neededSpace % alignment > 0)
            adjustment += alignment;
        }
        return adjustment;
      }
     
      // this should work...
 
      template <class T, typename... Args>
      T* newObject(Args&&... params) {
        auto mem = allocate(sizeof(T));
        return new (mem) T(std::forward<Args>(params)...);
      }

      template <class T>
      void deleteObject(T* ptr) {
        if (ptr) {
          ptr->~T();
          deallocate(ptr);
        }
      }
      
    protected:
      void* _start;
      size_t _size;

      size_t _num_allocations;
      size_t _used_memory;
  };

  class LinearAllocator : public Allocator {
    public:
      LinearAllocator(size_t size, void* start);
      ~LinearAllocator();

      virtual void *allocate(size_t size, u8 align) override;
      virtual void deallocate( void* p ) override;
      virtual size_t allocated_size( void* p ) override;
      void clear(); // ?
    private:
      void* _current_position;
      LinearAllocator ( const LinearAllocator& ); 
      LinearAllocator& operator = ( const LinearAllocator& );
  };

  class StackAllocator : public Allocator {
    public:
      StackAllocator(size_t size, void* start);
      ~StackAllocator();

      virtual void *allocate(size_t size, u8 align) override;
      virtual void deallocate( void* p ) override;
      virtual size_t allocated_size( void* p ) override;
    private:
      StackAllocator ( const StackAllocator& );
      StackAllocator& operator = (const StackAllocator&);

      void* _current_position;
      struct AllocationHeader
      {
        #if _DEBUG
        void* previous_address;
        #endif
        u8 adjustment;
      };
      #if _DEBUG
      void* _previous_position;
      #endif
  };

  // frame allocator is used for allocations that only persist *per frame*.
  // best for things that get destroyed after frame is rendered.
  class FrameAllocator : Allocator {
    public:
      // override the allocator virtual functions..
    private:

  };

  // basically dlmalloc?
  class HeapAllocator : Allocator {
    public:

    private:

  };

  // this allocates raw virtual memory.
  class PageAllocator : Allocator {
    public:

    private:

  };

  // pool allocator is optimized for allocating many things of the same size.
  // this is gonna be the best thing for allocating SoA-type structures,
  // probably all components...
  class PoolAllocator : public Allocator {
    public:
      PoolAllocator(size_t objectSize, u8 align, size_t size, void* mem);
      ~PoolAllocator();

      virtual void *allocate(size_t size, u8 align) override;
      virtual void deallocate(void *p) override;
    private:
      PoolAllocator(const PoolAllocator&);
      PoolAllocator& operator = (const PoolAllocator&);

      size_t _objectSize;
      u8     _objectAlignment;
      void** _free_list;
  };

}

#endif
