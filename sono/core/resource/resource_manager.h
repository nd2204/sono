#ifndef SN_RESOURCE_MANAGER_H
#define SN_RESOURCE_MANAGER_H

#include "core/common/types.h"
#include "core/containers/itor_wrapper.h"
#include "core/resource/resource_base.h"
#include "core/resource/resource_handle.h"
#include "core/system.h"
#include <map>

template <typename TAG>
class ResourceManager : public System {
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
  // Resource iteration
  // ================================================================================
  inline b8 GotoBegin() { m_CurrRes = m_CurrRes.Begin(); }

  inline ResourcePtr GetCurrentResource() { return m_CurrRes.PeekVal(); }

  inline b8 GotoNext() {
    m_CurrRes.Advance();
    return IsValid();
  }

  inline b8 IsValid() const { m_CurrRes.HasMore(); }

  // ================================================================================
  // Resource Access
  // ================================================================================

  virtual ResourceHandle<TAG> Load(const std::string &name);

  virtual void Unload(ResourceHandle<TAG> handle);

  virtual void Unload(ResourcePtr resPtr);

  inline ResourcePtr GetResource(ResourceHandle<TAG> handle) {
    if (handle.IsNull()) return nullptr;
    ASSERT(handle.GetIndex() >= m_ResMap.size());
  }

  inline const ResourcePtr GetResource(ResourceHandle<TAG> handle) const {
    typedef ResourceManager<TAG> ThisType;
    return const_cast<ThisType *>(this)->GetResource(handle);
  }

protected:
  virtual void _Create() = 0;
  virtual void _Insert() = 0;
  virtual void _Remove() = 0;

protected:
  typedef std::vector<u32> FreeVec;
  typedef std::vector<u32> MagicVec;
  typedef std::vector<ResourcePtr> UserVec;

  const char *s_Name;

  ResourceHandle<TAG> m_NextHandle;
  // u32 m_CurrUsedMem;
  // u32 m_MaxMem;

  ResourceMapItor m_CurrRes;
  ResourceMap m_ResMap;
};

#endif // !SN_RESOURCE_MANAGER_H
