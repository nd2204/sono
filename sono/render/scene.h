#ifndef SN_SCENE_GRAPH_H
#define SN_SCENE_GRAPH_H

#include <render/render_system.h>
#include <core/common/types.h>
#include <core/math/transform.h>
#include <unordered_map>
#include <vector>

using Entity = u32;

struct Hierachy {
  i32 parent;
  i32 firstChild;
  i32 lastChild;
  i32 nextSibling;
  i32 level;
};

class Scene {
public:
  Scene() = default;
  ~Scene() = default;

  template <typename T>
  T &GetComponent(Entity e);

  Entity CreateEntity();

  u32 AddNode(i32 parent, i32 level);
  void Render(RenderSystem *rs);
  void RenderSceneTree(RenderSystem *rs);

private:
  std::unordered_map<usize, usize> m_NodeToName;

  std::vector<Hierachy> m_Hierachy;
  std::vector<Transform> m_Transforms;
  std::vector<std::string> m_Names; // Node names
};

template <>
inline Transform &Scene::GetComponent<Transform>(Entity e) {
  return m_Transforms[e];
}

#endif // !SN_SCENE_GRAPH_H
