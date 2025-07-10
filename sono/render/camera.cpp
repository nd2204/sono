#include "camera.h"

Camera::Camera() {
  m_Position = Vec3(0.0f, 3.0f, 3.0f);

  m_FOV = Sono::Radians(80.0f);
  m_zNear = 0.1f;
  m_zFar = 100.0f;
  m_Aspect = 1.7777777778;
  m_ProjectionMatrix = Mat4::Perspective(m_FOV, m_Aspect, m_zNear, m_zFar);

  LookAt(Vec3::Zero);

  m_ProjType = ProjectionType::PERSPECTIVE;
}
// --------------------------------------------------------------------------------
Camera::~Camera() {}
// --------------------------------------------------------------------------------
void Camera::LookAt(const Vec3 &target) {
  // since the camera point in the negative z-axis,
  // so dir point at the opposite direction
  Vec3 dir = (m_Position - target).Normalized();

  m_Rotations.yaw = Sono::Degrees(atan2(-dir.z, dir.x));
  m_Rotations.pitch = Sono::Degrees(asin(-dir.y));

  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::SetPosition(f32 xpos, f32 ypos, f32 zpos) {
  m_Position = Vec3(xpos, ypos, zpos);
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::SetPosition(const Vec3 &newPos) {
  m_Position = newPos;
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::SetEulerAngles(f32 yaw, f32 pitch, f32 roll) {
  m_Rotations.yaw = yaw;
  m_Rotations.pitch = pitch;
  m_Rotations.roll = roll;
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::SetEulerAngles(const Vec3 &euler) {
  m_Rotations = euler;
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::Rotate(const Vec3 &euler) {
  m_Rotations += euler;
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::Move(const Vec3 &v) {
  m_Position += v;
  UpdateView();
}
// --------------------------------------------------------------------------------
void Camera::SetPerspective(Radian fov, f32 aspect, f32 zNear, f32 zFar) {
  m_ProjType = ProjectionType::PERSPECTIVE;
  m_FOV = fov;
  m_Aspect = aspect;
  m_zNear = zNear;
  m_zFar = zFar;
  m_ProjectionMatrix = Mat4::Perspective(fov, aspect, zNear, zFar);
}
// --------------------------------------------------------------------------------
void Camera::SetOrthogonal() {
  m_ProjType = ProjectionType::PERSPECTIVE;
  // m_ProjectionMatrix = Mat4::Ortho();
}
// --------------------------------------------------------------------------------
const Vec3 &Camera::GetForward() const { return m_Forward; }
// --------------------------------------------------------------------------------
const Vec3 &Camera::GetRight() const { return m_Right; }
// --------------------------------------------------------------------------------
const Vec3 &Camera::GetUp() const { return m_Up; }
// --------------------------------------------------------------------------------
const Vec3 &Camera::GetEulerAngles() const { return m_Rotations; }
// --------------------------------------------------------------------------------
const Vec3 &Camera::GetPosition() const { return m_Position; }
// --------------------------------------------------------------------------------
const Mat4 &Camera::GetViewMatrix() const { return m_ViewMatrix; }
// --------------------------------------------------------------------------------
const Mat4 &Camera::GetProjectionMatrix() const { return m_ProjectionMatrix; }
// --------------------------------------------------------------------------------
void Camera::UpdateView() {
  Vec3 forward(
    cos(Sono::Radians(m_Rotations.yaw)) * cos(Sono::Radians(m_Rotations.pitch)),
    sin(Sono::Radians(m_Rotations.pitch)),
    sin(Sono::Radians(m_Rotations.yaw)) * cos(Sono::Radians(m_Rotations.pitch))
  );
  m_Forward = forward.Normalized();
  m_Right = m_Forward.Cross(Vec3::Up).Normalized();
  m_Up = m_Right.Cross(m_Forward);

  m_ViewMatrix = Mat4::LookAt(m_Position, m_Position + m_Forward, m_Up);
}
