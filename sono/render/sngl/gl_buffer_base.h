#ifndef SN_GL_BUFFER_BASE_H
#define SN_GL_BUFFER_BASE_H

#include "render/buffer_base.h"

#include "glad/glad.h"

class GLBuffer : public IBuffer {
public:
  GLBuffer(GLenum target, BufferUsage usage, usize size);

  virtual ~GLBuffer();

  virtual void *Map() override;

  virtual void Unmap() override;

  virtual void Update(const void *data, usize size, usize offset = 0) override;

  virtual void Release() override;

  virtual u32 GetSize() const override;

  virtual BufferUsage GetUsage() const override;

  void Bind() const;

  void Unbind() const;

  GLuint GetID() const;

  operator u32() const;

private:
  GLuint m_BufferID;
  GLenum m_Target;
  usize m_Size;
  BufferUsage m_Usage;
};

#endif // !SN_GL_BUFFER_BASE_H
