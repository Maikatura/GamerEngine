#pragma once

#include "elems/camera.h"
#include "elems/mesh.h"
#include "elems/light.h"
#include "shader/shader_util.h"
#include "render/opengl_buffer_manager.h"
#include "elems/input.h"
#include "Models/Model.h"

namespace GamerEngine
{
  class SceneView
  {
  public:
      SceneView();

      ~SceneView();

    GamerEngine::Light* get_light() { return mLight.get(); }

    void Resize(int32_t width, int32_t height);

    void Render();

    void LoadMesh(const std::string& filepath);

    void SetMesh(std::shared_ptr<Model> mesh);

    std::shared_ptr<Model> GetMesh() { return mMesh; }
    
    void OnMouseMove(double x, double y, GamerEngine::EInputButton button);

    void OnMouseWheel(double delta);

    void ResetView();

  private:
    std::unique_ptr<GamerEngine::Camera> mCamera;
    std::unique_ptr<GamerEngine::OpenGL_FrameBuffer> mFrameBuffer;
    std::unique_ptr<nshaders::Shader> mShader;
    std::unique_ptr<GamerEngine::Light> mLight;
    std::shared_ptr<Model> mMesh;
    glm::vec2 mSize;
  };
}

