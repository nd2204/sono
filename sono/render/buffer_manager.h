#ifndef SN_BUFFER_MANAGER_H
#define SN_BUFFER_MANAGER_H

#include "core/common/singleton.h"

/// This class is responsible for managing buffers for rendering
/// All buffers creation and deletion should be called by this class
class BufferManager : public Singleton<BufferManager> {
public:
  BufferManager();
  virtual ~BufferManager();

private:
};

#endif // !SN_BUFFER_MANAGER_H
