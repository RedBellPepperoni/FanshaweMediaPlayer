#include "Channel.h"

namespace FanshaweGameEngine
{
	namespace Audio
	{
		const std::string& Channel::GetName() const
		{
			return m_name;
		}
		const float Channel::GetVolume() const
		{
			return m_volume;
		}
		const float Channel::GetPitch() const
		{
			return m_pitch;
		}
		const float Channel::GetPan() const
		{
			return m_pan;
		}
		

		

		void Channel::SetName(const std::string& newName)
		{
			m_name = newName;
		}
		void Channel::SetVolume(const float newVolume)
		{
			if (newVolume < 0.0f)
			{
				m_volume = 0.0f;
			}
			else if (newVolume > 1.0f)
			{
				m_volume = 1.0f;
			}
			else
			{
				m_volume = newVolume;
			}
			
		}
		void Channel::SetPan(const float newPan)
		{
			if (newPan < -1.0f)
			{
				m_pan = -1.0f;
			}
			else if (newPan > 1.0f)
			{
				m_pan  = 1.0f;
			}
			else
			{
				m_pan = newPan;
			}
		}
		void Channel::SetPitch(const float newPitch)
		{
			m_pitch = newPitch;
		}
		
	}
}