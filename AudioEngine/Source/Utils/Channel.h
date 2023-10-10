#pragma once
#include <string>

// Forward Declaration
namespace FMOD
{
	class Channel;
}

using FMOD::Channel;

namespace FanshaweGameEngine
{
	namespace Audio
	{


		class Channel
		{

		public:

			// For now until i figure out a good way to do getetr and setters with pointers
			// the fmod channel api pointer
			FMOD::Channel* m_fmodChannel;


			Channel() = default;
			~Channel() = default;

			const std::string& GetName() const;
			const float GetVolume() const;
			const float GetPitch() const;
			const float GetPan() const;
			
			

			void SetName(const std::string& newName);
			void SetVolume(const float newVolume);
			void SetPan(const float newPan);
			void SetPitch(const float newPitch);
			




		private:

			// the friendly name of the channel
			std::string m_name;

			

			// the voulme of the channel
			float m_volume;

			// pitch value of the channel
			float m_pitch;

			// pan of the sound playing on this channel
			float m_pan;

			

		};
	}
}


