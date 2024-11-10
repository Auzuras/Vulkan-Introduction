#include "IShader.h"

#include "Renderer.h"

namespace Resources
{
	const bool IShader::Load(Core::IDevice* _Device, std::filesystem::path _ResourcePath)
	{
		const char* ext = _ResourcePath.extension().string().c_str();
		const char* name = _ResourcePath.filename().string().c_str();

		Core::ShaderType type;

		// Checks shader extension
		// TODO: add frag verification
		if (_ResourcePath.extension() == ".vert")
		{
			type = Core::ShaderType::VERTEX;
		}
		else if (_ResourcePath.extension() == ".frag")
		{
			type = Core::ShaderType::FRAGMENT;
		}
		else if (_ResourcePath.extension() == ".geo")
		{
			type = Core::ShaderType::GEOMETRY;
		}
		else
		{
			DEBUG_ERROR("Cannot load shader due to unsuported extension: %s", ext);
			return false;
		}

		std::ifstream shaderFile(_ResourcePath);
		std::string line = "";
		std::string shaderCode = "";

		// Checks if the shader has been openned
		if (!shaderFile.is_open())
		{
			DEBUG_ERROR("Failed to open shader file: %s", _ResourcePath.string().c_str());
		}

		// Reads the shader
		while (std::getline(shaderFile, line))
			shaderCode += line + "\n";

		shaderFile.close();

		Core::Renderer::GetRHI()->CompileShader(_Device, name, shaderCode, type);

		return true;
	}

	const bool IShader::Unload()
	{
		// TODO: Unload/Destroy shader 

		return false;
	}
}