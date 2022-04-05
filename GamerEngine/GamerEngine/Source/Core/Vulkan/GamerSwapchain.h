#pragma once
#include "GamerContext.h"
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif
#include <iostream>

bool CreateVulkanSwapChain(VkContext* vkContext)
{
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

	// Render Pass
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VK_FORMAT_B8G8R8A8_SRGB;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(vkContext->device, &renderPassInfo, nullptr, &vkContext->renderPass) != VK_SUCCESS)
		{
			std::cout << "Failed to create Render pass" << std::endl;
			return false;
		}
	}
	
}

bool DestroyVulkanSwapChain(VkContext* vkContext)
{
	vkDestroySemaphore(vkContext->device, vkContext->aquireSemaphore, 0);
	vkDestroySemaphore(vkContext->device, vkContext->submitSemaphore, 0);
	vkDestroyCommandPool(vkContext->device, vkContext->commandPool, 0);

	// for (uint32_t i = 0; i < vkContext->scImgCount; i++)
	// {
	// 	vkDestroyImageView(vkContext->device, VkContext->sc, 0);
	// }

	vkDestroySwapchainKHR(vkContext->device, vkContext->swapchain, 0);

	return true;
}

bool RecreateVulkanSwapChain(VkContext* vkContext)
{
	if (vkContext->device == VK_NULL_HANDLE)
	{
		return false;
	}
	
	vkDeviceWaitIdle(vkContext->device);

	DestroyVulkanSwapChain(vkContext);
	CreateVulkanSwapChain(vkContext);

	return true;
}

