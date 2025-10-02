#pragma once

#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
// Utils - Libs
#include <C78E/Utils/StdUtils.h>
#include <C78E/Utils/PlatformUtils.h>

#include <C78E/Utils/LogUtils.h>
#include <C78E/Math/Math.h>
#include <C78E/Utils/YamlUtils.h>
#include <C78E/Utils/ImGui/ImGuiUtils.h>
#include <entt/entt.hpp>

// Core
#include <C78E/Core/Base.h>
#include <C78E/Core/Core.h>
#include <C78E/Core/Log/SmartLog.h>
#include <C78E/Core/Cast.h>
#include <C78E/Core/Timestep.h>
#include <C78E/Core/Timer.h>
#include <C78E/Core/UUID.h>
#include <C78E/Core/Buffer/Buffer.h>
#include <C78E/Core/Random.h>
#include <C78E/Core/Application/Application.h>

#include <C78E/Core/Events/ApplicationEvent.h>
#include <C78E/Core/Events/KeyEvent.h>
#include <C78E/Core/Events/MouseEvent.h>
#include <C78E/Core/Events/WindowEvent.h>

#include <C78E/Core/FileSystem/FileSystem.h>

#include <C78E/Core/Image/ImageFormat.h>
#include <C78E/Core/Image/ColorSpace.h>
#include <C78E/Core/Image/MultiSample.h>


// Graphics
#include <C78E/Graphics/API/GraphicsInstance.h>
#include <C78E/Core/Window/Window.h>
#include <C78E/Graphics/API/GraphicsContext.h>

#include <C78E/Graphics/API/SwapChain/SwapChain.h>
#include <C78E/Graphics/API/Command/RenderPass.h>
#include <C78E/Graphics/API/Image/FrameBuffer.h>

#include <C78E/Graphics/API/Pipeline/Pipeline.h>
#include <C78E/Graphics/API/Pipeline/PipelineLayout.h>
#include <C78E/Graphics/API/Pipeline/PipelineConfig.h>


#include <C78E/Graphics/API/Command/CommandBuffer.h>


// Temporary Graphics - Vulkan
#include <Platform/Khronos/Vulkan/API/VulkanGraphicsContext.h>
#include <Platform/Khronos/Vulkan/API/Command/VulkanCommandBuffer.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipeline.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineLayout.h>
#include <Platform/Khronos/Vulkan/API/Pipeline/VulkanPipelineConfig.h>
#include <Platform/Khronos/Vulkan/API/SwapChain/VulkanSwapChain.h>
#include <Platform/Khronos/Vulkan/API/Buffer/VulkanFrameBuffer.h>

//TODO: GPU Assets

// Physics

#include <C78E/Physics/PhysicsWorld.h>

//GUI
#include <C78E/GUI/GUIUtils.h>


#ifdef C78E_ENTRYPOINT
#include <C78E/Core/EntryPoint.h>
#endif
