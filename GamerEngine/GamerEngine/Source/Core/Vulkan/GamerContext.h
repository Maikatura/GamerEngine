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
    VkPipelineCache pipelineCache;
    VkSwapchainKHR swapchain;
    VkCommandPool commandPool;
    VkDescriptorPool descriptorPool;

    VkFramebuffer frameBuffer;
    VkImageView  imageView;
    
    VkSemaphore aquireSemaphore;
    VkSemaphore submitSemaphore;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;

    
    uint32_t scImgCount;
    VkImage scImages[5];

    int graphicsIndex;

};