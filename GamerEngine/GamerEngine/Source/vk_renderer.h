#pragma once
#include <vulkan/vulkan.h>
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif
#include <iostream>



#define ArraySize(arr) sizeof((arr)) / sizeof((arr[0]))

static VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT msgServerity,
	VkDebugUtilsMessageTypeFlagsEXT msgFlags,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
)
{
	OutputDebugStringW((LPCWSTR)pCallbackData->pMessage);
	std::cout << "Validatrion Error" << pCallbackData->pMessage << std::endl;
	return false;
}

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

bool VulkanInit(VkContext* vkContext, void* aWindow)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "GamerEngine";
	appInfo.pEngineName = "GamerEngine";

	const char* extentions[] = {
#ifdef WINDOWS_BUILD
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif LINUX_BUILD
#endif
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_KHR_SURFACE_EXTENSION_NAME };

	const char* layers[]
	{
		"VK_LAYER_KHRONOS_validation"
	};

	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.ppEnabledExtensionNames = extentions;
	instanceInfo.enabledExtensionCount = ArraySize(extentions);
	instanceInfo.ppEnabledLayerNames = layers;
	instanceInfo.enabledLayerCount = ArraySize(layers);

	VkResult instanceResult = vkCreateInstance(&instanceInfo, nullptr, &vkContext->instance);
	if (instanceResult != VK_SUCCESS)
	{
		std::cout << "Failed to create instance" << std::endl;
		return false;
	}

	auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkContext->instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsMessengerEXT)
	{
		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
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
		surfaceInfo.hwnd = (HWND)aWindow;
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
					VkResult canRender = vkGetPhysicalDeviceSurfaceSupportKHR(gpu, j, vkContext->surface, &surfaceSupport);

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

		const char* extentions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};


		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pQueueCreateInfos = &queueInfo;
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.ppEnabledExtensionNames = extentions;
		deviceInfo.enabledExtensionCount = ArraySize(extentions);

		VkResult createDeviceResult = vkCreateDevice(vkContext->gpu, &deviceInfo, 0, &vkContext->device);

		if (createDeviceResult != VK_SUCCESS)
		{
			std::cout << "Failed to create Device" << std::endl;
			return false;
		}

		vkGetDeviceQueue(vkContext->device, vkContext->graphicsIndex, 0, &vkContext->graphicsQueue);
	}


	// Swapchain
	{

		uint32_t formatCount = 0;
		VkSurfaceFormatKHR surfaceFormats[10];
		VkResult reusltOne = vkGetPhysicalDeviceSurfaceFormatsKHR(vkContext->gpu, vkContext->surface, &formatCount, 0);
		VkResult reusltTwo = vkGetPhysicalDeviceSurfaceFormatsKHR(vkContext->gpu, vkContext->surface, &formatCount, surfaceFormats);

		if (reusltOne != VK_SUCCESS)
		{
			std::cout << "Failed to create surface format 1" << std::endl;
			return false;
		}

		if (reusltTwo != VK_SUCCESS)
		{
			std::cout << "Failed to create surface format 2" << std::endl;
			return false;
		}

		for (uint32_t i = 0; i < formatCount; i++)
		{
			VkSurfaceFormatKHR format = surfaceFormats[i];

			if (format.format == VK_FORMAT_B8G8R8A8_SRGB)
			{
				vkContext->surfaceFormat = format;
				break;
			}
		}

		VkSurfaceCapabilitiesKHR surfaceCaps = {};
		VkResult success = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkContext->gpu, vkContext->surface, &surfaceCaps);
		if (success != VK_SUCCESS)
		{
			std::cout << "Failed to get gpu surface capabilities" << std::endl;
			return false;
		}

		uint32_t imgCount = surfaceCaps.minImageCount + 1;
		imgCount = imgCount > surfaceCaps.maxImageCount ? imgCount - 1 : imgCount;

		VkSwapchainCreateInfoKHR scInfo = {};
		scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Can Fuck this up :)
		scInfo.surface = vkContext->surface;
		scInfo.imageFormat = vkContext->surfaceFormat.format;
		scInfo.preTransform = surfaceCaps.currentTransform;
		scInfo.imageExtent = surfaceCaps.currentExtent;
		scInfo.minImageCount = imgCount;
		scInfo.imageArrayLayers = 1;

		VkResult scResult = vkCreateSwapchainKHR(vkContext->device, &scInfo, 0, &vkContext->swapchain);
		if (scResult != VK_SUCCESS)
		{
			std::cout << "Failed to create swapchain" << std::endl;
			return false;
		}

		VkResult scResulktOne = vkGetSwapchainImagesKHR(vkContext->device, vkContext->swapchain, &vkContext->scImgCount, 0);
		VkResult scResulktTwo = vkGetSwapchainImagesKHR(vkContext->device, vkContext->swapchain, &vkContext->scImgCount, vkContext->scImages);

		if (scResulktOne != VK_SUCCESS)
		{
			std::cout << "Failed to get swapchain 1" << std::endl;
			return false;
		}

		if (scResulktTwo != VK_SUCCESS)
		{
			std::cout << "Failed to get swapchain 2" << std::endl;
			return false;
		}

	}

	// Command Pool
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = vkContext->graphicsIndex;
		VkResult commandResult = vkCreateCommandPool(vkContext->device, &poolInfo, 0, &vkContext->commandPool);

		if (commandResult != VK_SUCCESS)
		{
			std::cout << "Failed create Command pool" << std::endl;
			return false;
		}
	}

	// Sync Objects
	{

		VkSemaphoreCreateInfo semaInfo = {};
		semaInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;


		VkResult objectOne = vkCreateSemaphore(vkContext->device, &semaInfo, 0, &vkContext->aquireSemaphore);
		VkResult objectTwo = vkCreateSemaphore(vkContext->device, &semaInfo, 0, &vkContext->submitSemaphore);

		if (objectOne != VK_SUCCESS)
		{
			return false;
		}

		if (objectTwo != VK_SUCCESS)
		{
			return false;
		}
	}

	return true;
};


