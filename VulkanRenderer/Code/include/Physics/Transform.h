#pragma once

#include "Vectors/Vector3.h"
#include "Matrices/Matrix4.h"

namespace Physics
{
	class Transform
	{
	private:
		Math::Vector3 m_Position = Math::Vector3::zero;
		Math::Vector3 m_Rotation = Math::Vector3::zero;
		Math::Vector3 m_Scale = Math::Vector3::one;

		Math::Vector3 m_Right = Math::Vector3::right;
		Math::Vector3 m_Up = Math::Vector3::up;
		Math::Vector3 m_Forward = Math::Vector3::forward;

		Math::Matrix4 m_LocalTRS = Math::Matrix4::identity;

	public:
		bool isChanged = false;

		///////////////////////////////////////////////////////////////////////

		/// Getters and Setters methods

		///////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Position getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetPosition() const { return m_Position; }

		/// <summary>
		/// Position setter
		/// </summary>
		/// <param name="_Position">: New position </param>
		inline void SetPosition(Math::Vector3 _Position) { m_Position = _Position; }

		/// <summary>
		/// Rotation getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetRotation() const { return m_Rotation; }

		/// <summary>
		/// Rotation setter
		/// </summary>
		/// <param name="_Rotation">: New rotation </param>
		inline void SetRotation(Math::Vector3 _Rotation) { m_Rotation = _Rotation; }

		/// <summary>
		/// Scale getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetScale() const { return m_Scale; }

		/// <summary>
		/// Scale setter
		/// </summary>
		/// <param name="_Scale">: New scale </param>
		inline void SetScale(Math::Vector3 _Scale) { m_Scale = _Scale; }

		/// <summary>
		/// Right vector getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetRight() const { return m_Right; }

		/// <summary>
		/// Up vector getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetUp() const { return m_Up; }

		/// <summary>
		/// Forward vector getter
		/// </summary>
		/// <returns></returns>
		inline Math::Vector3 GetFowrward() const { return m_Forward; }
	};
}