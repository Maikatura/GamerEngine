#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <iostream>
#include <windows.h>

#define ArraySize(arr) sizeof((arr)) / sizeof((arr[0]))

struct VkContext
{
	VkInstance instance;
	VkSurfaceKHR surface;
};

bool vk_init(VkContext* vkContext, void* aWindow)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "GamerEngine";
	appInfo.pEngineName = "GamerEngine";

	const char* extentions[] = {
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME
	};
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.ppEnabledExtensionNames = extentions;
	instanceInfo.enabledExtensionCount = ArraySize(extentions);

	VkResult instanceResult = vkCreateInstance(&instanceInfo, nullptr, &vkContext->instance);
	if (instanceResult != VK_SUCCESS)
	{
		return false;
	}

	VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.hwnd = (HWND)aWindow;
	surfaceInfo.hinstance = GetModuleHandleA(0);

	VkResult surfaceResult = vkCreateWin32SurfaceKHR(vkContext->instance, &surfaceInfo, 0, &vkContext->surface);

	if (surfaceResult != VK_SUCCESS)
	{
		return false;
	}

	return true;
};
