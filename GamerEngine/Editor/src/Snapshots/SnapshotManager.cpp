#include "Editor.pch.h"
#include <Snapshots/SnapshotManager.h>
#include "GraphicsEngine.h"
#include "entt/entt.hpp"
#include "Scene/Scene.h"
#include <Components/Components.hpp>


SnapshotManager::SnapshotManager()
{
}

void SnapshotManager::CreateSnapshot()
{
	snapshotRegistrySize = myRegistry->size();
	snapshotReleased = myRegistry->released();
	snapshotRegistryEntities.insert(snapshotRegistryEntities.end(), myRegistry->data(),
        myRegistry->data() + snapshotRegistrySize);

	// Store the actual snapshot data per each component class
	// This copy-per-class approach is necessary due to how EnTT is built
	CreateSnapshotForComponent<TransformComponent>(entitySnapshot_TransformComponent, componentSnapshot_TransformComponent);
    CreateSnapshotForComponent<ModelComponent>(entitySnapshot_ModelComponent, componentSnapshot_ModelComponent);
}

void SnapshotManager::RestoreSnapShot()
{
	/*myRegistry = {};
	myRegistry->assign(snapshotRegistryEntities.data(), snapshotRegistryEntities.data() + snapshotRegistrySize, snapshotReleased);*/

	// Restore the actual snapshot data per each component class
	// This copy-per-class approach is necessary due to how EnTT is built
	RestoreSnapshotForComponent<TransformComponent>(entitySnapshot_TransformComponent, componentSnapshot_TransformComponent);
    RestoreSnapshotForComponent<ModelComponent>(entitySnapshot_ModelComponent, componentSnapshot_ModelComponent);

    CleanUpSnapshotData();
}

void SnapshotManager::CleanUpSnapshotData()
{
    snapshotRegistryEntities.clear();

    // Clear all existing snapshot data
    CleanUpSnapshotData<TransformComponent>(entitySnapshot_TransformComponent, componentSnapshot_TransformComponent);
    CleanUpSnapshotData<ModelComponent>(entitySnapshot_ModelComponent, componentSnapshot_ModelComponent);
}

template <typename ComponentType>
void SnapshotManager::CreateSnapshotForComponent(std::vector<entt::entity>& entitySnapshot,
    std::vector<ComponentType>& componentSnapshot)
{
    // Get reference to unique storage for this component type
    auto&& storage = myRegistry->storage<ComponentType>();

    // Create entity snapshot
    // For why not using memcpy (speed) and examples of different methods for copying data into a vector: https://stackoverflow.com/a/261607/3735890
    entitySnapshot.insert(entitySnapshot.end(), storage.data(), storage.data() + storage.size());

    // Create component snapshot
    // Note that data may cross multiple pages in memory. Thus, need to copy the data into the output vector one page at a time
    const std::size_t pageSize = entt::component_traits<ComponentType>::page_size;
    const std::size_t totalPages = (storage.size() + pageSize - 1u) / pageSize;
    for(std::size_t pageIndex{}; pageIndex < totalPages; pageIndex++)
    {
        // Calculate number of elements to copy so only copyyig over the necessary number of elements
        // Truly necessary as using ComponentType vector here instead of, say, vector of arbitrary bytes
        const std::size_t offset = pageIndex * pageSize;
        const std::size_t numberOfElementsToCopy = (((pageSize) < (storage.size() - offset)) ? (pageSize) : (storage.size() - offset));

        // Do the actual copying
        ComponentType* pageStartPtr = storage.raw()[pageIndex];
        componentSnapshot.insert(componentSnapshot.end(), pageStartPtr, pageStartPtr + numberOfElementsToCopy);
    }
}

template <typename ComponentType>
void SnapshotManager::RestoreSnapshotForComponent(std::vector<entt::entity>& entitySnapshot,
	std::vector<ComponentType>& componentSnapshot)
{
    auto&& storage = myRegistry->storage<ComponentType>();

    // Restore entities
    //storage.insert(entitySnapshot.begin(), entitySnapshot.end());

    // Restore components
    // Note that data may cross multiple pages in memory. Thus, need to copy the data into the storage one page at a time
    const std::size_t pageSize = entt::component_traits<ComponentType>::page_size;
    const std::size_t totalPages = (storage.size() + pageSize - 1u) / pageSize;
    for(std::size_t pageIndex = 0; pageIndex < totalPages; pageIndex++)
    {
        const std::size_t offset = pageIndex * pageSize;
        const std::size_t numberOfElementsToCopy = (((pageSize) < (componentSnapshot.size() - offset)) ? (pageSize) : (componentSnapshot.size() - offset));

        ComponentType* pageStartPtr = storage.raw()[pageIndex];
        memcpy(pageStartPtr, componentSnapshot.data() + offset, sizeof(ComponentType) * numberOfElementsToCopy);
    }
}


template<typename ComponentType>
void SnapshotManager::CleanUpSnapshotData(std::vector<entt::entity>& entitySnapshot, std::vector<ComponentType>& componentSnapshot)
{
    entitySnapshot.clear();
    componentSnapshot.clear();
}