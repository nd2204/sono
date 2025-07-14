#ifndef SN_VERTEX_LAYOUT_H
#define SN_VERTEX_LAYOUT_H

#include "core/common/types.h"

#include <initializer_list>
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

  VertexLayout(
    std::initializer_list<std::pair<VertexAttributeSemantic, VertexAttribType>> attribPairs
  ) {
    m_Stride = 0;
    for (auto &[semantic, type] : attribPairs) {
      Push(semantic, type);
    }
  }

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

#endif // !SN_VERTEX_LAYOUT_H
