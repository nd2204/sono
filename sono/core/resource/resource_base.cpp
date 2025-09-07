#include <core/resource/resource_base.h>

// --------------------------------------------------------------------------------
void ResourceBase::SetPriority(PriorityType priority) { m_Priority = priority; }
// --------------------------------------------------------------------------------
ResourceBase::PriorityType ResourceBase::GetPriority() const { return m_Priority; }
// --------------------------------------------------------------------------------
void ResourceBase::SetRefCount(u32 refCount) { m_RefCount = refCount; }
// --------------------------------------------------------------------------------
u32 ResourceBase::GetRefCount() const { return m_RefCount; }
// --------------------------------------------------------------------------------
bool ResourceBase::IsLocked() const { return m_RefCount > 0; }
// --------------------------------------------------------------------------------
void ResourceBase::Clear() {}
// --------------------------------------------------------------------------------
b8 ResourceBase::Load() { return false; }
// --------------------------------------------------------------------------------
void ResourceBase::Destroy() {}
// --------------------------------------------------------------------------------
bool ResourceBase::operator<(const ResourceBase &rhs) {
  if (GetPriority() == rhs.GetPriority()) {
    return GetSize() < rhs.GetSize();
  } else {
    return GetPriority() < rhs.GetPriority();
  }
}
