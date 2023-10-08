#pragma once
#include <string>



namespace FanshaweGameEngine
{
	namespace Audio
	{


		class AudioClip
		{

		public:

			AudioClip();
			AudioClip(const std::string& filePath, float volume, bool shouldLoop = false);

			~AudioClip();


			const std::string GetID() const;
			const char* GetPath() const;

			const bool GetLoaded() const;
			const bool GetLooping() const;
			const float GetVolume() const;

			void SetLoaded(const bool hasloaded);
			void SetLooping(const bool shouldLoop);
			void SetVolume(const float newVolume);



		private:

			// For now the uniqueID is the disk path for the sound
			std::string m_uniqueId;

			const char*  m_filePath;

			// Stores if the Sound has been loaded into the Engine
			bool m_hasLoaded;

			// Store if the Sound is OneShot or looping
			bool m_shouldLoop;

			//Defines the current volume of the Sound
			float m_volume;



		};
	}
}