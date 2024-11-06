#pragma once

namespace Core
{
	class Time
	{
	private:
		static inline float m_LastFrame = 0.f;

	public:
		static inline float deltaTime = 0.f;

		static void UpdateDeltaTime(const float _CurrentFrameTime);
	};
}