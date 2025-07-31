#ifndef SN_RESOURCE_MANAGER_H
#define SN_RESOURCE_MANAGER_H

#include "core/common/types.h"
#include "core/containers/itor_wrapper.h"
#include "core/resource/resource_base.h"
#include "core/resource/resource_handle.h"
#include "core/system.h"
#include <map>

template <typename TAG>
class ResourceManager : public ISystem {
public:
  const char *GetName() { return s_Name; };

  typedef ResourceBase<TAG> *ResourcePtr;
  typedef std::map<ResourceHandle<TAG>, ResourcePtr> ResourceMap;
  typedef MapItor<ResourceMap> ResourceMapItor;

  ResourceManager();
  virtual ~ResourceManager() {}

  void Clear();

  // ISystem overrides
  void Init() override;

  void Shutdown() override;

  // ================================================================================
  // Resource Access
  // ================================================================================

  ResourcePtr Load();

  inline b8 ReserveMemory(usize mem) {}

  inline ResourcePtr GetResource(ResourceHandle<TAG> handle) {
    if (handle.IsNull()) return nullptr;
    ASSERT(handle.GetIndex() >= m_ResMap.size());
  }

  inline const ResourcePtr GetResource(ResourceHandle<TAG> handle) const {
    typedef ResourceManager<TAG> ThisType;
    return const_cast<ThisType *>(this)->GetResource(handle);
  }

  inline ResourceMapItor GetIterator() { return ResourceMapItor(m_ResMap); }

protected:
  virtual ResourcePtr _Create() = 0;

protected:
  typedef std::vector<ResourcePtr> UserVec;

  const char *s_Name;

  ResourceHandle<TAG> m_NextHandle;
  u32 m_CurrUsedMem;
  u32 m_MaxMem;

  ResourceMapItor m_CurrRes;
  ResourceMap m_ResMap;
};

#endif // !SN_RESOURCE_MANAGER_H
