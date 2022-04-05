#pragma once
#include "GamerContext.h"
#include "GamerSwapchain.h"
#include "GamerRender.h"
#include "Core/Imgui/imgui.h"
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif
#include <iostream>
#include <windows.h>

#include "../Imgui/imgui_impl_vulkan.h"
#include "../Imgui/imgui_impl_win32.h"

#define ArraySize(arr) sizeof((arr)) / sizeof((arr[0]))

static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT msgServerity,
    VkDebugUtilsMessageTypeFlagsEXT msgFlags,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    msgServerity;
    msgFlags;
    pUserData;
    pCallbackData;

#if _DEBUG
    //std::cout << pCallbackData->pMessage << std::endl;
#endif
    return false;
}


namespace GamerEngine
{
    class GamerEngine
    {
    public:
        GamerEngine();

        bool Init(void* aWindow, std::string aGameName = "GamerEngine69", const int sizeX = 1280,
                  const int sizeY = 720);
        bool InitImGui();
        bool Update();
        bool UpdateSwapchain();

        void ResizeViewport(const int aSizeX, const int aSizeY);
        bool DestroyInstance();

        VkCommandBuffer BeginSingleTimeCommand();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    private:
        VkContext myWindowContext = {};
        HWND myWindow;
    };
}

inline GamerEngine::GamerEngine::GamerEngine()
{
}

inline bool GamerEngine::GamerEngine::Init(void* aWindow, std::string aGameName, const int sizeX, const int sizeY)
{
    sizeX;
    sizeY;

    auto vkContext = &myWindowContext;
    myWindow = static_cast<HWND>(aWindow);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = aGameName.c_str();
    appInfo.pEngineName = "GameEngine";

    const char* extentionsOne[] = {
#ifdef WINDOWS_BUILD
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif LINUX_BUILD
#endif
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME
    };

    const char* layers[]
    {
        "VK_LAYER_KHRONOS_validation"
    };

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.ppEnabledExtensionNames = extentionsOne;
    instanceInfo.enabledExtensionCount = ArraySize(extentionsOne);
    instanceInfo.ppEnabledLayerNames = layers;
    instanceInfo.enabledLayerCount = ArraySize(layers);

    VkResult instanceResult = vkCreateInstance(&instanceInfo, nullptr, &vkContext->instance);
    if (instanceResult != VK_SUCCESS)
    {
        std::cout << "Failed to create instance" << std::endl;
        return false;
    }

    auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        vkContext->instance, "vkCreateDebugUtilsMessengerEXT");

    if (vkCreateDebugUtilsMessengerEXT)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
        debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debugInfo.pfnUserCallback = VkDebugCallback;

        vkCreateDebugUtilsMessengerEXT(vkContext->instance, &debugInfo, 0, &vkContext->debugMessenger);
    }
    else
    {
        return false;
    }

    // Create Service
    {
#ifdef WINDOWS_BUILD
        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hwnd = myWindow;
        surfaceInfo.hinstance = GetModuleHandleA(0);

        VkResult surfaceResult = vkCreateWin32SurfaceKHR(vkContext->instance, &surfaceInfo, 0, &vkContext->surface);
        if (surfaceResult != VK_SUCCESS)
        {
            std::cout << "Failed Create Surface" << std::endl;
            return false;
        }
#elif LINUX_BUILD
#endif
    }


    // Choose GPU
    {
        vkContext->graphicsIndex = -1;

        uint32_t gpuCount = 0;
        // :)
        VkPhysicalDevice gpus[10];

        VkResult gpuCHeckOne = vkEnumeratePhysicalDevices(vkContext->instance, &gpuCount, 0);
        VkResult gpuCHeckTwo = vkEnumeratePhysicalDevices(vkContext->instance, &gpuCount, gpus);

        if (gpuCHeckOne != VK_SUCCESS)
        {
            std::cout << "Failed GPU Check 1" << std::endl;
            return false;
        }

        if (gpuCHeckTwo != VK_SUCCESS)
        {
            std::cout << "Failed GPU Check 2" << std::endl;
            return false;
        }

        for (uint32_t i = 0; i < gpuCount; i++)
        {
            VkPhysicalDevice gpu = gpus[i];

            uint32_t queueFamilyCount = 0;
            VkQueueFamilyProperties queueProps[10];

            vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, 0);
            vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueProps);

            for (uint32_t j = 0; j < queueFamilyCount; j++)
            {
                if (queueProps[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    VkBool32 surfaceSupport = VK_FALSE;
                    VkResult canRender = vkGetPhysicalDeviceSurfaceSupportKHR(
                        gpu, j, vkContext->surface, &surfaceSupport);

                    if (canRender != VK_SUCCESS)
                    {
                        std::cout << "Failed GPU SHIT" << std::endl;
                        return false;
                    }

                    if (surfaceSupport)
                    {
                        vkContext->graphicsIndex = j;
                        vkContext->gpu = gpu;
                        break;
                    }
                }
            }
        }

        if (vkContext->graphicsIndex < 0)
        {
            return false;
        }
    }

    // Logical Device
    {
        float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = vkContext->graphicsIndex;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        const char* extentionsTwo[] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };


        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.pQueueCreateInfos = &queueInfo;
        deviceInfo.queueCreateInfoCount = 1;
        deviceInfo.ppEnabledExtensionNames = extentionsTwo;
        deviceInfo.enabledExtensionCount = ArraySize(extentionsTwo);

        VkResult createDeviceResult = vkCreateDevice(vkContext->gpu, &deviceInfo, 0, &vkContext->device);

        if (createDeviceResult != VK_SUCCESS)
        {
            std::cout << "Failed to create Device" << std::endl;
            return false;
        }

        vkGetDeviceQueue(vkContext->device, vkContext->graphicsIndex, 0, &vkContext->graphicsQueue);
    }

    // Descriptor Pool
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(2);

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(2);
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        if (vkCreateDescriptorPool(myWindowContext.device, &poolInfo, nullptr, &myWindowContext.descriptorPool) != VK_SUCCESS)
        {
            return false;
        }
    }

    CreateVulkanSwapChain(vkContext);

