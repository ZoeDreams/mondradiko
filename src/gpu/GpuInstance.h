/**
 * @file GpuInstance.h
 * @author Marceline Cramer (cramermarceline@gmail.com)
 * @brief Manages all low-level Vulkan objects such as device, debug messenger,
 * VMA allocator, etc.
 * @date 2020-10-24
 *
 * @copyright Copyright (c) 2020 Marceline Cramer
 *
 * This file is part of Mondradiko.
 *
 * Mondradiko is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mondradiko is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Mondradiko.  If not, see <https: //www.gnu.org/licenses/>.
 *
 */

#ifndef SRC_GPU_GPUINSTANCE_H_
#define SRC_GPU_GPUINSTANCE_H_

#include <vector>

#include "displays/DisplayInterface.h"
#include "src/api_headers.h"

namespace mondradiko {

class GpuInstance {
 public:
  explicit GpuInstance(DisplayInterface*);
  ~GpuInstance();

  bool findFormatFromOptions(const std::vector<VkFormat>*,
                             const std::vector<VkFormat>*, VkFormat*);
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer);

  bool enableValidationLayers = true;

  DisplayInterface* display;

  VkInstance instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  VkDevice device = VK_NULL_HANDLE;

  uint32_t graphics_queue_family;
  VkQueue graphics_queue;

  VkCommandPool command_pool = VK_NULL_HANDLE;

  VmaAllocator allocator = nullptr;

  VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;

 private:
  const std::vector<const char*> validationLayers = {
      "VK_LAYER_KHRONOS_validation"};

  bool checkValidationLayerSupport();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT*);
  void createInstance(VulkanRequirements*);
  void setupDebugMessenger();
  void findPhysicalDevice(DisplayInterface*);
  void findQueueFamilies();
  void createLogicalDevice(VulkanRequirements*);
  void createCommandPool();
  void createAllocator();
  void createDescriptorPool();

  PFN_vkCreateDebugUtilsMessengerEXT ext_vkCreateDebugUtilsMessengerEXT =
      nullptr;
  PFN_vkDestroyDebugUtilsMessengerEXT ext_vkDestroyDebugUtilsMessengerEXT =
      nullptr;
};

}  // namespace mondradiko

#endif  // SRC_GPU_GPUINSTANCE_H_