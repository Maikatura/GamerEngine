#pragma once
#include "Components/Components.hpp"
#include "entt/entt.hpp"


class SnapshotManager
{
public:
    SnapshotManager();
    SnapshotManager(entt::registry* aRegistry) : myRegistry(aRegistry) {}

	void CreateSnapshot();
	void RestoreSnapShot();

private:

    void CleanUpSnapshotData();

    template <class ComponentType>
    void CreateSnapshotForComponent(std::vector<entt::entity>& entitySnapshot, std::vector<ComponentType>& componentSnapshot);

    template<typename ComponentType>
    void RestoreSnapshotForComponent(std::vector<entt::entity>& entitySnapshot, std::vector<ComponentType>& componentSnapshot);

    template<typename ComponentType>
    void CleanUpSnapshotData(std::vector<entt::entity>& entitySnapshot, std::vector<ComponentType>& componentSnapshot);

    entt::registry nullReg{};
	entt::registry* myRegistry;

	std::vector<entt::entity> entitySnapshot_TransformComponent;
    std::vector<TransformComponent> componentSnapshot_TransformComponent;

    std::vector<entt::entity> entitySnapshot_ModelComponent;
    std::vector<ModelComponent> componentSnapshot_ModelComponent;


	// Additional snapshot data for proper entity restoration
    entt::entity snapshotReleased = entt::null;
    std::size_t snapshotRegistrySize = {};
    std::vector<entt::entity> snapshotRegistryEntities;

};
