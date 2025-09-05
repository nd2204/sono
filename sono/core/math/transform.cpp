#include <core/math/transform.h>
#include <sstream>

void Transform::LookAt(const Transform &target) { LookAt(target.GetPosition()); }
// --------------------------------------------------------------------------------
void Transform::LookAt(const Vec3 &worldPos) {
  Vec3 forward = (worldPos - m_Position).Normalized();
  Vec3 right = forward.Cross(Vec3::Up).Normalized();
  Vec3 up = right.Cross(forward).Normalized();

  m_Rotation = Quaternion::FromMat3(Mat3(forward, right, up));
  m_IsDirty = true;
}
// --------------------------------------------------------------------------------
Vec3 Transform::GetEulerRotation() const { return m_Rotation.ToEuler(); }
// --------------------------------------------------------------------------------
void Transform::SetEulerRotation(const Vec3 &eulerRot) {
  m_Rotation = Quaternion::FromEuler(eulerRot);
  m_IsDirty = true;
}
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
const Vec3 &Transform::GetScale() const { return m_Scale; }
// --------------------------------------------------------------------------------
void Transform::SetScale(const Vec3 &scale) {
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
