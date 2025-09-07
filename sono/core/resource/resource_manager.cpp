#include <core/resource/resource_manager.h>
#include <core/common/logger.h>
#include <core/memory/memory_system.h>

#include <memory>

template <class ResType>
ResourceManager<ResType>::ResourceManager()
  : m_CurrRes(ResourceMapItor(m_ResMap))
  , m_MaxMem(SN_MEM_MIB * 100)
/* , m_Allocator(m_MaxMem, sizeof(ResourceBase), 16)*/ {
  Clear();
}
// --------------------------------------------------------------------------------
template <class ResType>
void ResourceManager<ResType>::Clear() {}
// --------------------------------------------------------------------------------
template <class ResType>
void ResourceManager<ResType>::Init() {
  ENGINE_INFO("|ResourceManager| Initializing %s ...", s_Name);
}
// --------------------------------------------------------------------------------
template <class ResType>
void ResourceManager<ResType>::Shutdown() {
  ENGINE_INFO("|ResourceManager| Shutting down %s ...", s_Name);
}
// --------------------------------------------------------------------------------
template <class ResType>
ResourceHandle ResourceManager<ResType>::Load(const std::string &name) {}
// --------------------------------------------------------------------------------
template <class ResType>
void ResourceManager<ResType>::Unlock(ResourceHandle handle) {}
// --------------------------------------------------------------------------------
template <class ResType>
void ResourceManager<ResType>::Unlock(ResourcePtr resPtr) {}
