#include <render/render_device.h>
#include <core/memory/memory_system.h>

constexpr u32 kFrameMemorySize = SN_MEM_MIB;

// --------------------------------------------------------------------------------
RenderDevice::RenderDevice(Allocator *resAllocator)
  : queue(nullptr)
  , m_pResourceAllocator(resAllocator)
  , m_FrameAllocator(kFrameMemorySize) {}
// --------------------------------------------------------------------------------
RenderDevice::~RenderDevice() { m_FrameAllocator.FreeInternalBuffer(); }
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
