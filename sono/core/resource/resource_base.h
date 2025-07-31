#ifndef SN_RESOURCE_BASE_H
#define SN_RESOURCE_BASE_H

#include "core/common/types.h"
#include <ctime>

template <typename TAG>
class ResourceManager;

template <typename TAG>
class ResourceBase {
public:
  friend class ResourceManager<TAG>;

  enum PriorityType {
    RES_PRIORITY_0 = 0,
    RES_PRIORITY_1,
    RES_PRIORITY_2,
    RES_PRIORITY_MAX // maximum priority thresshold
  };

  ResourceBase() { Clear(); }
  virtual ~ResourceBase() { Destroy(); }

  virtual void Clear();

  virtual b8 Create() { return false; }

  virtual void Destroy() {}

  inline void SetPriority(PriorityType priority) { m_Priority = priority; }

  inline PriorityType GetPriority() const { return m_Priority; }

  inline void SetRefCount(u32 refCount) { m_RefCount = refCount; }

  inline u32 GetRefCount() const { return m_RefCount; }

  // Return true if the resource is in use
  inline bool IsLocked() const { return m_RefCount > 0; }

  // Allow sorting in list for discarding
  bool operator<(const ResourceBase &rhs);

  // ================================================================================
  // Subclass must implements these
  // ================================================================================

  virtual void Recreate() = 0;

  virtual void Dispose() = 0;

  virtual u32 GetSize() const = 0;

  virtual b8 IsDisposed() const = 0;

protected:
  PriorityType m_Priority;
  u32 m_RefCount;
};

template <typename TAG>
inline void ResourceBase<TAG>::Clear() {}

template <typename TAG>
inline bool ResourceBase<TAG>::operator<(const ResourceBase &rhs) {
  if (GetPriority() == rhs.GetPriority()) {
    return GetSize() < rhs.GetSize();
  } else {
    return GetPriority() < rhs.GetPriority();
  }
}

#endif // !SN_RESOURCE_BASE_H
