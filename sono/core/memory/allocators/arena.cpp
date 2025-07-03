#include "arena.h"
#include "core/memory/memory_system.h"

#include <cstdint>
#include <cstring>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

using Marker = ArenaAllocator::Marker;

ArenaAllocator::ArenaAllocator(u32 arenaSizeBytes) {
  ASSERT(arenaSizeBytes > 0);
  m_BufSize = arenaSizeBytes;
  m_Offset = 0;
  m_Buf = (u8 *)SN_ALLOC(arenaSizeBytes, ALLOC_TYPE_ALLOCATOR_ARENA);
  SN_ASSERT_F(m_Buf, "Error when allocating memory for arena of size %d", arenaSizeBytes);
}

ArenaAllocator::ArenaAllocator(u8 *backingBuffer, u32 backingBufferSize)
  : m_Buf(backingBuffer)
  , m_BufSize(backingBufferSize)
  , m_Offset(0) {};

ArenaAllocator::~ArenaAllocator() { SN_FREE(m_Buf); }

Marker ArenaAllocator::GetMarker() const { return m_Offset; }

void *ArenaAllocator::AllocAlign(u32 sizeBytes, usize align) {
  uintptr_t current_ptr = (uintptr_t)m_Buf + (uintptr_t)m_Offset;
  uintptr_t offset = AlignAddress(current_ptr, align);
  offset -= (uintptr_t)m_Buf; // Change to relative offset

  if (offset + sizeBytes <= m_BufSize) {
    void *ptr = &m_Buf[offset];
    m_Offset = offset + sizeBytes;
    memset(ptr, 0, sizeBytes);
    return ptr;
  }

  return nullptr;
}

void *ArenaAllocator::Alloc(u32 sizeBytes) {
  // LOG_DEBUG_F("arena [offset=%d, arenaSize=%d]", m_Offset, m_BufSize);
  return this->AllocAlign(sizeBytes, 1);
}

void ArenaAllocator::FreeToMarker(Marker marker) {
  uintptr_t buf_ptr = (usize)m_BufSize;
  uintptr_t current_ptr = (usize)m_BufSize + (usize)m_Offset;

  uintptr_t m = static_cast<uintptr_t>(marker);
  // Only allow rolling back if marker is inside the range of [buf_ptr,
  // current_ptr]
  if (m >= buf_ptr && m <= current_ptr) {
    m_Offset = marker;
  }
}

void ArenaAllocator::Clear() { this->m_Offset = 0; }
