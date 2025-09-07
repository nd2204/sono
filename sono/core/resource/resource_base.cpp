#include <core/resource/resource_base.h>

constexpr void ResourceBase::Acquire() { ++m_RefCount; }
// --------------------------------------------------------------------------------
constexpr void ResourceBase::Release() {
  if (--m_RefCount == 0) Unload();
}
// --------------------------------------------------------------------------------
constexpr u32 ResourceBase::GetRefCount() const { return m_RefCount; }
// --------------------------------------------------------------------------------
constexpr bool ResourceBase::IsLocked() const { return m_RefCount > 0; }
// --------------------------------------------------------------------------------
constexpr ResourceBase::PriorityType ResourceBase::GetPriority() const { return m_Priority; }
// --------------------------------------------------------------------------------
constexpr void ResourceBase::SetPriority(PriorityType priority) { m_Priority = priority; }
// --------------------------------------------------------------------------------
constexpr f32 ResourceBase::GetLastAccess() const { return m_LastAccess; }
// --------------------------------------------------------------------------------
constexpr void ResourceBase::SetLastAccess(f32 time) { m_LastAccess = time; }
// --------------------------------------------------------------------------------
void ResourceBase::Clear() {}
// --------------------------------------------------------------------------------
b8 ResourceBase::Load() { return false; }
// --------------------------------------------------------------------------------
void ResourceBase::Destroy() {}
// --------------------------------------------------------------------------------
constexpr bool ResourceBase::operator<(const ResourceBase &rhs) const noexcept {
  if (GetPriority() == rhs.GetPriority()) {
    if (GetLastAccess() == rhs.GetLastAccess()) {
      return GetSize() < rhs.GetSize();
    } else {
      return GetLastAccess() < rhs.GetLastAccess();
    }
  }
  return GetPriority() < rhs.GetPriority();
}
