#include "pool.h"
#include "core/common/snassert.h"

PoolAllocator::PoolAllocator(u32 poolSizeBytes, u32 chunkSize, usize chunkAlign) {}

PoolAllocator::PoolAllocator(
  u8 *backingBuffer, u32 backingBufferSize, u32 chunkSize, usize chunkAlign
) {}

PoolAllocator::~PoolAllocator() {}

void *PoolAllocator::Alloc(u32 sizeBytes) { return nullptr; }

void PoolAllocator::Free(void *ptr) { SN_ASSERT(false, "PoolAllocator::Free not implemented"); }

void PoolAllocator::FreeAll() { SN_ASSERT(false, "PoolAllocator::FreeAll not implemented"); }
