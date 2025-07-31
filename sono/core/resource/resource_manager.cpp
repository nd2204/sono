#include "resource_manager.h"

#include "core/common/logger.h"

#include <memory>

template <typename TAG>
ResourceManager<TAG>::ResourceManager() {}

template <typename TAG>
void ResourceManager<TAG>::Init() {
  LOG_INFO_F("[ResourceManager] Initializing %s ...", s_Name);
}

template <typename TAG>
void ResourceManager<TAG>::Shutdown() {
  LOG_INFO_F("[ResourceManager] Shutting down %s ...", s_Name);
}

template <typename TAG>
typename ResourceManager<TAG>::ResourcePtr ResourceManager<TAG>::Load() {
  return nullptr;
}
