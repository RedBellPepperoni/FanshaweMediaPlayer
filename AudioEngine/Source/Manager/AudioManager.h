#pragma once
#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>



namespace FanshaweGameEngine
{
	namespace Audio
	{
		// Forwward Declaring for faster compile times
		class AudioClip;
		class Channel;
	

		class AudioManager
		{

			// The Function to check for FMOD errors : This returns the error result along with the line where the error occured
			static void  ErrorCheckFMod(FMOD_RESULT result, int line);
			#define CHECKFMODERR(fModResult) ErrorCheckFMod(fModResult,__LINE__); 


			// Type Definitions for readability
			typedef std::map<std::string, FMOD::Sound*> SoundMap;
			typedef std::vector<Channel*> ChannelList;


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
			int PlayNewSound(const AudioClip& clip);


			void PlayPauseToggle(int id);

			

			bool GetChannelPaused(const int id);

			unsigned int GetSoundCliplength();

			float GetChannelVolume(const int id);
			
			// Modifying Channel Values
			void SetChannelVolume(const int id, const float value);
			void SetChannelPitch(const int id, const float value);
			void SetChannelPan(const int id, const float value);

			void GetPlaybackPosition(int id, unsigned int& currentValue);


			static AudioManager* GetCurrent();




		private:

			static AudioManager* audiomanagerImp;

			// The maximum number of Channels for the audio engine 
			static const uint32_t MAX_AUDIO_CHANNELS = 16;

			FMOD::System* m_audiosystem = nullptr;


			int m_nextChannelId;


			unsigned int m_CurrentClipLength = 0;

			SoundMap m_sounds;
			ChannelList m_channels;

			

		};

	}

}


