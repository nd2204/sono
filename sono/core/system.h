#ifndef SN_SYSTEM_INTERFACE_H
#define SN_SYSTEM_INTERFACE_H

#include <core/common/logger.h>

class ISystem {
public:
  virtual const char *GetName() { return "Unamed System"; }
  virtual void Init() { LOG_INFO_F("Initializing %s", GetName()); }
  virtual void Shutdown() { LOG_INFO_F("Shutting down %s", GetName()); }
};

#endif // !SN_SYSTEM_INTERFACE_H
