#include <core/memory/allocators/pool.h>

#include <core/common/logger.h>
#include <core/common/snassert.h>
#include <core/memory/memory_system.h>

PoolAllocator::PoolAllocator(u32 poolSizeBytes, u32 chunkSize, usize chunkAlign)
  : m_IsHeapAllocated(1) {
  SN_WARN_FUNCTION_UNIMPLEMENTED;
}

PoolAllocator::PoolAllocator(
  u8 *backingBuffer, u32 backingBufferSize, u32 chunkSize, usize chunkAlign
)
  : m_IsHeapAllocated(0) {
  // Adjust buffer size
  AlignResult result = AlignForward((uintptr_t)backingBuffer, chunkAlign);
  backingBufferSize -= (usize)result.offset;
  // Align chunk size
  chunkSize = AlignForward(chunkSize, chunkAlign).aligned;

  SN_ASSERT(chunkSize >= sizeof(FreeChunk), "chunkSize is too small");
  SN_ASSERT(chunkSize <= backingBufferSize, "chunkSize must fit in the buffer");

  m_Buf = backingBuffer;
  m_BufSize = backingBufferSize;
  m_ChunkSize = chunkSize;
  m_FreeChunkHead = nullptr;

  FreeAll();
}

PoolAllocator::~PoolAllocator() {
  if (m_IsHeapAllocated) {
    SN_FREE(m_Buf);
  }
}

void *PoolAllocator::Alloc(usize sizeBytes, AllocationType tag) {
  (void)sizeBytes;
  FreeChunk *pChunk = m_FreeChunkHead;
  if (pChunk) {
    m_FreeChunkHead = m_FreeChunkHead->next;
    return SN_ZERO((void *)pChunk, m_ChunkSize)
  }
  SN_ASSERT(pChunk, "Pool memory is full");
  return nullptr;
}

void PoolAllocator::Free(void *ptr) {
  if (!ptr) return;

  // clang-format off
  SN_ASSERT(
    ptr >= m_Buf && ptr < &m_Buf[m_BufSize],
    "Memory is out of bound of the buffer in this pool"
  );
  // clang-format on

  FreeChunk *chunk = reinterpret_cast<FreeChunk *>(ptr);
  chunk->next = m_FreeChunkHead;
  m_FreeChunkHead = chunk;
}

void PoolAllocator::FreeAll() {
  usize chunkCount = m_BufSize / m_ChunkSize;
  for (usize i = 0; i < chunkCount; ++i) {
    FreeChunk *chunk = reinterpret_cast<FreeChunk *>(&m_Buf[i * m_ChunkSize]);
    chunk->next = m_FreeChunkHead;
    m_FreeChunkHead = chunk;
  }
}
