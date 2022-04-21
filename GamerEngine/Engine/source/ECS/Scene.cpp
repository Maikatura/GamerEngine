#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include <imgui/imgui.h>
#include "Components/MeshComponent.h"
#include "Components/TagComponent.h"
#include "window/EngineWindow.h"

namespace GamerEngine
{
	Scene::Scene()
	{
		myCamera = std::make_unique<Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 1000.0f);
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		entt::entity entityID = myRegistry.create();
		Entity returnEntity = Entity(entityID, this);
		returnEntity.AddComponent<TransformComponent>();
		returnEntity.AddComponent<TagComponent>();
		return returnEntity;
	}

	void Scene::Render(GamerEngine::Shader* shader)
	{
		auto view = myRegistry.view<MeshComponent>();

		for (auto entity : view)
		{
			auto [mesh, transform] = myRegistry.get<MeshComponent, TransformComponent>(entity);
			mesh.myModel.Update(transform, shader);
			mesh.myModel.Render();
		}
	}

	void Scene::RenderCamera(GamerEngine::Shader* shader)
	{
		if (ImGui::IsWindowHovered())
		{
			GLFWwindow* mWindow = (GLFWwindow*)GLWindow::GetInstance()->GetNativeWindow();
			myCamera->OnMouseMove(ImGui::GetMousePos().x, ImGui::GetMousePos().y, Input::GetPressedButton(mWindow));
		}


		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		CommonUtilities::Vector3<float> size = { (float)viewportPanelSize.x, (float)viewportPanelSize.y, 0 };

		myCamera->SetAspect(size.x / size.y);

		auto view = myRegistry.view<MeshComponent>();

		for (auto entity : view)
		{
			MeshComponent& mesh = myRegistry.get<MeshComponent>(entity);
			myCamera->Update(&mesh, shader);
		}

	}

	entt::registry* Scene::GetRegistry()
	{
		return &myRegistry;
	}

	GamerEngine::Camera* Scene::GetCamera()
	{
		return myCamera.get();
	}
}
