#pragma once
#include "NativeScriptComponent.h"
#include "Scene/Scene.h"
#include "Core/Model/ScriptableEntity.h"
#include "Components/TransfromComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Components.hpp"
#include "Components/ModelComponent.h"
#include "Components/TagComponent.h"
#include "Components/IDComponent.h"
#include "Components/CameraController.h"

//using AllComponents = 
//		ComponentGroup<GamerEngine::TransformComponent, GamerEngine::CameraComponent, GamerEngine::ScriptComponent,
//	GamerEngine::NativeScriptComponent>;

using AllComponents =
ComponentGroup<GamerEngine::TransformComponent, GamerEngine::NativeScriptComponent, ModelComponent, GamerEngine::CameraComponent, PointLightComponent, SpotLightComponent, DirectionalLightComponent,GamerEngine::ScriptComponent>;

using ScriptComponents =
ComponentGroup<GamerEngine::TransformComponent>;
