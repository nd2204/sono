#include <core/math/transform.h>
#include <sstream>

// --------------------------------------------------------------------------------
void Transform::Move(const Vec3 &translation, CoordSpace relSpace) {
  if (relSpace == CoordSpace::LOCAL) {
    m_Position +=
      GetRight() * translation.x + GetUp() * translation.y + GetForward() * translation.z;
  } else {
    m_Position += translation;
  }

  m_ModelMatrix.SetTranslation(m_Position);
}
// --------------------------------------------------------------------------------
const Vec3 &Transform::GetPosition() const { return m_Position; }
// --------------------------------------------------------------------------------
void Transform::SetPosition(const Vec3 &newPos) {
  m_Position = newPos;
  m_ModelMatrix.SetTranslation(m_Position);
}
// --------------------------------------------------------------------------------
Vec3 Transform::GetForward() const {
  return m_Rotation * Vec3::Forward; // forward is the local -z axis
}
// --------------------------------------------------------------------------------
Vec3 Transform::GetRight() const {
  return m_Rotation * Vec3::Right; // right is the local +x axis
}
// --------------------------------------------------------------------------------
Vec3 Transform::GetUp() const {
  return m_Rotation * Vec3::Up; // up is the local +y axis
}
// --------------------------------------------------------------------------------
const Quaternion &Transform::GetRotation() const { return m_Rotation; }
// --------------------------------------------------------------------------------
void Transform::SetRotation(const Quaternion &q) {
  m_Rotation = q;
  m_EditorEuler = Sono::Degrees(m_Rotation.ToEuler()); // sync for inspector
  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
void Transform::Rotate(const Quaternion &q, CoordSpace relSpace) {
  switch (relSpace) {
    case CoordSpace::GLOBAL:
      m_Rotation = q * m_Rotation;
      break;
    case CoordSpace::LOCAL:
      m_Rotation = m_Rotation * q;
      break;
  }
  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
void Transform::RotateAxis(const Vec3 &axis, Radian angleRad, CoordSpace relSpace) {
  Rotate(Quaternion(angleRad, axis.Normalized()), relSpace);
}
// --------------------------------------------------------------------------------
void Transform::LookAt(const Transform &target, const Vec3 &worldUp) {
  LookAt(target.GetPosition(), worldUp);
}
// --------------------------------------------------------------------------------
void Transform::LookAt(const Vec3 &worldPos, const Vec3 &worldUp) {
  Vec3 forward = (worldPos - m_Position).Normalized();
  Vec3 right = worldUp.Cross(forward).Normalized();
  Vec3 up = forward.Cross(right);

  m_Rotation = Quaternion::FromMatrix(
    Mat3(right.x, right.y, right.z, up.x, up.y, up.z, forward.x, forward.y, forward.z)
  );
  m_EditorEuler = Sono::Degrees(m_Rotation.ToEuler());

  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
const Vec3 &Transform::GetEuler() const { return m_EditorEuler; }
// --------------------------------------------------------------------------------
void Transform::SetEuler(const Vec3 &eulerDeg) {
  m_EditorEuler = eulerDeg;
  Vec3 eulerRad = Sono::Radians(eulerDeg);
  m_Rotation = Quaternion::FromEuler(eulerRad);
  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
const Vec3 &Transform::GetScale() const { return m_Scale; }
// --------------------------------------------------------------------------------
void Transform::Scale(const Vec3 &scale) {
  m_Scale = scale;
  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
b8 Transform::IsDirty() const { return m_IsDirty; }
// --------------------------------------------------------------------------------
Mat4 Transform::GetLocalModelMatrix() const {
  return Mat4::Scale(m_Scale) * m_Rotation.ToMat4() * Mat4::Translation(m_Position);
}
// --------------------------------------------------------------------------------
const Mat4 &Transform::GetModelMatrix() const { return m_ModelMatrix; }
// --------------------------------------------------------------------------------
void Transform::UpdateModelMatrix() {
  m_ModelMatrix = GetLocalModelMatrix();
  m_IsDirty = false;
}
// --------------------------------------------------------------------------------
void Transform::UpdateModelMatrix(const Mat4 &parentModelMat) {
  m_ModelMatrix = parentModelMat * GetLocalModelMatrix();
  m_IsDirty = false;
}
// --------------------------------------------------------------------------------
std::string Transform::ToString() const {
  std::ostringstream oss;
  oss
    << "\"position\": "
    << m_Position.ToString()
    << ",\n"
    << "\"rotation\": "
    << m_Rotation.ToString()
    << ",\n"
    << "\"scale\": "
    << m_Scale.ToString();
  return oss.str();
}
