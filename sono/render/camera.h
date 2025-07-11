#ifndef CAMERA_H
#define CAMERA_H

#include "core/math/mat4.h"
#include "core/math/vec3.h"
#include "render/render_context.h"

enum class ProjectionType { PERSPECTIVE, ORTHOGONAL };

class Camera {
public:
  Camera();

  ~Camera();

  /// NOTE: change Vec3 to Point3 in the future?
  void LookAt(const Vec3 &pos);

  void SetPosition(f32 xpos, f32 ypos, f32 zpos);

  void SetPosition(const Vec3 &newPos);

  void Rotate(const Vec3 &euler);

  void SetEulerAngles(f32 yaw, f32 pitch, f32 roll);

  void SetEulerAngles(const Vec3 &euler);

  const Vec3 &GetEulerAngles() const;

  void Move(const Vec3 &v);

  void SetPerspective(Radian fov, f32 aspect, f32 zNear, f32 zFar);

  void SetOrthogonal();

  const Vec3 &GetForward() const;

  const Vec3 &GetRight() const;

  const Vec3 &GetUp() const;

  const Vec3 &GetPosition() const;

  const Mat4 &GetViewMatrix() const;

  const Mat4 &GetProjectionMatrix() const;

private:
  void UpdateView();

private:
  Mat4 m_ViewMatrix;
  Mat4 m_Frustum;
  Mat4 m_ProjectionMatrix;

  Vec3 m_Position;
  Vec3 m_Forward;
  Vec3 m_Right;
  Vec3 m_Up;
  Vec3 m_Rotations;

  f32 m_zNear;
  f32 m_zFar;
  f32 m_Aspect;
  Radian m_FOV;

  ProjectionType m_ProjType;
};

#endif // !CAMERA_H
