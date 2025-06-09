#ifndef ARENA_H
#define ARENA_H

#include "core/common/types.h"

class ArenaAllocator {
public:
  /// @brief: Stack marker: Represents the current top of the
  /// stack. You can only roll back to a marker, not to
  /// arbitrary locations within the stack.
  typedef u32 Marker;

  /// @param arenaSizeBytes the sizes for internal arena buffer
  explicit ArenaAllocator(u32 arenaSizeBytes);
  explicit ArenaAllocator(u8 *backingBuffer, u32 backingBufferSize);

  ~ArenaAllocator();

  // Deletes copy and assignment
  ArenaAllocator(const ArenaAllocator &) = delete;
  ArenaAllocator operator=(const ArenaAllocator &) = delete;

  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out preallocated memory
  void *Alloc(u32 sizeBytes);

  /// @param sizeBytes the size in bytes
  /// @param sizeBytes the size in bytes
  /// @return the pointer to the zeroed out, aligned, preallocated memory
  void *AllocAlign(u32 sizeBytes, usize align);

  /// @return: the offset in bytes from the buffer to the current stack top.
  Marker GetMarker() const;

  /// @brief: Rolls the stack back to a previous marker.
  void FreeToMarker(Marker marker);

  /// @brief: Clear the arena (resetting the offset to 0).
  /// @note: This does not free the dynamically allocated memory
  void Clear();

private:
  u8 *m_Buf;
  u32 m_BufSize;
  Marker m_Offset;
};

#endif // !ARENA_H