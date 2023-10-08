#pragma once
#include <fmod.hpp>
#include <map>
#include <string>



namespace FanshaweGameEngine
{
	namespace Audio
	{
		// Forwward Declaring for faster compile times
		class AudioClip;

	

		class AudioManager
		{

			// The Function to check for FMOD errors : This returns the error result along with the line where the error occured
			static void  ErrorCheckFMod(FMOD_RESULT result, int line);
			#define CHECKFMODERR(fModResult) ErrorCheckFMod(fModResult,__LINE__); 


			// Type Definitions for readability
			typedef std::map<std::string, FMOD::Sound*> SoundMap;
			typedef std::map<std::string, FMOD::Channel*> ChannelMap;


		public:
			// Constructor
			AudioManager();

			//Destructor
			~AudioManager();

			// Initialized the Core Audio Systems
			static void Init();


			// Called every Frame of the Gameloop
			static void Update();


			// Called to Deactivate/ Shutdown the System after use
			static void Shutdown();


			// Sound Loading / Unloading
			bool LoadSound(AudioClip& clip);

			
			void UnloadSound(const AudioClip& clip);

			// Sound PlayBack
			void PlaySound(const AudioClip& clip);


			static AudioManager* GetCurrent();


		private:

			static AudioManager* audiomanagerImp;

			// The maximum number of Channels for the audio engine 
			static const uint32_t MAX_AUDIO_CHANNELS = 1024;

			FMOD::System* m_audiosystem = nullptr;


			


			SoundMap m_sounds;
			ChannelMap m_loopChannels;


		};

	}

}


