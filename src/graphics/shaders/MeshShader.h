/**
 * @file MeshShader.h
 * @author Marceline Cramer (cramermarceline@gmail.com)
 * @brief Contains pipeline state for the MeshPipeline.
 * @date 2020-10-24
 *
 * @copyright Copyright (c) 2020 Marceline Cramer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https: //www.gnu.org/licenses/>.
 *
 */

#ifndef SRC_GRAPHICS_SHADERS_MESHSHADER_H_
#define SRC_GRAPHICS_SHADERS_MESHSHADER_H_

#include "graphics/shaders/ShaderModule.h"

class MeshShader {
 public:
  explicit MeshShader(VulkanInstance*);
  ~MeshShader();

  ShaderStages getStages();

 private:
  VulkanInstance* vulkanInstance;

  ShaderModule vertShader;
  ShaderModule fragShader;
};

#endif  // SRC_GRAPHICS_SHADERS_MESHSHADER_H_