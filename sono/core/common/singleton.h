#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "../common/snassert.h"

template <class T>
class Singleton {
public:
  Singleton() {
    SN_ASSERT(mInstance, "Only one instance can exist!");
    mInstance = nullptr;
  }

  ~Singleton() {
    ASSERT(mInstance);
    mInstance = nullptr;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  static T &Get() {
    ASSERT(mInstance);
    return (*mInstance);
  }

  static T *GetPtr() { return mInstance; }

protected:
  static T *mInstance;

private:
};

#endif // !SINGLETON_HPP