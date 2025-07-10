#ifndef ARENA_H
#define ARENA_H

#include "core/common/types.h"
#include "core/memory/allocator.h"
#include <utility>

class ArenaAllocator : public Allocator {
public:
  /// @brief: Stack marker: Represents the current top of the
  /// stack. You can only roll back to a marker, not to
  /// arbitrary locations within the stack.
  typedef u32 Marker;

  ArenaAllocator();

  /// @param arenaSizeBytes the sizes for internal arena buffer
  explicit ArenaAllocator(u32 arenaSizeBytes);
  explicit ArenaAllocator(u8 *backingBuffer, u32 backingBufferSize);

  ~ArenaAllocator();

  void AllocateArena(u32 arenaSizeBytes);
  void AssignArena(u8 *backingBuffer, u32 backingBufferSize);

  // Deletes copy and assignment
  ArenaAllocator(const ArenaAllocator &) = delete;
  ArenaAllocator operator=(const ArenaAllocator &) = delete;

  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out preallocated memory
  void *Alloc(usize sizeBytes) override;

  /// @param sizeBytes the size in bytes
  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out, aligned, preallocated memory
  void *AllocAlign(usize sizeBytes, u16 align) override;

  /// this does nothing
  void Free(void *mem) override { (void)mem; };

  /// @return: the offset in bytes from the buffer to the current stack top.
  Marker GetMarker() const;

  /// @brief free the internal buffer, error if the buffer is not heap allocated
  void FreeInternalBuffer();

  /// @brief: Rolls the stack back to a previous marker.
  void FreeToMarker(Marker marker);

  /// @brief: Clear the arena (resetting the offset to 0).
  /// @note: This does not free the dynamically allocated memory
  void Clear();

private:
  u8 *m_Buf;
  u32 m_BufSize;
  Marker m_Offset;
  b8 m_IsHeapAlloc;
};

#endif // !ARENA_H
