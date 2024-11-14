#pragma once

#include "Debug/Log.h"
#include <filesystem>

#include "RHI/RHITypes/IDevice.h"

namespace Core
{
	class IResource
	{
	private:

	public:
		/// <summary>
		/// Loads a resource specified by a path
		/// </summary>
		/// <param name="_ResourcePath">: Path of the resource </param>
		/// <returns></returns>
		virtual const bool Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath) = 0;

		/// <summary>
		/// Unloads the resource in the memory
		/// </summary>
		/// <returns></returns>
		virtual const bool Unload(Core::IDevice* _Device) = 0;
	};
}