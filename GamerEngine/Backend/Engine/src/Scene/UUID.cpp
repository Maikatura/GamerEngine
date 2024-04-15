#include "GraphicsEngine.pch.h"
#include "UUID.h"
#include <random>

#include <unordered_map>


static std::random_device ourRandomDevice;
static std::mt19937_64 s_Engine(ourRandomDevice());
static std::uniform_int_distribution<uint64_t> ourUniformDistribution;

GamerEngine::UUID::UUID()
: myUUID(ourUniformDistribution(s_Engine))
{}

GamerEngine::UUID::UUID(uint64_t uuid)
: myUUID(uuid)
{}
