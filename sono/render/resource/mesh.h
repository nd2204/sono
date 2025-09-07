#ifndef SN_MESH_H
#define SN_MESH_H

#include <render/render_device.h>
#include <render/vertex_type.h>
#include <render/vertex_layout.h>
#include <render/buffer_base.h>

struct SubMesh {
  u32 idxOffset;
  u32 idxCount;
};

struct Mesh {
  Buffer *pVertexBuffer;
  Buffer *pIndexBuffer;
  VertexArray *pVertexArray; // NOTE: this will be removed
  std::vector<SubMesh> subMeshes;
  VertexLayout layout;
};

class MeshFactory {
public:
  static Mesh CreateCubeMesh(RenderDevice *device) {
    Mesh ret;
    std::vector<VertexPNT> cubeVertices = {
      // -z face
      VertexPNT({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}),
      VertexPNT({+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}),
      VertexPNT({+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}),
      VertexPNT({-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}),
      // +z face
      VertexPNT({-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {0.0f, 0.0f}),
      VertexPNT({+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {1.0f, 0.0f}),
      VertexPNT({+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {1.0f, 1.0f}),
      VertexPNT({-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, +1.0f}, {0.0f, 1.0f}),
      // -x face
      VertexPNT({-0.5f, +0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
      VertexPNT({-0.5f, +0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
      VertexPNT({-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
      VertexPNT({-0.5f, -0.5f, +0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
      // +x face
      VertexPNT({+0.5f, +0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
      VertexPNT({+0.5f, +0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
      VertexPNT({+0.5f, -0.5f, -0.5f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
      VertexPNT({+0.5f, -0.5f, +0.5f}, {+1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
      // -y face
      VertexPNT({-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}),
      VertexPNT({+0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}),
      VertexPNT({+0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}),
      VertexPNT({-0.5f, -0.5f, +0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),
      // +y face
      VertexPNT({-0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}, {0.0f, 1.0f}),
      VertexPNT({+0.5f, +0.5f, -0.5f}, {0.0f, +1.0f, 0.0f}, {1.0f, 1.0f}),
      VertexPNT({+0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}, {1.0f, 0.0f}),
      VertexPNT({-0.5f, +0.5f, +0.5f}, {0.0f, +1.0f, 0.0f}, {0.0f, 0.0f}),
    };

    std::vector<u16> cubeIndices = {
      0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
      12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };

    ret.pVertexBuffer = device->CreateBuffer(
      {.count = cubeVertices.size(),
       .stride = sizeof(VertexPNT),
       .usage = BufferUsage::Vertex,
       .data = cubeVertices.data()}
    );

    ret.pIndexBuffer = device->CreateBuffer(
      {.count = cubeIndices.size(),
       .stride = sizeof(u16),
       .usage = BufferUsage::Index,
       .data = cubeIndices.data()}
    );

    ret.layout = VertexTraits<VertexPNT>::GetLayout();

    VertexArray *pVAO = device->CreateVertexArray();
    pVAO->AddVertexBuffer(ret.pVertexBuffer, ret.layout);
    pVAO->SetIndexBuffer(ret.pIndexBuffer);
    ret.pVertexArray = pVAO;

    return ret;
  }

  static Mesh CreateWordGridMesh(RenderDevice *device, i32 gridSize, f32 spacing) {
    Mesh ret;

    std::vector<VertexP> mesh;
    for (int i = -gridSize; i <= gridSize; i++) {
      // Ignore the forward and right axis
      if (i == 0) continue;
      mesh.emplace_back(i * spacing, 0.0f, -gridSize * spacing);
      mesh.emplace_back(i * spacing, 0.0f, +gridSize * spacing);
      mesh.emplace_back(-gridSize * spacing, 0.0f, i * spacing);
      mesh.emplace_back(+gridSize * spacing, 0.0f, i * spacing);
    }

    // X axis
    mesh.emplace_back(-gridSize, 0.0f, 0.0f);
    mesh.emplace_back(+gridSize, 0.0f, 0.0f);
    // Y axis
    mesh.emplace_back(0.0f, -gridSize, 0.0f);
    mesh.emplace_back(0.0f, +gridSize, 0.0f);
    // Z axis
    mesh.emplace_back(0.0f, 0.0f, -gridSize);
    mesh.emplace_back(0.0f, 0.0f, +gridSize);

    ret.pVertexBuffer = device->CreateBuffer(
      {.count = mesh.size(),
       .stride = sizeof(VertexP),
       .usage = BufferUsage::Vertex,
       .data = mesh.data()}
    );

    ret.layout = VertexTraits<VertexP>::GetLayout();

    VertexArray *pVAO = device->CreateVertexArray();
    pVAO->AddVertexBuffer(ret.pVertexBuffer, ret.layout);
    ret.pVertexArray = pVAO;

    return ret;
  }
};

#endif // !SN_MESH_H
