#ifndef SN_GL_BUFFER_BASE_H
#define SN_GL_BUFFER_BASE_H

#include <render/buffer_base.h>
#include <glad/glad.h>

class GLBuffer : public Buffer {
public:
  GLBuffer(const BufferDesc &desc);

  virtual ~GLBuffer();

  virtual void *Map() override;

  virtual void Unmap() override;

  virtual void Update(const void *data, usize size, usize offset = 0) override;

  virtual void Release() override;

  virtual void Bind() const override;

  virtual void Unbind() const override;

  GLuint GetID() const;

  operator u32() const;

private:
  GLuint m_BufferID;
  GLenum m_Target;
  GLenum m_GlUsage;
};

#endif // !SN_GL_BUFFER_BASE_H
