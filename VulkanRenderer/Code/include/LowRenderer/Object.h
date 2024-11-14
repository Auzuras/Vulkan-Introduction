#pragma once

#include "Transform.h"
#include "Renderer.h"

namespace LowRenderer
{
	class Object
	{
	private:
		Physics::Transform m_Transform;

	protected:
		std::vector<Core::IDescriptor*> p_Descriptors;
		std::vector<Core::IBuffer*> p_UniformBuffers;
		
	public:

		/// <summary>
		/// Object transform getter
		/// </summary>
		/// <returns></returns>
		inline Physics::Transform GetTransform() const { return m_Transform; }

		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}