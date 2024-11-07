#include "IShader.h"

namespace Resources
{
	IShader* IShader::Load(std::filesystem::path _ResourcePath)
	{
		const char* ext = _ResourcePath.extension().string().c_str();

		// Checks shader extension
		// TODO: add frag verification
		if (_ResourcePath.extension() != ".vert")
		{
			DEBUG_ERROR("Cannot load shader due to unsuported extension: %s", ext);
			return nullptr;
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
		
		// TODO: Compile shader

		return nullptr;
	}

	const bool IShader::Unload()
	{
		// TODO: Unload/Destroy shader 

		return false;
	}
}