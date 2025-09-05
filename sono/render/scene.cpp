#include <render/scene.h>

u32 Scene::AddNode(i32 parent, i32 level) {
  i32 node = (i32)m_Hierachy.size();
  m_Hierachy.push_back(
    {.parent = parent, .firstChild = -1, .lastChild = -1, .nextSibling = -1, .level = level}
  );
  m_Transforms.emplace_back();

  // update parent node if valid index provided
  if (parent > -1) {
    i32 parentFirstChild = m_Hierachy[parent].firstChild;
    // If parent doesn't has first child
    if (parentFirstChild <= -1) {
      // Set first child and last child to parent
      m_Hierachy[parent].firstChild = node;
      m_Hierachy[parent].lastChild = node;
    } else {
      // Add sibling to the last child of parent
      i32 parentLastChild = m_Hierachy[parent].lastChild;
      // Set the last child reference for parent node if it not set
      if (parentLastChild <= -1) {
        parentLastChild = parentFirstChild;
        while (m_Hierachy[parentLastChild].nextSibling > -1)
          parentLastChild = m_Hierachy[parentLastChild].nextSibling;
      }
      m_Hierachy[parentLastChild].nextSibling = node;
      m_Hierachy[parent].lastChild = node;
    }
  }

  return node;
}

Entity Scene::CreateEntity() {
  // NOTE: for now the level is not set
  return AddNode(-1, 0);
}

void Scene::Render(RenderSystem *rs) {
  RenderDevice *device = rs->GetRenderDevice();
  CommandList *cmdList = device->CreateCommandList();
  RenderPass *pass = cmdList->BeginRenderPass({.clearCol = Color3(0, 0, 0)});
}

void Scene::RenderSceneTree(RenderSystem *rs) {}
