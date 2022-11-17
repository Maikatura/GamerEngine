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
	CreateSnapshotForComponent<TransformComponent>(entitySnapshot_Transform);
    CreateSnapshotForComponent<ModelComponent>(entitySnapshot_Models);
}

void SnapshotManager::RestoreSnapShot()
{
	/*myRegistry = {};
	myRegistry->assign(snapshotRegistryEntities.data(), snapshotRegistryEntities.data() + snapshotRegistrySize, snapshotReleased);*/

	// Restore the actual snapshot data per each component class
	// This copy-per-class approach is necessary due to how EnTT is built
	RestoreSnapshotForComponent<TransformComponent>(entitySnapshot_Transform);
    RestoreSnapshotForComponent<ModelComponent>(entitySnapshot_Models);
    /*RestoreSnapshotForComponent()*/

    CleanUpSnapshotData();
}

void SnapshotManager::CleanUpSnapshotData()
{
    snapshotRegistryEntities.clear();

    // Clear all existing snapshot data
    CleanUpSnapshotData<TransformComponent>(entitySnapshot_Transform);
    CleanUpSnapshotData<ModelComponent>(entitySnapshot_Models);
}

template <typename ComponentType>
void SnapshotManager::CreateSnapshotForComponent(SnapshotContainer<ComponentType>& aSnapshot)
{
    // Get reference to unique storage for this component type
    auto&& storage = myRegistry->storage<ComponentType>();

    // Create entity snapshot
    // For why not using memcpy (speed) and examples of different methods for copying data into a vector: https://stackoverflow.com/a/261607/3735890
    aSnapshot.entitySnapshot.insert(aSnapshot.entitySnapshot.end(), storage.data(), storage.data() + storage.size());

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
        aSnapshot.componentSnapshot.insert(aSnapshot.componentSnapshot.end(), pageStartPtr, pageStartPtr + numberOfElementsToCopy);
    }
}

template <typename ComponentType>
void SnapshotManager::RestoreSnapshotForComponent(SnapshotContainer<ComponentType>& aSnapshot)
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
        const std::size_t numberOfElementsToCopy = (((pageSize) < (aSnapshot.componentSnapshot.size() - offset)) ? (pageSize) : (aSnapshot.componentSnapshot.size() - offset));

        ComponentType* pageStartPtr = storage.raw()[pageIndex];
        memcpy(pageStartPtr, aSnapshot.componentSnapshot.data() + offset, sizeof(ComponentType) * numberOfElementsToCopy);
    }
}


template<typename ComponentType>
void SnapshotManager::CleanUpSnapshotData(SnapshotContainer<ComponentType>& aSnapshot)
{
    aSnapshot.entitySnapshot.clear();
    aSnapshot.componentSnapshot.clear();
}