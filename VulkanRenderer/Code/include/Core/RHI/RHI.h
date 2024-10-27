#pragma once

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <string>

namespace Core
{
	enum ShaderType
	{
		VERTEX,
		FRAGMENT
	};

	/// <summary>
	/// RHI class -
	/// Renderer Hardware Interface class -
	/// The goal of this class is to wrap every graphic API
	/// </summary>
	class RHI
	{
	public:
		GLFWwindow* window = nullptr;

		RHI() = default;
		~RHI() = default;

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
		virtual const bool Initialize(GLFWwindow* _Window) = 0;

		/// <summary>
		/// Termination method of the graphic API
		/// </summary>
		/// <returns></returns>
		virtual const bool Terminate() = 0;

		virtual void DrawFrame() = 0;

		///////////////////////////////////////////////////////////////////////

		/// Rendering methods

		///////////////////////////////////////////////////////////////////////

		///// <summary>
		///// Render method
		///// </summary>
		//virtual void Render() = 0;

		/////////////////////////////////////////////////////////////////////////

		///// Shader methods

		/////////////////////////////////////////////////////////////////////////

		///// <summary>
		///// Compilation method, returns -1 if compilation failed or the shaderID
		///// </summary>
		///// <param name="_ShaderCode"> : Shader source code </param>
		///// <param name="_ShaderType"> : Shader type for the compilation (Vertex, Fragment) </param>
		///// <returns></returns>
		//virtual int CompileShader(const char* _ShaderCode, ShaderType _ShaderType) = 0;

		///// <summary>
		///// Links a vertex shader and a fragment shader, returns -1 if linking failed or the shaderProgramID
		///// </summary>
		///// <param name="_VertexShaderID"> : Vertex shader ID </param>
		///// <param name="_FragmentShaderID"> Fragment shader ID </param>
		///// <returns></returns>
		//virtual int LinkShaders(unsigned int _VertexShaderID, unsigned int _FragmentShaderID) = 0;

		///// <summary>
		///// Sets the shader program for rendering
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader program you want to use </param>
		//virtual void UseShader(unsigned int _ShaderProgramID) const = 0;

		///// <summary>
		///// Sets a bool to a choosen shader
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader you want to send the data </param>
		///// <param name="_Name"> : Name of the variable in the shader </param>
		///// <param name="_Value"> : Value you want to send </param>
		//virtual void SetBool(unsigned int _ShaderProgramID, const std::string& _Name, bool _Value) const = 0;

		///// <summary>
		///// Sets a int to a choosen shader
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader you want to send the data </param>
		///// <param name="_Name"> : Name of the variable in the shader </param>
		///// <param name="_Value"> : Value you want to send </param>
		//virtual void SetInt(unsigned int _ShaderProgramID, const std::string& _Name, int _Value) const = 0;

		///// <summary>
		///// Sets a float to a choosen shader
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader you want to send the data </param>
		///// <param name="_Name"> : Name of the variable in the shader </param>
		///// <param name="_Value"> : Value you want to send </param>
		//virtual void SetFloat(unsigned int _ShaderProgramID, const std::string& _Name, float _Value) const = 0;

		///// <summary>
		///// Sets a matrix4 to a choosen shader
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader you want to send the data </param>
		///// <param name="_Name"> : Name of the variable in the shader </param>
		///// <param name="_Nbr"> : Number of matrices you want to send </param>
		///// <param name="_Transpose"> : If you want to transpose the matrix before sending it </param>
		///// <param name="_Value"> : Value you want to send </param>
		//virtual void SetMat4(unsigned int _ShaderProgramID, const std::string& _Name, int _Nbr, bool _Transpose, float* _Value) const = 0;

		///// <summary>
		///// Sets a matrix3 to a choosen shader
		///// </summary>
		///// <param name="_ShaderProgramID"> : Shader you want to send the data </param>
		///// <param name="_Name"> : Name of the variable in the shader </param>
		///// <param name="_Nbr"> : Number of matrices you want to send </param>
		///// <param name="_Transpose"> : If you want to transpose the matrix before sending it </param>
		///// <param name="_Value"> : Value you want to send </param>
		//virtual void SetMat3(unsigned int _ShaderProgramID, const std::string& _Name, int _Nbr, bool _Transpose, float* _Value) const = 0;

		/////////////////////////////////////////////////////////////////////////

		///// Model methods

		/////////////////////////////////////////////////////////////////////////

		///// <summary>
		///// Loads a model data in the GPU
		///// </summary>
		///// <param name="_vertexNbr"> : Number of vertices </param>
		///// <param name="_indicesNbr"> : Number of indices </param>
		///// <param name="_vertex"> : Vertices data </param>
		///// <param name="_indices"> : Indices data </param>
		///// <returns></returns>
		////virtual int LoadModel(const size_t _vertexNbr, const size_t _indicesNbr, const Resources::Vertex* _vertex, const uint32_t* _indices) = 0;

		///// <summary>
		///// Unloads a model data from the GPU
		///// </summary>
		///// <param name="_ModelID"> : Model you want to unload </param>
		///// <returns></returns>
		//virtual bool UnloadModel(const unsigned int _ModelID) = 0;

		///// <summary>
		///// Draws a model specified by its ID
		///// </summary>
		///// <param name="_ModelID"> : ID of the model you want to draw </param>
		//virtual void DrawModel(const unsigned int _ModelID, const size_t _IndicesNbr) const = 0;

		///// <summary>
		///// Draw a model with a texture speicified by their IDs
		///// </summary>
		///// <param name="_ModelID">: ID of the model you want to draw </param>
		///// <param name="_TetxureID"> : ID of the texture you want to draw </param>
		//virtual void DrawTexturedModel(const unsigned int _ModelID, const size_t _IndicesNbr, const unsigned int _TetxureID) const = 0;

		/////////////////////////////////////////////////////////////////////////

		///// Texture methods

		/////////////////////////////////////////////////////////////////////////

		///// <summary>
		///// Loads a texture data in the GPU
		///// </summary>
		///// <param name="_TextureID"> : ID of the texture </param>
		///// <param name="_TextureData"> : Data contained in the texture </param>
		///// <param name="_Width"> : Width of the texture </param>
		///// <param name="_Height"> : Height of the texture </param>
		///// <param name="_ChannelNbr"> : Number of channels of the texture </param>
		///// <returns></returns>
		//virtual int LoadTexture(unsigned int& _TextureID, unsigned char* _TextureData, const unsigned int _Width, const unsigned int _Height, const unsigned int _ChannelNbr) = 0;

		///// <summary>
		///// Unloads a texture data from the GPU
		///// </summary>
		///// <param name="_TextureID"> : ID of the texture you want to unload </param>
		///// <returns></returns>
		//virtual bool UnloadTexture(const unsigned int _TextureID) = 0;
	};
}