#ifndef SYSTEM_H
#define SYSTEM_H

class SubSystem {
public:
  virtual ~SubSystem() = default;

  // Initialize the subsystem
  virtual void Init() = 0;

  // Shutdown the subsystem
  virtual void Shutdown() = 0;
};

#endif // !SYSTEM_H