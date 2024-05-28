#pragma once
#include "Components/AllComponents.h"
#include <entt.hpp>

template<class T>
struct SnapshotContainer
{
    std::vector<entt::entity> entitySnapshot;
    std::vector<T> componentSnapshot;
};

class SnapshotManager
{
public:
    SnapshotManager();
    SnapshotManager(GamerEngine::Scene* aScene, entt::registry* aRegistry) : myScene(aScene), myRegistry(aRegistry) {}

	void CreateSnapshot();
	void RestoreSnapShot();

private:

    void CleanUpSnapshotData();

    template <class ComponentType>
    void CreateSnapshotForComponent(SnapshotContainer<ComponentType>& aSnapshot);

    template<typename ComponentType>
    void RestoreSnapshotForComponent(SnapshotContainer<ComponentType>& aSnapshot);

    template<typename ComponentType>
    void CleanUpSnapshotData(SnapshotContainer<ComponentType>& aSnapshot);

    entt::registry nullReg{};
	entt::registry* myRegistry;
    GamerEngine::Scene* myScene;

    SnapshotContainer<GamerEngine::TransformComponent> entitySnapshot_Transform;
    //SnapshotContainer<ModelComponent> entitySnapshot_Models;


	// Additional snapshot data for proper entity restoration
    entt::entity snapshotReleased = entt::null;
    std::size_t snapshotRegistrySize = {};
    std::vector<entt::entity> snapshotRegistryEntities;

};
