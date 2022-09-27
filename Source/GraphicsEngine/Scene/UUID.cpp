
#include "GraphicsEngine.pch.h"
#include "UUID.h"
#include <random>

#include <unordered_map>

static std::random_device ourRandomDevice;
static std::mt19937_64 s_Engine(ourRandomDevice());
static std::uniform_int_distribution<uint64_t> ourUniformDistribution;

UUID2::UUID2()
: myUUID(ourUniformDistribution(s_Engine))
{}

UUID2::UUID2(uint64_t uuid)
: myUUID(uuid)
{}
