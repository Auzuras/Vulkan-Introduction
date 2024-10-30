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

		inline Math::Vector3 GetPosition() const { return m_Position; }
		inline void SetPosition(Math::Vector3 _Position) { m_Position = _Position; }

		inline Math::Vector3 GetRotation() const { return m_Rotation; }
		inline void SetRotation(Math::Vector3 _Rotation) { m_Rotation = _Rotation; }

		inline Math::Vector3 GetScale() const { return m_Scale; }
		inline void SetScale(Math::Vector3 _Scale) { m_Scale = _Scale; }
	};
}