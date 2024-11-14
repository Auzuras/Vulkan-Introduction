#pragma once

#include "Debug/Log.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <string>

#include "Window.h"
#include "RHI/RHITypes.h"

#include "RHI/RHITypes/RHITypesWrapper.h"

#include "IShader.h"
#include "IMesh.h"
#include "ITexture.h"

namespace Core
{
	/// <summary>
	/// Renderer Hardware Interface class -
	/// The goal of this class is to wrap every graphic API
	/// </summary>
	class IRendererHardware
	{
	public:

		///////////////////////////////////////////////////////////////////////

		/// Constructors and destructors

		///////////////////////////////////////////////////////////////////////

		IRendererHardware() = default;
		virtual ~IRendererHardware() = default;

		///////////////////////////////////////////////////////////////////////

		/// Initialization and termination methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Initialization method of the graphic API
		/// </summary>
		/// <param name="_Width"> : width of the window</param>
		/// <param name="_Height"> : height of the window</param>
		/// <param name="_WindowName"> : name of the window</param>
		/// <returns></returns>
		virtual const bool Initialize(Window* _Window) = 0;

		/// <summary>
		/// Termination method of the graphic API
		/// </summary>
		/// <returns></returns>
		virtual const bool Terminate() = 0;

		///////////////////////////////////////////////////////////////////////

		/// Device related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Creates a device
		/// </summary>
		/// <returns></returns>
		virtual IDevice* InstanciateDevice(Window* _Window) = 0;

		virtual void DestroyDevice(IDevice* _Device);

		///////////////////////////////////////////////////////////////////////

		/// SwapChain related methods

		///////////////////////////////////////////////////////////////////////
		
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual ISwapChain* InstantiateSwapChain(Window* _Window, IDevice* _Device) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		virtual void DestroySwapChain(ISwapChain* _SwapChain, IDevice* _Device);

		///////////////////////////////////////////////////////////////////////

		/// Pipeline related methods

		///////////////////////////////////////////////////////////////////////

		virtual IPipeline* InstantiatePipeline(IDevice* _Device, ISwapChain* _Swapchain, std::vector<PipelineShaderInfos> _ShadersInfos) = 0;

		virtual void DestroyPipeline(IPipeline* _Pipeline, IDevice* _Device);

		///////////////////////////////////////////////////////////////////////

		/// Command allocator related methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_Device"></param>
		/// <returns></returns>
		virtual ICommandAllocator* InstantiateCommandAllocator(IDevice* _Device) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="_CommandAllocator"></param>
		/// <param name="_Device"></param>
		virtual void DestroyCommandAllocator(ICommandAllocator* _CommandAllocator, IDevice* _Device);

		///////////////////////////////////////////////////////////////////////

		/// Shader related methods
		
		///////////////////////////////////////////////////////////////////////

		virtual IShader* CreateShader() = 0;
		virtual void DestroyShader(IShader* _Shader);

		///////////////////////////////////////////////////////////////////////

		/// Mesh related methods
		
		///////////////////////////////////////////////////////////////////////

		virtual IMesh* CreateMesh() = 0;
		virtual void DestroyMesh(IMesh* _Mesh);

		///////////////////////////////////////////////////////////////////////

		/// TExture related methods

		///////////////////////////////////////////////////////////////////////

		virtual ITexture* CreateTexture() = 0;
		virtual void DestroyTexture(ITexture* _Texture);

	};
}