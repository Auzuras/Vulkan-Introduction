#pragma once

#include "Transform.h"
#include <vector>

namespace Core
{
	class IDescriptor;
	class IBuffer;
}

namespace LowRenderer
{
	class Object
	{
	private:

	protected:
		std::vector<Core::IDescriptor*> p_Descriptors;
		std::vector<Core::IBuffer*> p_UniformBuffers;
		
	public:
		Physics::Transform m_Transform;

		/// <summary>
		/// Object transform getter
		/// </summary>
		/// <returns></returns>
		inline Physics::Transform GetTransform() const { return m_Transform; }

		inline Core::IDescriptor* GetDescriptor(unsigned int _CurrentFrame) { return p_Descriptors[_CurrentFrame]; }
		inline Core::IBuffer* GetUBO(unsigned int _CurrentFrame) { return p_UniformBuffers[_CurrentFrame]; }

		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}