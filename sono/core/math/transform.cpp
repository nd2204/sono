#include "transform.h"
#include "core/common/logger.h"
#include "math.h"

#include <algorithm>
#include <sstream>

void Transform::LookAt(const Transform &target) { LookAt(target.GetPosition()); }
// --------------------------------------------------------------------------------
void Transform::LookAt(const Vec3 &worldPos) {
  Vec3 dir = (m_Position - worldPos).Normalized();

  m_EulerRot.pitch = Sono::Degrees(asin(-dir.y));
  m_EulerRot.yaw = Sono::Degrees(atan2(-dir.z, -dir.x));

  UpdateRotation();
  UpdateModelMatrix();
}
// --------------------------------------------------------------------------------
void Transform::Move(const Vec3 &translation, CoordSpace relSpace) {
  if (relSpace == CoordSpace::LOCAL) {
    m_Position += m_Right * translation.x + m_Up * translation.y + m_Forward * translation.z;
  } else {
    m_Position += translation;
  }

  m_ModelMatrix.SetTranslation(m_Position);
}
// --------------------------------------------------------------------------------
std::string Transform::ToString() const {
  std::ostringstream oss;
  oss
    << "\"position\": "
    << m_Position.ToString()
    << ",\n"
    << "\"rotation\": "
    << m_EulerRot.ToString()
    << ",\n"
    << "\"scale\": "
    << m_Scale.ToString();
  return oss.str();
}
// --------------------------------------------------------------------------------
void Transform::UpdateRotation() {
  Vec3 _forward(
    cos(Sono::Radians(m_EulerRot.yaw)) * cos(Sono::Radians(m_EulerRot.pitch)),
    sin(Sono::Radians(m_EulerRot.pitch)),
    sin(Sono::Radians(m_EulerRot.yaw)) * cos(Sono::Radians(m_EulerRot.pitch))
  );

  m_Forward = _forward.Normalized();
  m_Right = m_Forward.Cross(Vec3::Up).Normalized();
  m_Up = m_Right.Cross(m_Forward);
}
// --------------------------------------------------------------------------------
void Transform::UpdateModelMatrix() {
  m_ModelMatrix =
    Mat4::Scale(m_Scale) * Mat4::RotationFromEuler(m_EulerRot) * Mat4::Translation(m_Position);
}
