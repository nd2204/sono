#ifndef POOL_H
#define POOL_H

#include <core/common/types.h>
#include <core/memory/allocator.h>
#include <list>

class PoolAllocator : public Allocator {
public:
  /// @param chunkSizeBytes the size of each chunk in bytes
  /// @param chunkCount the number of chunks
  explicit PoolAllocator(u32 poolSizeBytes, u32 chunkSize, usize chunkAlign);
  PoolAllocator(u8 *backingBuffer, u32 backingBufferSize, u32 chunkSize, usize chunkAlign);

  ~PoolAllocator();

  // Deletes copy and assignment
  PoolAllocator(const PoolAllocator &) = delete;
  PoolAllocator &operator=(const PoolAllocator &) = delete;

  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out preallocated memory
  void *Alloc(usize sizeBytes, AllocationType tag) override;

  /// @brief push the pointer to the free list
  /// @param ptr the pointer to the memory to free
  /// @note: This does not free the memory, it just marks it as free
  void Free(void *ptr) override;

  /// @brief push all available chunks to the free list
  void FreeAll();

private:
  struct FreeChunk {
    FreeChunk *next;
  };

  u8 *m_Buf;
  usize m_BufSize;
  usize m_ChunkSize;
  FreeChunk *m_FreeChunkHead;
  // TODO: Removes this after global allocator implementation
  b8 m_IsHeapAllocated;
};

#endif // !POOL_H
