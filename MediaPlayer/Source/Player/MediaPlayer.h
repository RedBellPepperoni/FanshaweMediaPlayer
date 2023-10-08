#pragma once
#include <string>
#include <vector>

namespace FanshaweGameEngine
{
	//Forward Declaration
	namespace Audio
	{
		class AudioClip;
	}
	using Audio::AudioClip;


	//enum 




	namespace Runtime
	{
		
		
		



		class MediaPlayer
		{

			using AudioClipList = std::vector<AudioClip>;

		public:
			
			MediaPlayer();
			~MediaPlayer();

			// the Setup fucntion dealting with initializing the audio manager and other important bits
			void Init();

			// Gameloop Update frame
			void Update();

			// call to exit the Media player
			void Quit();
			
			// Wrapper to load the correct clip to the audio manager
			void LoadClip(const std::string& filePath);

			// Wrapper to remove a prevoiusly loaded sound from the audio manager
			void DeleteClip(const std::string& filePath);

			// Wrapper to play a sound from the current index
			void PlayClip(const int index);

			// Wrapper to change the volume of the currently playing clip
			void SetClipVolume(const float newVolume);

			//Wrapper to set the looping state of the clip before playing it
			void SetClipLooping(const int index, const bool shouldLoop);

			//Wrapper to seek / pan along the length of the clip
			void SetClipPan(const float newPlayLocation);

			// Bool check to see if the application is pending exit
			const bool GetRunning() const;

			
			

		private:

			// Function to get the string name of the current playing clip : useful for display
			const std::string& GetClipName() const;

			
			void DrawCommandLineInterface();



			int m_currentClipIndex = 0;
			AudioClipList m_clipList;
			bool m_isRunning = false;
			
		};
	}
}

