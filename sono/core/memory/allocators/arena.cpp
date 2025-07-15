#include "arena.h"
#include "core/memory/memory_system.h"

#include <cstdint>
#include <cstring>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

using Marker = ArenaAllocator::Marker;

ArenaAllocator::ArenaAllocator()
  : m_Buf(nullptr)
  , m_BufSize(0)
  , m_Offset(0)
  , m_IsHeapAlloc(false) {}
// ------------------------------------------------------------------------------------------
ArenaAllocator::ArenaAllocator(u32 arenaSizeBytes) { AllocateArena(arenaSizeBytes); }
// ------------------------------------------------------------------------------------------
ArenaAllocator::ArenaAllocator(u8 *backingBuffer, u32 backingBufferSize) {
  AssignArena(backingBuffer, backingBufferSize);
}
// ------------------------------------------------------------------------------------------
void ArenaAllocator::AllocateArena(u32 arenaSizeBytes) {
  ASSERT(arenaSizeBytes > 0);
  m_IsHeapAlloc = true;
  m_BufSize = arenaSizeBytes;
  m_Offset = 0;
  m_Buf = (u8 *)SN_ALLOC(arenaSizeBytes, ALLOC_TYPE_ALLOCATOR_ARENA);
  SN_ASSERT_F(m_Buf, "Error when allocating memory for arena of size %d", arenaSizeBytes);
}
// ------------------------------------------------------------------------------------------
void ArenaAllocator::AssignArena(u8 *backingBuffer, u32 backingBufferSize) {
  ASSERT(backingBuffer);
  ASSERT(backingBufferSize > 0);
  m_IsHeapAlloc = false;
  m_Buf = backingBuffer;
  m_BufSize = backingBufferSize;
  m_Offset = 0;
}
// ------------------------------------------------------------------------------------------
ArenaAllocator::~ArenaAllocator() {
  if (m_IsHeapAlloc && m_Buf) {
    FreeInternalBuffer();
  }
}
// ------------------------------------------------------------------------------------------
Marker ArenaAllocator::GetMarker() const { return m_Offset; }
// ------------------------------------------------------------------------------------------
u32 ArenaAllocator::GetSize() const { return m_BufSize; }
// ------------------------------------------------------------------------------------------
void ArenaAllocator::FreeInternalBuffer() {
  ASSERT(m_IsHeapAlloc);
  SN_FREE(m_Buf);
  m_Offset = 0;
  m_BufSize = 0;
  m_Buf = nullptr;
}
// ------------------------------------------------------------------------------------------
void *ArenaAllocator::AllocAlign(usize sizeBytes, u16 align) {
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
// ------------------------------------------------------------------------------------------
void *ArenaAllocator::Alloc(usize sizeBytes) {
  // LOG_DEBUG_F("arena [offset=%d, arenaSize=%d]", m_Offset, m_BufSize);
  return this->AllocAlign(sizeBytes, 1);
}
// ------------------------------------------------------------------------------------------
void ArenaAllocator::FreeToMarker(Marker marker) {
  // Only allow rolling back if marker is inside the range of [buf_ptr, current_ptr]
  if (marker <= m_BufSize) {
    m_Offset = marker;
  }
}
// ------------------------------------------------------------------------------------------
void ArenaAllocator::Clear() { this->m_Offset = 0; }
