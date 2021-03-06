// Copyright (c) 2020-2021 the Mondradiko contributors.
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <vector>

#include "core/renderer/RenderPass.h"

namespace mondradiko {

// Forward declarations
class GlyphLoader;
class GpuImage;
class GpuInstance;
class GpuPipeline;
class GpuShader;
class Renderer;
class ScriptEnvironment;
class UiPanel;

class UserInterface : public RenderPass {
 public:
  UserInterface(GlyphLoader*, Renderer*);
  ~UserInterface();

  // RenderPass implementation
  void createFrameData(uint32_t) final;
  void destroyFrameData() final;
  void allocateDescriptors(uint32_t, GpuDescriptorPool*) final;
  void preRender(uint32_t, VkCommandBuffer) final;
  void render(uint32_t, VkCommandBuffer, const GpuDescriptorSet*) final;

 private:
  GlyphLoader* glyphs;
  GpuInstance* gpu;
  Renderer* renderer;

  ScriptEnvironment* scripts = nullptr;

  UiPanel* main_panel = nullptr;

  GpuShader* panel_vertex_shader = nullptr;
  GpuShader* panel_fragment_shader = nullptr;
  VkPipelineLayout panel_pipeline_layout = VK_NULL_HANDLE;
  GpuPipeline* panel_pipeline = nullptr;

  struct FrameData {
    GpuImage* panel_atlas = nullptr;
  };

  std::vector<FrameData> frame_data;
};

}  // namespace mondradiko
