#ifndef SN_TRANSFORM_H
#define SN_TRANSFORM_H

#include "mat4.h"
#include "vec3.h"
#include <string>

enum class CoordSpace { LOCAL, GLOBAL };

class Transform {
public:
  Transform()
    : m_Position{0.0f, 0.0f, 0.0f}
    , m_EulerRot(0.0f, 0.0f, 0.0f)
    , m_Scale(1.0f, 1.0f, 1.0f)
    , m_Up(0.0f, 1.0f, 0.0f)
    , m_Right(1.0f, 0.0f, 0.0f)
    , m_Forward(0.0f, 0.0f, 1.0f)
    , m_ModelMatrix(1.0f) {}

  explicit Transform(const Vec3 &pos, const Vec3 &rot = Vec3::Zero, const Vec3 &scl = Vec3(1.0f)) {
    m_Position = pos;
    m_EulerRot = rot;
    m_Scale = scl;

    UpdateRotation();
    UpdateModelMatrix();
  }

  void LookAt(const Transform &target);
  void LookAt(const Vec3 &worldPos);

  void Move(const Vec3 &translation, CoordSpace relSpace = CoordSpace::LOCAL);

  inline const Mat4 &GetModelMatrix() const { return m_ModelMatrix; }
  inline const Vec3 &GetForward() const { return m_Forward; }
  inline const Vec3 &GetRight() const { return m_Right; }
  inline const Vec3 &GetUp() const { return m_Up; }

  inline const Vec3 &GetPosition() const { return m_Position; }
  inline Vec3 &GetPosition() { return m_Position; }

  inline void SetPosition(const Vec3 &newPos) {
    m_Position = newPos;
    m_ModelMatrix.SetTranslation(newPos);
  }

  inline const Vec3 &GetEulerRotation() const { return m_EulerRot; }
  inline Vec3 &GetEulerRotation() { return m_EulerRot; }
  inline void SetEulerRotation(const Vec3 &eulerRot) {
    m_EulerRot = eulerRot;
    UpdateRotation();
    UpdateModelMatrix();
  }

  inline const Vec3 &GetScale() const { return m_Scale; }
  inline Vec3 &GetScale() { return m_Scale; }
  inline void SetScale(const Vec3 &scale) {
    m_Scale = scale;
    UpdateModelMatrix();
  }

  std::string ToString() const;

private:
  void UpdateRotation();
  void UpdateModelMatrix();

private:
  Vec3 m_Position;
  Vec3 m_EulerRot;
  Vec3 m_Scale;

  Vec3 m_Up;
  Vec3 m_Right;
  Vec3 m_Forward;

  Mat4 m_ModelMatrix;
};

#endif // !SN_TRANSFORM_H
