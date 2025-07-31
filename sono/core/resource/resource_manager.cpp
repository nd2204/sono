#include "resource_manager.h"

#include "core/common/logger.h"

#include <memory>

template <typename TAG>
ResourceManager<TAG>::ResourceManager()
  : m_CurrRes(ResourceMapItor(m_ResMap)) {
  Clear();
}
// --------------------------------------------------------------------------------
template <typename TAG>
void ResourceManager<TAG>::Clear() {}
// --------------------------------------------------------------------------------
template <typename TAG>
void ResourceManager<TAG>::Init() {
  LOG_INFO_F("[ResourceManager] Initializing %s ...", s_Name);
}
// --------------------------------------------------------------------------------
template <typename TAG>
void ResourceManager<TAG>::Shutdown() {
  LOG_INFO_F("[ResourceManager] Shutting down %s ...", s_Name);
}
// --------------------------------------------------------------------------------
template <typename TAG>
ResourceHandle<TAG> ResourceManager<TAG>::Load(const std::string &name) {
  ResourceHandle<TAG> handle;
  return handle;
}
// --------------------------------------------------------------------------------
template <typename TAG>
void ResourceManager<TAG>::Unload(ResourceHandle<TAG> handle) {}
// --------------------------------------------------------------------------------
template <typename TAG>
void ResourceManager<TAG>::Unload(ResourcePtr resPtr) {}
