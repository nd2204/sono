#ifndef SN_RESOURCE_MANAGER_H
#define SN_RESOURCE_MANAGER_H

#include <core/common/types.h>
#include <core/containers/itor_wrapper.h>
#include <core/memory/allocators/pool.h>
#include <core/resource/resource_base.h>
#include <core/resource/resource_handle.h>
#include <core/system.h>
#include <map>

using ResourceHandle = Handle::HandleType;

template <class ResType>
class ResourceManager : public System {
public:
  using ResourcePtr = ResourceBase *;
  using ResourceMap = std::map<ResourceHandle, ResourcePtr>;
  using ResourceMapItor = MapItor<ResourceMap>;

public:
  ResourceManager();

  virtual ~ResourceManager() {}

  void Clear();

  // ISystem overrides
  void Init() override;

  void Shutdown() override;

  inline const char *GetName() const override { return s_Name; };

  void SetMemoryBudget(u32 budget);

  // ================================================================================
  // Resource iteration
  // ================================================================================

  inline ResourcePtr GetCurrentResource() { return m_CurrRes.PeekVal(); }

  inline ResourceMapItor GetCurrentResourceItor() { return m_CurrRes; }

  // ================================================================================
  // Resource Access and lifetime
  // ================================================================================

  virtual Handle::HandleType Load(const std::string &name);

  virtual void Unlock(ResourceHandle handle);

  virtual void Unlock(ResourcePtr resPtr);

  inline ResourcePtr GetResource(ResourceHandle handle) {
    if (Handle::IsValid(handle)) return nullptr;
    ASSERT(Handle::GetIndex(handle) >= m_ResMap.size());
    return m_ResMap.at(Handle::GetIndex(handle));
  }

protected:
  virtual void CreateImpl() = 0;
  virtual void InsertImpl() = 0;
  virtual void RemoveImpl() = 0;

protected:
  const char *s_Name;

  ResourceHandle m_NextHandle;
  ResourceMapItor m_CurrRes;
  ResourceMap m_ResMap;
  u32 m_CurrUsedMem;
  u32 m_MaxMem;
  // PoolAllocator<> m_Allocator;
};

#endif // !SN_RESOURCE_MANAGER_H
