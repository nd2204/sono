#ifndef SN_SYSTEM_INTERFACE_H
#define SN_SYSTEM_INTERFACE_H

#include <core/common/logger.h>

class System {
public:
  virtual const char *GetName() const { return "Unamed System"; }
  virtual void Init() { ENGINE_INFO("Initializing %s", GetName()); }
  virtual void Shutdown() { ENGINE_INFO("Shutting down %s", GetName()); }
};

#endif // !SN_SYSTEM_INTERFACE_H
