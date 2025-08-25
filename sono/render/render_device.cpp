#include "render/render_device.h"

// --------------------------------------------------------------------------------
RenderDevice::RenderDevice(Allocator *allocator)
  : m_pAllocator(allocator) {}
// --------------------------------------------------------------------------------
RenderDevice::~RenderDevice() {}
// --------------------------------------------------------------------------------
void RenderDevice::DeleteAllBuffers() {
  for (auto it = m_Buffers.begin(); it != m_Buffers.end();) {
    if (DeleteBuffer(*it))
      it = m_Buffers.erase(it);
    else {
      it++;
    }
  }
}
