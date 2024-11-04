#pragma once

#include "Transform.h"

namespace LowRenderer
{
	class Object
	{
	private:
		Physics::Transform m_Transform;

	public:

		/// <summary>
		/// Object transform getter
		/// </summary>
		/// <returns></returns>
		inline Physics::Transform GetTransform() const { return m_Transform; }
	};
}