/**
 * @file GpuDescriptorSet.cc
 * @author Marceline Cramer (cramermarceline@gmail.com)
 * @brief An instance of a bindable GPU descriptor set.
 * @date 2020-11-30
 *
 * @copyright Copyright (c) 2020 Marceline Cramer
 * SPDX-License-Identifier: LGPL-3.0-or-later
 *
 */

#include "gpu/GpuDescriptorSet.h"

namespace mondradiko {

GpuDescriptorSet::GpuDescriptorSet(GpuInstance* gpu,
                                   VkDescriptorSet descriptor_set,
                                   uint32_t dynamic_offset_count)
    : gpu(gpu),
      descriptor_set(descriptor_set),
      dynamic_offsets(dynamic_offset_count) {}

GpuDescriptorSet::~GpuDescriptorSet() {}

void GpuDescriptorSet::updateBuffer(uint32_t binding, GpuBuffer* buffer) {
  VkDescriptorBufferInfo buffer_info{
      .buffer = buffer->buffer, .offset = 0, .range = buffer->buffer_size};

  VkWriteDescriptorSet descriptor_writes{
      .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
      .dstSet = descriptor_set,
      .dstBinding = 0,
      .dstArrayElement = 0,
      .descriptorCount = 1,
      .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
      .pBufferInfo = &buffer_info};

  vkUpdateDescriptorSets(gpu->device, 1, &descriptor_writes, 0, nullptr);
}

void GpuDescriptorSet::updateDynamicOffset(uint32_t index, uint32_t offset) {
  dynamic_offsets[index] = offset;
}

void GpuDescriptorSet::cmdBind(VkCommandBuffer command_buffer,
                               VkPipelineLayout pipeline_layout,
                               uint32_t binding) {
  vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline_layout, 0, 1, &descriptor_set,
                          static_cast<uint32_t>(dynamic_offsets.size()),
                          dynamic_offsets.data());
}

}  // namespace mondradiko
