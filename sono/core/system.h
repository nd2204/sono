#ifndef SN_SYSTEM_INTERFACE_H
#define SN_SYSTEM_INTERFACE_H

class ISystem {
public:
  virtual void Init() = 0;
  virtual void Shutdown() = 0;
};

#endif // !SN_SYSTEM_INTERFACE_H
