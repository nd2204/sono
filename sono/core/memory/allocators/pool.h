#ifndef POOL_H
#define POOL_H

#include "core/common/types.h"

class PoolAllocator {
public:
  /// @param chunkSizeBytes the size of each chunk in bytes
  /// @param chunkCount the number of chunks
  explicit PoolAllocator(u32 poolSizeBytes, u32 chunkSize, usize chunkAlign);
  PoolAllocator(
    u8 *backingBuffer, u32 backingBufferSize, u32 chunkSize, usize chunkAlign
  );

  ~PoolAllocator();

  // Deletes copy and assignment
  PoolAllocator(const PoolAllocator &) = delete;
  PoolAllocator &operator=(const PoolAllocator &) = delete;

  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out preallocated memory
  void *Alloc(u32 sizeBytes);

  /// @brief push the pointer to the free list
  /// @param ptr the pointer to the memory to free
  /// @note: This does not free the memory, it just marks it as free
  void Free(void *ptr);

  ///
  void FreeAll();

private:
  struct Chunk {
    Chunk *next;
  };

  u8 *m_Buf;
  u32 m_BufSize;
  u32 m_ChunkSize;
  Chunk *m_FreeList;
};

#endif // !POOL_H