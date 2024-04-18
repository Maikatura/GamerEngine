#pragma once
#include "NativeScriptComponent.h"
#include "Scene/Scene.h"
#include "Core/Model/ScriptableEntity.h"
#include "Components/TransfromComponent.h"
#include "Components/CameraComponent.h"
#include "Components/Components.hpp"
#include "Components/ModelComponent.h"
#include "Components/TagComponent.h"
#include "Components/Network/NetworkComponent.h"
#include "Components/IDComponent.h"
#include "Components/CameraController.h"

using AllComponents = 
		ComponentGroup<TransformComponent, CameraComponent, ScriptComponent,
			NativeScriptComponent>;