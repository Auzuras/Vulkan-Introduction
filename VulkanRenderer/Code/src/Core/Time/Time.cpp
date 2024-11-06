#include "Time/Time.h"

namespace Core
{
	void Time::UpdateDeltaTime(const float _CurrentFrameTime)
	{
		deltaTime = _CurrentFrameTime - m_LastFrame;

		m_LastFrame = _CurrentFrameTime;
	}
}