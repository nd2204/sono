#include "core/common/snassert.h"
#include "vertex_layout.h"

#include <unordered_map>

// ================================================================================
// VertexAttribute
// ================================================================================

inline u32 VertexAttribute::GetTypeCount(VertexAttribType type) {
  switch (type) {
    case VAT_FLOAT:
    case VAT_INT:
    case VAT_UINT:
      return 1;
    case VAT_FLOAT2:
    case VAT_INT2:
    case VAT_UINT2:
    case VAT_USHORT2_NORM:
    case VAT_HALF2:
      return 2;
    case VAT_FLOAT3:
    case VAT_INT3:
    case VAT_UINT3:
      return 3;
    case VAT_FLOAT4:
    case VAT_INT4:
    case VAT_UINT4:
    case VAT_BYTE4_NORM:
    case VAT_BYTE4_SNORM:
    case VAT_USHORT4_NORM:
    case VAT_HALF4:
      return 4;
  }
  SN_ASSERT_F(false, "VertexAttribType of %d is unsupported", type);
  return 0;
}

// clang-format off

inline u32 VertexAttribute::GetTypeSize(VertexAttribType type) {
  switch (type) {
  case VAT_FLOAT:        return 4;
  case VAT_FLOAT2:       return 8;
  case VAT_FLOAT3:       return 12;
  case VAT_FLOAT4:       return 16;
  case VAT_INT:          return 4;
  case VAT_INT2:         return 8;
  case VAT_INT3:         return 12;
  case VAT_INT4:         return 16;
  case VAT_UINT:         return 4;
  case VAT_UINT2:        return 8;
  case VAT_UINT3:        return 12;
  case VAT_UINT4:        return 16;
  case VAT_BYTE4_NORM:   return 4;
  case VAT_BYTE4_SNORM:  return 4;
  case VAT_USHORT2_NORM: return 4;
  case VAT_USHORT4_NORM: return 8;
  case VAT_HALF2:        return 2;
  case VAT_HALF4:        return 4;
  }
  SN_ASSERT_F(false, "VertexAttribType of %d is unsupported", type);
  return 0;
}

// clang-format on
u32 VertexAttribute::GetSize() const { return VertexAttribute::GetTypeSize(type); }

u32 VertexAttribute::GetElementCount() const { return VertexAttribute::GetTypeCount(type); }

// clang-format off

b8 VertexAttribute::operator==(const VertexAttribute &rhs) const {
  return !(offset != rhs.offset
    || location != rhs.location
    || type != rhs.type
    || semantic != rhs.semantic
  );
}

// clang-format on

// ================================================================================
// VertexLayout
// ================================================================================

VertexLayout &VertexLayout::Push(VertexAttributeSemantic semantic, VertexAttribType type) {
  m_Attributes.emplace_back(semantic, type, m_Stride, m_Attributes.size());
  UpdateStride();
  return *this;
}

void VertexLayout::UpdateStride() {
  m_Stride = 0;
  for (auto &attr : m_Attributes) {
    attr.offset = m_Stride;
    m_Stride += attr.GetSize();
  }
  // Align stride to 4-byte boundary for better performance
  m_Stride = (m_Stride + 3) & ~3;
}

const VertexAttribute *VertexLayout::Find(VertexAttributeSemantic semantic) const {
  for (auto it = m_Attributes.begin(); it != m_Attributes.end(); it++) {
    if (it->semantic == semantic) return &(*it);
  }
  return nullptr;
}

b8 VertexLayout::IsValid() const {
  if (m_Attributes.empty() || m_Stride == 0) return false;

  return true;
}

std::string VertexLayout::GetValidationError() const {
  if (m_Attributes.empty()) return "No attributes defined";
  if (m_Stride == 0) return "Zero stride";

  std::unordered_map<VertexAttributeSemantic, int> semanticCount;
  for (const auto &attr : m_Attributes) {
    semanticCount[attr.semantic]++;
    if (semanticCount[attr.semantic] > 1) {
      return "Duplicate semantic found";
    }
  }

  for (const auto &attr : m_Attributes) {
    if (attr.offset + attr.GetSize() > m_Stride) {
      return "Attribute extends beyond stride";
    }
  }

  return "";
}

b8 VertexLayout::operator==(const VertexLayout &other) const {
  if (m_Stride != other.m_Stride) return false;
  if (m_Attributes.size() != other.m_Attributes.size()) return false;

  for (size_t i = 0; i < m_Attributes.size(); ++i) {
    const auto &a = m_Attributes[i];
    const auto &b = other.m_Attributes[i];

    if (a != b) {
      return false;
    }
  }

  return true;
}
