#ifndef SN_RESOURCE_BASE_H
#define SN_RESOURCE_BASE_H

#include "core/common/types.h"
#include <ctime>

class ResourceBase {
public:
  friend class ResourceManager;

  enum PriorityType {
    RES_PRIORITY_0 = 0,
    RES_PRIORITY_1,
    RES_PRIORITY_2,
    RES_PRIORITY_MAX // maximum priority thresshold
  };

  ResourceBase() { Clear(); }

  virtual ~ResourceBase() {}

  void SetPriority(PriorityType priority);

  PriorityType GetPriority() const;

  void SetRefCount(u32 refCount);

  // Return the reference count of the current resource
  u32 GetRefCount() const;

  // Return true if the current resource is in use
  bool IsLocked() const;

  // Allow sorting in list for discarding
  bool operator<(const ResourceBase &rhs);

  virtual void Clear();

  virtual b8 Load();

  virtual void Destroy();

  // ================================================================================
  // Subclass must implements these
  // ================================================================================

  virtual void Reload() = 0;

  virtual void Unload() = 0;

  virtual u32 GetSize() const = 0;

  virtual b8 IsDisposed() const = 0;

protected:
  PriorityType m_Priority;
  u32 m_RefCount;
};

#endif // !SN_RESOURCE_BASE_H
