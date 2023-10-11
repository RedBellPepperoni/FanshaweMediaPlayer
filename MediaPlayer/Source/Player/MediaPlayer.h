#pragma once
#include <string>
#include <vector>
#include <memory>


namespace FanshaweGameEngine
{
	//Forward Declaration
	namespace Audio
	{
		class AudioClip;
	}
	using Audio::AudioClip;

	namespace UI
	{
		class CommandLineInterface;
	}

	//enum 




	namespace Runtime
	{
		
		
		enum PlayerState
		{
			Splash,
			Player
		};



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
			void LoadClip(const std::string& filePath, bool isStreamed = false);

			

			// Wrapper to play a sound from the current index
			void PlayNewClip(const int index);

			// Wrapper to change the volume of the currently playing clip
			void SetClipVolume(const float newVolume);

			void PlayPause();
	

			//Wrapper to seek / pan along the length of the clip
			void SetClipPan(const float newPan);

			//Wrapper to seek / pan along the length of the clip
			void SetClipPitch(const float newPitch);

			

			// Bool check to see if the application is pending exit
			const bool GetRunning() const;


			void UnloadAllSounds();
			
			

		private:

			// Function to get the string name of the current playing clip : useful for display
			const std::string& GetClipName() const;

			
			


			// the index ofthe clip that is currenly playing sound
			int m_playingClipIndex = 0;

			// the index of the clip that is selected in the UI
			// if this is the same as playing CLip : pressing spacebar will toggle pasue/play
			// otherwise if its different then the pressing spacebar will play the 
			int m_selectedClipIndex = 0;

			// The channel what is playing the audio. (the lastest one that was given a sound to play)
			int m_latestChannelIndex = 0;

			// A Storage container to store all the loaded clips sequentially
			AudioClipList m_clipList;

			bool m_isRunning = false;

			// The current state of teh User Interface
			PlayerState state;
			
			// The Reference to the Command line UI
			std::unique_ptr<UI::CommandLineInterface> m_cmdUI;


			
		};
	}
}

