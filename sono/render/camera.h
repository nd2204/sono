#ifndef CAMERA_H
#define CAMERA_H

#include "core/math/mat4.h"
#include "core/math/vec3.h"

class Camera {
public:
  Camera();

  ~Camera();

  /// NOTE: change Vec3 to Point3 in the future?
  void LookAt(const Vec3 &pos);

  void SetPosition(const Vec3 &newPos);

  Mat4 GetViewProjectionMatrix() const;

private:
  Mat4 m_ViewMatrix;
  Mat4 m_ProjectionMatrix;
  Mat4 m_Frustum;
  Vec3 m_Position;
};

#endif // !CAMERA_H
