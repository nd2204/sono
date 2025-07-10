#ifndef SN_INPUT_DEVICE_H
#define SN_INPUT_DEVICE_H

#include "core/common/types.h"

#include <string>

class InputDevice {
public:
protected:
  u32 m_DeviceID;
  std::string m_DeviceName;
};

#endif // !SN_INPUT_DEVICE_H
