#ifndef SN_VERTEX_LAYOUT_H
#define SN_VERTEX_LAYOUT_H

#include "core/common/types.h"
#include "core/math/vec4.h"
#include "core/math/vec3.h"
#include "core/math/vec2.h"

#include <string>
#include <vector>

// ================================================================================
// VERTEX LAYOUT
// ================================================================================

enum VertexAttribType : u8 {
  VAT_FLOAT,
  VAT_FLOAT2,
  VAT_FLOAT3,
  VAT_FLOAT4,
  VAT_INT,
  VAT_INT2,
  VAT_INT3,
  VAT_INT4,
  VAT_UINT,
  VAT_UINT2,
  VAT_UINT3,
  VAT_UINT4,
  VAT_BYTE4_NORM,   // 4 bytes normalized to [0,1]
  VAT_BYTE4_SNORM,  // 4 bytes normalized to [-1,1]
  VAT_USHORT2_NORM, // 2 ushorts normalized to [0,1]
  VAT_USHORT4_NORM, // 4 ushorts normalized to [0,1]
  VAT_HALF2,        // 2 half floats
  VAT_HALF4         // 4 half floats
};

enum VertexAttributeSemantic : u8 {
  VAS_POSITION,
  VAS_NORMAL,
  VAS_TANGENT,
  VAS_BITANGENT,
  VAS_TEXCOORD,
  /// Diffuse color
  VAS_COLOR,
  /// Specular color
  VAS_COLOR2,
  VAS_BONE_INDICES,
  VAS_BONE_WEIGHTS,
  VAS_INSTANCE_TRANSFORM,
  VAS_INSTANCE_COLOR,
};

struct VertexAttribute {
  u32 offset;   //
  u32 location; // Shader binding location
  VertexAttribType type;
  VertexAttributeSemantic semantic;
  b8 normalized;
  u8 _pad;

  VertexAttribute(VertexAttributeSemantic sem, VertexAttribType t, u32 off, u32 loc = 0)
    : offset(off)
    , location(loc)
    , type(t)
    , semantic(sem)
    , normalized(false) {}

  u32 GetSize() const;

  u32 GetElementCount() const;

  static u32 GetTypeCount(VertexAttribType type);

  static u32 GetTypeSize(VertexAttribType type);

  b8 operator==(const VertexAttribute &rhs) const;

  b8 operator!=(const VertexAttribute &rhs) const { return !(*this == rhs); }
};

class VertexLayout {
public:
  VertexLayout()
    : m_Stride(0) {}

  VertexLayout &Push(VertexAttributeSemantic semantic, VertexAttribType type);

  /// Query method
  const std::vector<VertexAttribute> &GetAttributes() const { return m_Attributes; }

  const VertexAttribute *Find(VertexAttributeSemantic semantic) const;

  u32 GetStride() const { return m_Stride; };

  u32 GetAttributeCount() const { return static_cast<u32>(m_Attributes.size()); };

  b8 IsValid() const;

  std::string GetValidationError() const;

  b8 operator==(const VertexLayout &rhs) const;

  b8 operator!=(const VertexLayout &rhs) const { return !(*this == rhs); }

private:
  void UpdateStride();

private:
  std::vector<VertexAttribute> m_Attributes;
  u32 m_Stride;
};

// ================================================================================
// TYPED VERTEX STRUCTURES
// ================================================================================

using Color = Vec<4, u8>;

struct VertexP {
  Vec3 position;

  VertexP() = default;

  VertexP(const Vec3 &pos)
    : position(pos) {}

  VertexP(f32 x, f32 y, f32 z)
    : position(x, y, z) {}
};

/// Vertex struct with 1 Position and 1 Color
struct VertexPC {
  Vec3 position;
  Color color; // byte packed rgba

  VertexPC() = default;

  VertexPC(const Vec3 &pos, const Color &col)
    : position(pos)
    , color(col) {}

  VertexPC(f32 x, f32 y, f32 z, u32 col)
    : position(x, y, z)
    , color((col >> 24) & 255, (col >> 16) & 255, (col >> 8) & 255, (col) & 255) {}
};

struct VertexPT {
  Vec3 position;
  Vec2 texCoord;

  VertexPT() = default;

  VertexPT(f32 x, f32 y, f32 z, f32 u, f32 v)
    : position(x, y, z)
    , texCoord(u, v) {}

  VertexPT(const Vec3 &pos, const Vec2 &tex)
    : position(pos)
    , texCoord(tex) {}
};

struct VertexPNT {
  Vec3 position;
  Vec3 normal;
  Vec2 texCoord;

  VertexPNT() = default;

  VertexPNT(const f32 pos[3], const f32 norm[3], const f32 tex[2])
    : position(pos[0], pos[1], pos[2])
    , normal(norm[0], norm[1], norm[2])
    , texCoord(tex[0], tex[1]) {}

  VertexPNT(const Vec3 &pos, const Vec3 norm, const Vec2 tex)
    : position(pos)
    , normal(norm)
    , texCoord(tex) {}
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

#endif // !SN_VERTEX_LAYOUT_H

