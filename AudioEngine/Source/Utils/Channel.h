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

			


			

			Channel() = default;
			~Channel() = default;

			const std::string& GetName() const;
			const float GetVolume() const;
			const float GetPitch() const;
			const float GetPan() const;
			const bool GetPaused() const;
			const bool IsActive() const;
			const unsigned int GetSeekPosition() const;


			
		    FMOD::Channel* GetFModChannel();

			void SetName(const std::string& newName);
			void SetVolume(const float newVolume);
			void SetPaused(const bool paused);
			void SetPan(const float newPan);
			void SetPitch(const float newPitch);
			void StopPlaying();
			
			



		private:

			// the friendly name of the channel
			std::string m_name;
		
			// The pan Location of the Audio
			// This cvariable is essentail since fmod api doesnt have a a getter for pan value , (it's all matricess in there)
			// This is a hacky way to store the value instead of doing matrix math
			float m_pan;


			// Setting the top and bottom limit of volume
			float m_maxVolume = 10.0f;
			float m_minVolume = 0.0f;

			// Setting the top and bottom limit of pitch
			float m_maxPitch = 2.0f;
			float m_minPitch = 0.1f;

		public:
		
			// For now until i figure out a good way to do getetr and setters with pointers
			// the fmod channel api pointer
			FMOD::Channel* m_fmodChannel;

		};
	}
}


