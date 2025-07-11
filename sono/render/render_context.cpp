#include "render_context.h"

// --------------------------------------------------------------------------------
RenderContext::RenderContext(u32 width, u32 height)
  : m_Width(width)
  , m_Height(height) {};
// --------------------------------------------------------------------------------
RenderContext::~RenderContext() {};
// --------------------------------------------------------------------------------
u32 RenderContext::GetWidth() const { return this->m_Width; }
// --------------------------------------------------------------------------------
u32 RenderContext::GetHeight() const { return this->m_Height; }
// --------------------------------------------------------------------------------
f32 RenderContext::GetAspect() const { return (f32)this->m_Width / (f32)this->m_Height; }