bool VulkanRender(VkContext* vkContext)
{
	uint32_t imgIndex;
	VkResult renderCheckResult = vkAcquireNextImageKHR(vkContext->device, vkContext->swapchain, 0, vkContext->aquireSemaphore, 0, &imgIndex);
	if (renderCheckResult != VK_SUCCESS)
	{
		return false;
	}
	VkCommandBuffer cmd;
	VkCommandBufferAllocateInfo allcInfo = {};
	allcInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allcInfo.commandBufferCount = 1;
	allcInfo.commandPool = vkContext->commandPool;
	VkResult allcResult = vkAllocateCommandBuffers(vkContext->device, &allcInfo, &cmd);
	if (allcResult != VK_SUCCESS)
	{
		return false;
	}
	

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkResult beginResult = vkBeginCommandBuffer(cmd, &beginInfo);
	if (beginResult != VK_SUCCESS)
	{
		return false;
	}

	// Render Command
	{

		VkClearColorValue color = { 0.09f,0.55f,0.76f, 1.0f };
		VkImageSubresourceRange range = {};
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.layerCount = 1;
		range.levelCount = 1;

		vkCmdClearColorImage(cmd, vkContext->scImages[imgIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, &color, 1, &range);
	}

	VkResult endResult = vkEndCommandBuffer(cmd);
	if (endResult != VK_SUCCESS)
	{
		std::cout << "Failed End Buffer Commnand";
		return false;
	}

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pWaitDstStageMask = &waitStage;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cmd;
	submitInfo.pSignalSemaphores = &vkContext->submitSemaphore;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &vkContext->aquireSemaphore;

	VkResult renderSubmitResult = vkQueueSubmit(vkContext->graphicsQueue, 1, &submitInfo, 0);
	if (renderSubmitResult != VK_SUCCESS)
	{
		return false;
	}

	
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pSwapchains = &vkContext->swapchain;
	presentInfo.swapchainCount = 1;
	presentInfo.pImageIndices = &imgIndex;
	presentInfo.pWaitSemaphores = &vkContext->submitSemaphore;
	presentInfo.waitSemaphoreCount = 1;

	VkResult queuePresentResult = vkQueuePresentKHR(vkContext->graphicsQueue, &presentInfo);
	if (queuePresentResult != VK_SUCCESS)
	{
		return false;
	}

	VkResult waitResult = vkDeviceWaitIdle(vkContext->device);
	if (waitResult != VK_SUCCESS)
	{
		return false;
	}

	vkFreeCommandBuffers(vkContext->device, vkContext->commandPool, 1, &cmd);
}