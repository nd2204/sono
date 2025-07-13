#ifndef SN_INDEX_BUFFER_H
#define SN_INDEX_BUFFER_H

#include "core/common/types.h"

enum IndexType : u8 { INDEX_TYPE_U16, INDEX_TYPE_U32 };

class IIndexBuffer {
public:
  virtual IndexType GetIndexType() const = 0;

  virtual u32 GetIndexCount() const = 0;
};

#endif // !SN_INDEX_BUFFER_H
