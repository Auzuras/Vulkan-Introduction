#include "Transform.h"

namespace Physics
{
	Math::Vector3 Transform::GetPosition() const
	{
		return m_Position;
	}

	void Transform::SetPosition(Math::Vector3 _Position)
	{
		m_Position = _Position;
	}

	Math::Vector3 Transform::GetRotation() const
	{
		return m_Rotation;
	}

	void Transform::SetRotation(Math::Vector3 _Rotation)
	{
		m_Rotation = _Rotation;
	}

	Math::Vector3 Transform::GetScale() const
	{
		return Math::Vector3();
	}

	void Transform::SetScale(Math::Vector3 _Scale)
	{
		m_Scale = _Scale;
	}
}