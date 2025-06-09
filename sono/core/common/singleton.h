#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "../common/snassert.h"

template <class T>
class Singleton {
public:
  Singleton() {
    // Assert error when instance is not null
    SN_ASSERT(!m_sInstance, "Only one instance can exist!");
    m_sInstance = static_cast<T *>(this);
  }

  ~Singleton() {
    ASSERT(m_sInstance);
    m_sInstance = nullptr;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  static T &Get() {
    ASSERT(m_sInstance);
    return (*m_sInstance);
  }

  static T *GetPtr() { return m_sInstance; }

protected:
  static T *m_sInstance;

private:
};

#endif // !SINGLETON_HPP