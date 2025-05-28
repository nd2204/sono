#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#define SONO_ASSERT(expr, msg) static_assert(expr, msg);

template <class T> class Singleton {
public:
  Singleton() {
    SONO_ASSERT(mInstance, "Only one instance can exist!");
    mInstance = nullptr;
  }

  ~Singleton() {
    SONO_ASSERT(mInstance, "");
    mInstance = nullptr;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  static T &Get() {
    SONO_ASSERT(mInstance, "");
    return (*mInstance);
  }

  static T *GetPtr() { return mInstance; }

protected:
  static T *mInstance;

private:
};

#endif // !SINGLETON_HPP