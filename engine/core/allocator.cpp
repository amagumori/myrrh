#include "allocator.h"

using namespace Engine;

// all of this is taken from this page:
// http://www.gamedev.net/page/resources/_/technical/general-programming/c-custom-memory-allocation-r3010

// http://blog.molecular-matters.com/2011/07/05/memory-system-part-1/

// USAGE: 

// TODO: frame, heap, and page allocators.

/* LINEAR ALLOCATOR
 *
 * new allocations move the pointer to the first free address forward.
 * individual deallocations can't be done.  clear() is used to clear
 * the entire allocator memory.
 */

LinearAllocator::LinearAllocator(size_t size, void* start) :
  Allocator(size, start), 
  _current_position(start)
{
   //assert(size > 0); 
}

LinearAllocator::~LinearAllocator() 
{
  _current_position = nullptr;
}

void* LinearAllocator::allocate(size_t size, u8 alignment) {
  //assert(size != 0);
  u8 adjustment = alignForwardAdjustment(_current_position, alignment);
 
  // if you overrun your max size return nullptr 
  if (_used_memory + adjustment + size > _size)
    return nullptr;
  
  uintptr_t aligned_address = (uintptr_t)_current_position + adjustment;

  // step forward
  _current_position = (void*) (aligned_address + size);
  _used_memory += size + adjustment;
  _num_allocations++;

  return (void*)aligned_address;
}

void LinearAllocator::deallocate(void* p) {
  //assert( false && "use clear instead, LinearAllocator doesn't provide an implementation of deallocate.");
}

void LinearAllocator::clear() {
  _num_allocations = 0;
  _used_memory = 0;
  _current_position = _start;
}

/* STACK ALLOCATOR
 *
 * 3 pointers are maintained: base pointer, stack pointer, pointer to last alloc made.
 * new allocations move the pointer by the requested num bytes + alignment adjustment to store the header and align it on the current byte boundary.
 * allocation header contains: { adjustment used in this allocation, pointer to previous allocation }.
 * to deallocate, allocator checks if the address to the mem you want to dealloc corresponds to address of last allocation made.
 * if so, allocator accesses allocation header to replace previous_alloc pointer with the one in the header, then frees the memory associated with the header. (??) 
 */

StackAllocator::StackAllocator(size_t size, void* start)
  : Allocator(size, start), _current_position(start)
{
  //assert(size > 0);
  #if _DEBUG
  _previous_position = nullptr;
  #endif
}

StackAllocator::~StackAllocator()
{
  #if _DEBUG
  _previous_position = nullptr;
  #endif
  _current_position = nullptr;
}

void* StackAllocator::allocate(size_t size, u8 align) {
  //assert(size > 0);
  
  u8 adjustment = alignForwardAdjustmentWithHeader(_current_position, align, sizeof(AllocationHeader));
  
  if (_used_memory + adjustment + size > _size)
    return nullptr;

  char* aligned_address = (char*)((uintptr_t)_current_position + adjustment);

  // hmm
  AllocationHeader* header = (AllocationHeader*)( (uintptr_t)aligned_address - sizeof(AllocationHeader));
  
  header->adjustment = adjustment;

  #if _DEBUG
  header->previous_address = _previous_position;
  _previous_position = aligned_address;
  #endif

  _current_position = aligned_address + size;
  _used_memory += size + adjustment;
  _num_allocations++;

  return aligned_address;

}

// little shaky on how this works.
void StackAllocator::deallocate(void* p) {
  //assert( p == _previous_position );

  AllocationHeader* header = (AllocationHeader*)((char*)p - sizeof(AllocationHeader));

  _used_memory -= (uintptr_t)_current_position - (uintptr_t)p + header->adjustment;
  _current_position = (void *) ((uintptr_t)p - header->adjustment);

  #if _DEBUG
  _previous_position = header->previous_address;
  #endif

  _num_allocations--;
}

/* POOL ALLOCATOR
 *
 * this allocator only allows allocations of a fixed size and alignment.
 * it's fucking fast as shit because of this
 *
 * a linked list of free blocks is maintained, but since they're all the same
 * size, each one just stores pointer to next.
 *
 * no alignment per allocation means almost no memory overhead.
 *
 * on allocation, allocator returns first free block and updates linked list.
 * on dealloc, allocator adds dealloc'd block to free blocks linked list.
 */

PoolAllocator::PoolAllocator(size_t objectSize, u8 objectAlignment, size_t size, void* mem) :
  Allocator(size, mem), _objectSize(objectSize), _objectAlignment(objectAlignment)
{
  //assert(objectSize >= sizeof(void*))

  u8 adjustment = alignForwardAdjustment(mem, objectAlignment);

  // ugly!!!!
  _free_list = (void**) ((char**)((char*)mem + adjustment));

  size_t numObjects = (size-adjustment) / objectSize;

  void** p = _free_list;

  for (size_t i = 0; i < numObjects - 1; i++) {
    // adding a size_t and void** should be fine?
    *p = p + objectSize;
    p = (void**) *p;
  }

  *p = nullptr;

}

PoolAllocator::~PoolAllocator() {
  _free_list = nullptr;
}

void* PoolAllocator::allocate(size_t size, u8 align) {
  //assert(size == _objectSize && align == _objectAlignment);

  if (_free_list == nullptr)
    return nullptr;

  void* p = _free_list;
 
  // what the fuck?? 
  _free_list = (void**) (*_free_list);

  _used_memory += size;
  _num_allocations++;
  
  return p;
}

void PoolAllocator::deallocate(void* p) {
  // why is it always cast into this really weird thing?
  *((void**)p) = _free_list;

  _free_list = (void**)p;
  _used_memory -= _objectSize;
  _num_allocations--;
}
