#include "graphics/FrameData.hpp"

#include "graphics/VulkanInstance.hpp"
#include "log/log.hpp"

FrameData::FrameData(VulkanInstance* vulkanInstance, uint32_t framesInFlightCount)
 : vulkanInstance(vulkanInstance),
   framesInFlight(framesInFlightCount)
{
    log_dbg("Creating frame data.");

    for(FrameInFlight& frame : framesInFlight) {
        VkCommandBufferAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = vulkanInstance->commandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1
        };

        if(vkAllocateCommandBuffers(vulkanInstance->device, &allocInfo, &frame.commandBuffer) != VK_SUCCESS) {
            log_ftl("Failed to allocate frame command buffers.");
        }

        VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
        };

        if(vkCreateFence(vulkanInstance->device, &fenceInfo, nullptr, &frame.isInUse) != VK_SUCCESS) {
            log_ftl("Failed to create frame fence.");
        }

        frame.submitted = false;
    }
}

FrameData::~FrameData()
{
    log_dbg("Destroying frame data.");

    for(FrameInFlight& frame : framesInFlight) {
        vkFreeCommandBuffers(vulkanInstance->device, vulkanInstance->commandPool, 1, &frame.commandBuffer);
        vkDestroyFence(vulkanInstance->device, frame.isInUse, nullptr);
    }
}

VkCommandBuffer FrameData::beginPrimaryCommand()
{
    if(framesInFlight[currentFrame].submitted) {
        vkWaitForFences(vulkanInstance->device, 1, &framesInFlight[currentFrame].isInUse, VK_TRUE, UINT64_MAX);
        framesInFlight[currentFrame].submitted = false;
    }

    if(++currentFrame >= framesInFlight.size()) {
        currentFrame = 0;
    }

    VkCommandBuffer commandBuffer = framesInFlight[currentFrame].commandBuffer;

    VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void FrameData::endPrimaryCommand()
{
    vkEndCommandBuffer(framesInFlight[currentFrame].commandBuffer);
}

void FrameData::submitPrimaryCommand()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &framesInFlight[currentFrame].commandBuffer;

    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;

    vkResetFences(vulkanInstance->device, 1, &framesInFlight[currentFrame].isInUse);

    if(vkQueueSubmit(vulkanInstance->graphicsQueue, 1, &submitInfo, framesInFlight[currentFrame].isInUse) != VK_SUCCESS) {
        log_ftl("Failed to submit primary frame command buffer.");
    }

    framesInFlight[currentFrame].submitted = true;
}