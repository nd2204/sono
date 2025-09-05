#ifndef SN_TRANSFORM_H
#define SN_TRANSFORM_H

#include "core/math/quaternion.h"
#include "mat4.h"
#include "vec3.h"
#include <string>

enum class CoordSpace { LOCAL, GLOBAL };

class Transform {
public:
  Transform()
    : m_Position{0.0f, 0.0f, 0.0f}
    , m_Scale(1.0f, 1.0f, 1.0f)
    , m_ModelMatrix(1.0f) {}

  explicit Transform(const Vec3 &pos, const Vec3 &rot = Vec3::Zero, const Vec3 &scl = Vec3(1.0f)) {
    m_Position = pos;
    m_Rotation = Quaternion::FromEuler(rot);
    m_Scale = scl;

    UpdateModelMatrix();
  }

  // --------------------------------------------------------------------------------
  // Positions
  // --------------------------------------------------------------------------------

  const Vec3 &GetPosition() const;

  void SetPosition(const Vec3 &newPos);

  void Move(const Vec3 &translation, CoordSpace relSpace = CoordSpace::LOCAL);

  // --------------------------------------------------------------------------------
  // Orientation manipulation, query methods
  // --------------------------------------------------------------------------------

  void LookAt(const Transform &target);

  void LookAt(const Vec3 &worldPos);

  Vec3 GetForward() const;

  Vec3 GetRight() const;

  Vec3 GetUp() const;

  Vec3 GetEulerRotation() const;

  // Set the euler angle in radians
  void SetEulerRotation(const Vec3 &eulerRot);

  // --------------------------------------------------------------------------------
  // Scale
  // --------------------------------------------------------------------------------

  const Vec3 &GetScale() const;

  void SetScale(const Vec3 &scale);

  // --------------------------------------------------------------------------------
  // Others ...
  // --------------------------------------------------------------------------------

  b8 IsDirty() const;

  Mat4 GetLocalModelMatrix() const;

  void UpdateModelMatrix();

  void UpdateModelMatrix(const Mat4 &parentModelMat);

  const Mat4 &GetModelMatrix() const;

  std::string ToString() const;

private:
  Vec3 m_Position;
  Quaternion m_Rotation;
  Vec3 m_Scale;

  Mat4 m_ModelMatrix;

  // TODO: Make use of this flags after scene graph implementation
  b8 m_IsDirty;
};

#endif // !SN_TRANSFORM_H
