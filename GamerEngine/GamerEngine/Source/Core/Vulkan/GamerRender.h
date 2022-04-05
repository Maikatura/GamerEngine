#pragma once
#include "GamerContext.h"
#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#elif LINUX_BUILD
#endif
#include <iostream>

#include "../Imgui/imgui_impl_vulkan.h"
#include "../Imgui/imgui_impl_win32.h"

bool VulkanRender(VkContext* vkContext)
{
	uint32_t imgIndex;
	VkResult renderCheckResult = vkAcquireNextImageKHR(vkContext->device, vkContext->swapchain, 0, vkContext->aquireSemaphore, 0, &imgIndex);
	if (renderCheckResult != VK_SUCCESS)
	{
		std::cout << "Failed to acquire next image" << std::endl;
		return false;
	}
	VkCommandBuffer cmd;;
	VkCommandBufferAllocateInfo allcInfo = {};
	allcInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allcInfo.commandBufferCount = 1;
	allcInfo.commandPool = vkContext->commandPool;
	VkResult allcResult = vkAllocateCommandBuffers(vkContext->device, &allcInfo, &cmd);
	if (allcResult != VK_SUCCESS)
	{
		std::cout << "Failed to allocate command buffer" << std::endl;
		return false;
	}
	

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkResult beginResult = vkBeginCommandBuffer(cmd, &beginInfo);
	if (beginResult != VK_SUCCESS)
	{
		std::cout << "Failed to begin command buffer" << std::endl;
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

#if _DEBUG
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		ImGui::Render();
#endif	
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
		std::cout << "Failed to submit render command" << std::endl;
		return false;
	}
	
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pSwapchains = &vkContext->swapchain;
	presentInfo.swapchainCount = 1;
	presentInfo.pImageIndices = &imgIndex;
	presentInfo.pWaitSemaphores = &vkContext->aquireSemaphore;
	presentInfo.waitSemaphoreCount = 1;
	
	VkResult queuePresentResult = vkQueuePresentKHR(vkContext->graphicsQueue, &presentInfo);
	if (queuePresentResult != VK_SUCCESS)
	{
		std::cout << "Queue Present Failed" << std::endl;
		return false;
	}

	 VkResult waitResult = vkDeviceWaitIdle(vkContext->device); // For now this fixes memory leak
	 if (waitResult != VK_SUCCESS)
	 {
	 	return false;
	 }

	vkFreeCommandBuffers(vkContext->device, vkContext->commandPool, 1, &cmd);

	return true;
}