#if _DEBUG
    if (!InitImGui())
    {
        return false;
    }
#endif

    return true;
}

inline bool GamerEngine::GamerEngine::InitImGui()
{
    
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(myWindow);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = myWindowContext.instance;
    init_info.PhysicalDevice = myWindowContext.gpu;
    init_info.Device = myWindowContext.device;
    //init_info.QueueFamily = myWindowContext.graphicsQueue;
    init_info.Queue = myWindowContext.graphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = myWindowContext.descriptorPool;
    init_info.Allocator = nullptr;
    init_info.MinImageCount = 2;
    init_info.ImageCount = myWindowContext.scImgCount;
    //init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, myWindowContext.renderPass);

    // Upload the fonts for DearImgui
    VkCommandBuffer commandBuffer = BeginSingleTimeCommand();
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    EndSingleTimeCommands(commandBuffer);
    ImGui_ImplVulkan_DestroyFontUploadObjects();

    // VkCommandBuffer cmd;
    // ImGui_ImplVulkan_CreateFontsTexture(cmd);


    return true;
}

inline bool GamerEngine::GamerEngine::Update()
{
    return VulkanRender(&myWindowContext);
}

inline bool GamerEngine::GamerEngine::UpdateSwapchain()
{
    return RecreateVulkanSwapChain(&myWindowContext);
}

inline bool GamerEngine::GamerEngine::DestroyInstance()
{
    auto vkContext = &myWindowContext;
    vkDestroySwapchainKHR(vkContext->device, vkContext->swapchain, 0);
    vkDestroyRenderPass(vkContext->device, vkContext->renderPass, 0);
    vkDestroyPipelineLayout(vkContext->device, vkContext->pipelineLayout, 0);
    vkDestroyDevice(vkContext->device, 0);
    vkDestroySurfaceKHR(vkContext->instance, vkContext->surface, 0);
    vkDestroyInstance(vkContext->instance, 0);


    return true;
}

inline VkCommandBuffer GamerEngine::GamerEngine::BeginSingleTimeCommand()
{
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = myWindowContext.commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(myWindowContext.device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        return commandBuffer;
}

inline void GamerEngine::GamerEngine::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(myWindowContext.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(myWindowContext.graphicsQueue);

    vkFreeCommandBuffers(myWindowContext.device, myWindowContext.commandPool, 1, &commandBuffer);
}
