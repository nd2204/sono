#ifndef SN_VERTEX_TYPE_H
#define SN_VERTEX_TYPE_H

#include "vertex_layout.h"

#include "render/colors.h"
#include "core/math/vec4.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"

struct VertexP {
  Vec3 position;

  VertexP() = default;

  VertexP(const Vec3 &pos)
    : position(pos) {}

  VertexP(f32 x, f32 y, f32 z)
    : position(x, y, z) {}
};

// Vertex struct with 1 Position and 1 Color
struct VertexPC {
  Vec3 position;
  Color color;

  VertexPC() = default;

  VertexPC(const Vec3 &pos, const Color &col)
    : position(pos)
    , color(col) {}

  VertexPC(f32 x, f32 y, f32 z, u32 col)
    : position(x, y, z)
    , color(col) {}
};

struct VertexPT {
  f32 x, y, z;
  f32 u, v;

  VertexPT() = default;

  // clang-format off
  VertexPT(f32 x, f32 y, f32 z, f32 u, f32 v)
    : x(x) , y(y) , z(z) , u(u) , v(v) {}

  VertexPT(const Vec3 &pos, const Vec2 &tex)
    : x(pos.x) , y(pos.y) , z(pos.z) , u(tex.x) , v(tex.y) {}
  // clang-format on
};

struct VertexPN {
  Vec3 position;
  Vec3 normal;

  VertexPN() = default;

  // clang-format off
  VertexPN(const f32 pos[3], const f32 norm[3])
    : position(pos[0], pos[1], pos[2])
    , normal(norm[0],norm[1], norm[2]) {}

  VertexPN(const Vec3 &pos, const Vec3 norm)
    : position(pos)
    , normal(norm) {}
  // clang-format on
};

struct VertexPNT {
  Vec3 position;
  Vec3 normal;
  Vec2 texCoords;

  VertexPNT() = default;

  // clang-format off
  VertexPNT(const f32 pos[3], const f32 norm[3], const f32 tex[2])
    : position(pos[0], pos[1], pos[2])
    , normal(norm[0],norm[1], norm[2])
    , texCoords(tex[0], tex[1]) {}

  VertexPNT(const Vec3 &pos, const Vec3 norm, const Vec2 tex)
    : position(pos)
    , normal(norm)
    , texCoords(tex) {}
  // clang-format on
};

struct VertexPNTT {
  Vec3 position;
  Vec3 normal;
  Vec2 texCoord;
  Vec4 tangent; // w component for handedness

  VertexPNTT() = default;
};

template <typename T>
struct VertexTraits {
  static VertexLayout GetLayout();
};

// clang-format off
template <>
struct VertexTraits<VertexP> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3);
  }
};

// clang-format off
template <>
struct VertexTraits<VertexPN> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3)
      .Push(VAS_NORMAL, VAT_FLOAT3);
  }
};

template <>
struct VertexTraits<VertexPC> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3)
      .Push(VAS_COLOR, VAT_UINT);
  }
};

template <>
struct VertexTraits<VertexPT> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3)
      .Push(VAS_TEXCOORD, VAT_FLOAT2);
  }
};

template <>
struct VertexTraits<VertexPNT> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3)
      .Push(VAS_NORMAL, VAT_FLOAT3)
      .Push(VAS_TEXCOORD, VAT_FLOAT2);
  }
};

template <>
struct VertexTraits<VertexPNTT> {
  static VertexLayout GetLayout() {
    return VertexLayout()
      .Push(VAS_POSITION, VAT_FLOAT3)
      .Push(VAS_NORMAL, VAT_FLOAT3)
      .Push(VAS_TEXCOORD, VAT_FLOAT2)
      .Push(VAS_TANGENT, VAT_FLOAT4);
  }
};

namespace std {

template <>
struct hash<VertexPNT> {
  size_t operator()(const VertexPNT &v) const {
    size_t h1 = hash<Vec3>()(v.position);
    size_t h2 = hash<Vec3>()(v.normal);
    size_t h3 = hash<Vec2>()(v.texCoords);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

}


#endif // !SN_VERTEX_TYPE_H
