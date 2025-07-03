#include "camera.h"

Camera::Camera() {}

Camera::~Camera() {}

void Camera::LookAt(const Vec3 &pos) {}

void Camera::SetPosition(const Vec3 &newPos) {}

Mat4 Camera::GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }
