#include "core/math/math.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include <doctest.h>
#include <core/math/quaternion.h>

#include <glm/glm.hpp>
#include <sstream>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

inline bool ApproxEqual(f32 a, f32 b, float eps = 1e-5f) {
  return fabs(a + b) < eps || fabs(a - b) < eps;
}

// Helper: check approx equality
bool ApproxEqual(const Quaternion &a, const glm::quat &b, float eps = 1e-5f) {
  return ApproxEqual(a.x, b.x, eps)
    && ApproxEqual(a.y, b.y, eps)
    && ApproxEqual(a.z, b.z, eps)
    && ApproxEqual(a.w, b.w, eps);
}

// Helper: check approx equality
bool ApproxEqual(const Mat4 &a, const glm::mat4 &b, float eps = 1e-5f) {
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      if (!ApproxEqual(a[r][c], b[r][c], eps)) return false;
    }
  }
  return true;
}

template <int R, int C>
std::string glmMatToString(const glm::mat<R, C, float> &mat) {
  std::stringstream ss;
  ss << "mat" << R << "x" << C << "[";
  for (int i = 0; i < R; ++i) {
    ss << "[";
    for (int j = 0; j < C; ++j) {
      ss << std::to_string(mat[i][j]);
      if (j < C - 1) ss << ",";
    }
    ss << "]";
    if (i < R - 1) ss << ",";
  }
  ss << "]";
  return ss.str();
}

template <int N>
std::string glmVecToString(const glm::vec<N, float> &v) {
  std::ostringstream oss;
  oss << "vec" << N << "[";
  for (int i = 0; i < N - 1; i++) {
    oss << std::to_string(v[i]) << ",";
  }
  oss << std::to_string(v[N - 1]) << "]";
  return oss.str();
}

std::string glmQuatToString(const glm::quat &q) {
  std::stringstream ss;
  ss
    << "quat["
    << std::to_string(q.w)
    << ','
    << std::to_string(q.x)
    << ','
    << std::to_string(q.y)
    << ','
    << std::to_string(q.z)
    << ']';
  return ss.str();
}

TEST_SUITE("Math/Quaternion") {
  TEST_CASE("AxisAngle -> Quaternion") {
    glm::quat glmQ = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    Quaternion myQ = Quaternion::FromAxisAngle(Vec3(1, 0, 0), Sono::Radians(90.0f));

    INFO("glmQuat:  " << glmQuatToString(glmQ));
    INFO("sonoQuat: " << myQ.ToString());

    CHECK(ApproxEqual(myQ, glmQ));
  }

  TEST_CASE("Euler <-> Quaternion") {
    // yaw 90°
    glm::vec3 glmEuler(0, glm::radians(90.0f), 0);
    Vec3 myEuler(0, Sono::Radians(90.0f), 0);

    glm::quat glmQ = glm::quat(glmEuler);
    Quaternion myQ = Quaternion::FromEuler(myEuler); // your conversion result should go here

    SUBCASE("Euler -> Quaternion") {
      INFO("glmQuat:  " << glmQuatToString(glmQ));
      INFO("sonoQuat: " << myQ.ToString());

      CHECK(ApproxEqual(myQ, glmQ));
    }

    SUBCASE("Quaternion -> Euler") {
      Vec3 myE = myQ.ToEuler();

      INFO("originalEuler:  " << myEuler.ToString());
      INFO("recoveredEuler: " << myE.ToString());

      CHECK(ApproxEqual(myE.x, myEuler.x));
      CHECK(ApproxEqual(myE.y, myEuler.y));
      CHECK(ApproxEqual(myE.z, myEuler.z));
    }
  }

  TEST_CASE("Quaternion <-> Matrix") {

    SUBCASE("Quaternion -> Matrix") {
      glm::vec3 euler(0, glm::radians(90.0f), 0); // yaw 90°
      glm::mat4 glmM = glm::toMat4(glm::quat(euler));

      Vec3 myEuler(0, glm::radians(90.0f), 0);         // yaw 90°
      Quaternion myQ = Quaternion::FromEuler(myEuler); // your conversion result should go here
      Mat4 myM = myQ.ToMat4();                         // your conversion result should go here

      INFO("glm:  " << glmMatToString(glmM));
      INFO("sono: " << myM.ToString());

      CHECK(ApproxEqual(myM, glmM));
    }

    SUBCASE("Matrix -> Quaternion") {
      glm::quat glmQ = glm::quat_cast(glm::mat4(1.0f));
      Quaternion myQ = Quaternion::FromMatrix(Mat4(1.0f));

      INFO("glmQuat:  " << glmQuatToString(glmQ));
      INFO("sonoQuat: " << myQ.ToString());

      CHECK(ApproxEqual(myQ, glmQ));
    }
  }

  TEST_CASE("Quaternion x Quaternion (90 deg X then 90 deg Y)") {
    glm::quat glmA = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::quat glmB = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::quat glmC = glmA * glmB;

    Quaternion myA{glmA.w, glmA.x, glmA.y, glmA.z};
    Quaternion myB{glmB.w, glmB.x, glmB.y, glmB.z};
    Quaternion myC = myA * myB;

    INFO("glmQuatA:  " << glmQuatToString(glmA));
    INFO("sonoQuatA: " << myA.ToString());
    INFO("glmQuatB:  " << glmQuatToString(glmB));
    INFO("sonoQuatB: " << myB.ToString());
    INFO("glmQuatC:  " << glmQuatToString(glmC));
    INFO("sonoQuatC: " << myC.ToString());

    CHECK(ApproxEqual(myA, glmA));
    CHECK(ApproxEqual(myB, glmB));
    CHECK(ApproxEqual(myC, glmC));
  }
}
