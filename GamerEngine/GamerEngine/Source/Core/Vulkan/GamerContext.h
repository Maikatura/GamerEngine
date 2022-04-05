#pragma once
#include <vulkan/vulkan.h>

struct VkContext
{
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    VkSurfaceFormatKHR surfaceFormat;
    VkPhysicalDevice gpu;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSwapchainKHR swapchain;
    VkCommandPool commandPool;

    VkSemaphore aquireSemaphore;
    VkSemaphore submitSemaphore;
    
    uint32_t scImgCount;
    VkImage scImages[5];

    int graphicsIndex;

};