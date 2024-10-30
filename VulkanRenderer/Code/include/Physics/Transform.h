#pragma once

#include "Vectors/Vector3.h"

namespace Physics
{
	class Transform
	{
	private:
		Math::Vector3 m_Position;
		Math::Vector3 m_Rotation;
		Math::Vector3 m_Scale;
	public:
		bool isChanged = false;

		Math::Vector3 GetPosition() const;
		void SetPosition(Math::Vector3 _Position);

		Math::Vector3 GetRotation() const;
		void SetRotation(Math::Vector3 _Rotation);

		Math::Vector3 GetScale() const;
		void SetScale(Math::Vector3 _Scale);
	};
}