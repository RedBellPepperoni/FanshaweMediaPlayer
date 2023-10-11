#pragma once
#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>




namespace FanshaweGameEngine
{
	namespace Audio
	{
		// Forwward Declaring for faster compile times
		class AudioClip;
		class Channel;
	
		
		// ================= ERROR CHECKING Function for FMOD error results ================================
		// The Function to check for FMOD errors : This returns the error result along with the line where the error occured
		static std::string& ErrorCheckFMod(FMOD_RESULT result, int line);

#define CHECKFMODERR(fModResult) ErrorCheckFMod(fModResult,__LINE__); 


		class AudioManager
		{

			


			// Type Definitions for readability
			typedef std::map<std::string, FMOD::Sound*> SoundMap;
			typedef std::vector<std::shared_ptr<Channel>> ChannelList;


		public:

			enum class LoadType
			{
				SAMPLED,
				STREAMED
			};



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


			bool GetMultipleChannels();

			// Sound Loading / Unloading
			bool LoadSound(AudioClip& clip, LoadType type = LoadType::SAMPLED);
			

			void UnloadSound(const AudioClip& clip);

			// Sound PlayBack
			int PlayNewSound(const AudioClip& clip);

			// The play Pause toggle
			void PlayPauseToggle(int id = 0);

			// Getter for checking if the current channel is paused
			const bool GetChannelPaused(const int id = 0) const;

			// The Current playing sound's total legnth
			const unsigned int GetSoundCliplength() const;

			const float GetChannelVolume(const int id = 0) const;
			const float GetChannelPitch(const int id = 0) const;
			const float GetChannelPan(const int id = 0) const;

			// The position of the audio playback
			const unsigned int GetPlaybackPosition(const int id = 0) const;
			
			// Modifying Channel Values
			void SetChannelVolume(const float value,const int id = 0);
			void SetChannelPitch(const float value,const int id = 0);
			void SetChannelPan(const float value, const int id = 0);


			


			// The Reference to the implementation of the audio manager
			static AudioManager* GetCurrent();


			

		private:

			static AudioManager* audiomanagerImp;

			// The maximum number of Channels for the audio engine 
			static const uint32_t MAX_AUDIO_CHANNELS = 16;


			// An additioanl option, not using it since media player should only have one channel
			bool m_useMultipleChannels = false;




			// pointer to the audiosubsysem from the Fmod api
			FMOD::System* m_audiosystem = nullptr;

			int m_nextChannelId = 0;

			// The current seek position (how many miliseconds we are into the playback)
			unsigned int m_CurrentClipLength = 0;

			

			// The list of all the currently loaded (playable sounds)
			SoundMap m_sounds;

			// Extera storage for channels if multiple audio playback is needed
			// Here the first channel (m_channel[0]) is always reserved for one shot sounds;
			ChannelList m_channels;

			

		};

	}

}


