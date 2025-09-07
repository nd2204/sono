#ifndef SN_RESOURCE_BASE_H
#define SN_RESOURCE_BASE_H

#include "core/common/types.h"

class ResourceBase {
public:
  enum PriorityType {
    RES_PRIORITY_0 = 0,
    RES_PRIORITY_1,
    RES_PRIORITY_2,
    RES_PRIORITY_MAX // maximum priority thresshold
  };

  ResourceBase() { Clear(); }

  virtual ~ResourceBase() {}

  // Increment the reference counting
  constexpr void Acquire();

  // Decrement the reference counting.
  constexpr void Release();

  // Return the reference count of the current resource
  constexpr u32 GetRefCount() const;

  // Return true if the current resource is in use
  constexpr bool IsLocked() const;

  // Assign the priority of current resource
  constexpr void SetPriority(PriorityType priority);

  // Return the priority of current resource
  constexpr PriorityType GetPriority() const;

  // Get last access timestamp in seconds
  constexpr f32 GetLastAccess() const;

  // Set last access timestamp in seconds
  constexpr void SetLastAccess(f32 time);

  // Allow sorting in list for discarding
  constexpr bool operator<(const ResourceBase &rhs) const noexcept;

  virtual void Clear();

  virtual b8 Load();

  virtual void Destroy();

  // ================================================================================
  // Subclass must implements these
  // ================================================================================

  virtual void Reload() = 0;

  virtual void Unload() = 0;

  virtual u32 GetSize() const = 0;

  virtual b8 IsUnloaded() const = 0;

protected:
  PriorityType m_Priority;
  i32 m_RefCount;
  f32 m_LastAccess;
};

#endif // !SN_RESOURCE_BASE_H
