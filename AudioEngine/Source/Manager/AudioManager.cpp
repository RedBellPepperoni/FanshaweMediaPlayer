#include "AudioManager.h"
#include <fmod_errors.h>
#include <iostream>
#include "Source/Utils/AudioClip.h"
#include "Source/Utils/Channel.h"


namespace FanshaweGameEngine
{
	namespace Audio
	{

		std::string& ErrorCheckFMod(FMOD_RESULT result, int line)
		{
			// There is some Actual FMOD API error which is Real Bad
			if (result != FMOD_OK)
			{

				std::string errorMsg = "FMOD ERROR: AudioManager [Line : " + std::to_string(line) + "] " + std::to_string(result) + " - " + FMOD_ErrorString(result);
				return errorMsg;
			}
		}
		 




#pragma region Channel Implementation






#pragma endregion // Channel Implementation

		const std::string& Channel::GetName() const
		{
			return m_name;
		}


		FMOD::Channel* Channel::GetFModChannel()
		{
			return m_fmodChannel;
		}


		const float Channel::GetVolume() const
		{
			if (m_fmodChannel == nullptr)
			{
				return 1.0f;
			}

			float volume;

			CHECKFMODERR(m_fmodChannel->getVolume(&volume));

			return volume;
		}

		const float Channel::GetPitch() const
		{
			if (m_fmodChannel == nullptr)
			{
				return 1.0f;
			}

			float pitch;

			CHECKFMODERR(m_fmodChannel->getPitch(&pitch));

			return pitch;
		}
		const float Channel::GetPan() const
		{
			return m_pan;
		}

		const bool Channel::GetPaused() const
		{
			if (m_fmodChannel == nullptr)
			{
				return false;
			}

			bool paused;

			CHECKFMODERR(m_fmodChannel->getPaused(&paused));

			return paused;
		}

		const unsigned int Channel::GetSeekPosition() const
		{
			if (m_fmodChannel == nullptr)
			{
				return 0;
			}

			unsigned int seekPosition = 0;

			CHECKFMODERR(m_fmodChannel->getPosition(&seekPosition, FMOD_TIMEUNIT_MS));

			return seekPosition;
		}

		const bool Channel::IsActive() const
		{
			if (m_fmodChannel == nullptr)
			{
				return false;
			}

			bool isActive;

			CHECKFMODERR(m_fmodChannel->isPlaying(&isActive));

			return isActive;
		}




		void Channel::SetName(const std::string& newName)
		{		
			m_name = newName;
		}
		void Channel::SetVolume(const float newVolume)
		{
			if (m_fmodChannel == nullptr)
			{
				return;
			}

			// Clamping the volume so that it doesnt go higher or lower than the defined max and min

			float clampedVolume = newVolume;

			if (newVolume >= m_maxVolume)
			{
				clampedVolume = m_maxVolume;
			}
			else if (newVolume <= m_minVolume)
			{
				clampedVolume = m_minVolume;
			}
			

			// Actually setting the FMOD channel volume
			CHECKFMODERR(m_fmodChannel->setVolume(clampedVolume));

		}


		void Channel::SetPaused(const bool paused)
		{
			if (m_fmodChannel == nullptr)
			{
				return;
			}


			CHECKFMODERR(m_fmodChannel->setPaused(paused));
		}

		void Channel::SetPan(const float newPan)
		{
			if (m_fmodChannel == nullptr)
			{
				return;
			}

			// Clamping the Pan value so that it doesnt go beyond the standard
			// -1.0 is playing on the left channel, and 1.0 is playing on the right, 0 is the middle

			m_pan = newPan;

			if (newPan > 1.0f)
			{
				m_pan = 1.0f;
			}

			else if (newPan < -1.0f)
			{
				m_pan = -1.0f;
			}

			CHECKFMODERR(m_fmodChannel->setPan(m_pan));

		}

		void Channel::SetPitch(const float newPitch)
		{
			if (m_fmodChannel == nullptr)
			{
				return;
			}

			float clampedPitch = newPitch;

			if (newPitch >= m_maxPitch)
			{
				clampedPitch = m_maxPitch;
			}
			else if (newPitch <= m_minPitch)
			{
				clampedPitch = m_minPitch;
			}
	

			CHECKFMODERR(m_fmodChannel->setPitch(clampedPitch));
		}

		void Channel::StopPlaying()
		{
			if (m_fmodChannel == nullptr)
			{
				return;
			}

			CHECKFMODERR(m_fmodChannel->stop());
		}




#pragma region Audio Manager Setup

		// ================== AUDIO MANAGER FUNCTIONS =========================

		AudioManager* AudioManager::audiomanagerImp = nullptr;

	
		AudioManager* AudioManager::GetCurrent()
		{
			if (audiomanagerImp == nullptr)
			{
				std::cout << "ERROR: Audio Manager Not Initialized" << std::endl;
			}

			return audiomanagerImp;
		}

		//============================ C'tors and D'tors ==================================

		AudioManager::AudioManager()
		{
			
		}

		AudioManager::~AudioManager()
		{	
			delete audiomanagerImp;
		}


		// ====================== Static Methods =========================

		/*	
			This Initializes the pre required stuff including setting up the low level system, the channels 
		*/	

		void AudioManager::Init()
		{
			//  Audio manager Instance already exists
			if (audiomanagerImp != nullptr)
			{
				std::cout << "WARNING:  One Instance of Audio Manager already exists" << std::endl;
				return;
			}

			// Creating a instance of AudioManager
			audiomanagerImp = new AudioManager();

			// Creating the Fmod system and setting up the channels
			CHECKFMODERR(FMOD::System_Create(&GetCurrent()->m_audiosystem));
			CHECKFMODERR(GetCurrent()->m_audiosystem->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, nullptr));

			// Initialing Channels
			for (int i = 0; i < MAX_AUDIO_CHANNELS; i++)
			{
				// making a shared smart pointers for all the playback channels 
				GetCurrent()->m_channels.push_back(std::make_shared<Channel>());
			}
			
		}

		/*
			Should Update every fram :
		*/

		void AudioManager::Update()
		{
			// manager isnt initialized
			if (!audiomanagerImp)
			{
				return;
			}

			// Updating the low level system
			CHECKFMODERR(GetCurrent()->m_audiosystem->update());

		}

		void AudioManager::Shutdown()
		{
			// manager isnt initialized
			if (!audiomanagerImp)
			{
				return;
			}

			if (!GetCurrent()->m_audiosystem)
			{

			}

			// Closing the system
			CHECKFMODERR(GetCurrent()->m_audiosystem->close());
			// Finally releaseing the system
			CHECKFMODERR(GetCurrent()->m_audiosystem->release());
		}

		bool AudioManager::GetMultipleChannels()
		{
			return m_useMultipleChannels;
		}

#pragma endregion //Audio Manager Setup

#pragma region Audio Manager Functionality

		// ========================= FUNCTIONALITY IMPLEMENTATION =================================

		bool AudioManager::LoadSound(AudioClip& clip, LoadType type)
		{
			// initial Check t osee if the Sound clip file is already loaded
			if (clip.GetLoaded())
			{
				std::cout << "Audio Manager : " << clip.GetID() << "  : Already Loaded " << std::endl;
				return false;
			}
			

			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			// Secondary check to see if the same file is not load from differnt sources
			if (itr != m_sounds.end())
			{
				std::cout << "Audio Manager : " << clip.GetID() << "  : A Sound with the same name Already Exists " << std::endl;
				return false;
			}

			// Creating a new sound pointer to load the sound usingthe Api
			FMOD::Sound* newSound = nullptr;

			switch (type)
			{
			case FanshaweGameEngine::Audio::AudioManager::LoadType::SAMPLED:

				// Create a new sound using FMOD craete
				CHECKFMODERR(m_audiosystem->createSound(clip.GetPath(), FMOD_DEFAULT, nullptr, &newSound));


				break;

			case FanshaweGameEngine::Audio::AudioManager::LoadType::STREAMED:

				// Create a new sound using FMOD craete
				CHECKFMODERR(m_audiosystem->createStream(clip.GetPath(), FMOD_DEFAULT, nullptr, &newSound));

				break;

			}

			


			// if the new sound is valid, we add it to the cache
			if (newSound)
			{
				m_sounds[clip.GetID()] = newSound;
			}

			// Updating the data in the audio clip to show that the clip ha successfully loaded
			clip.SetLoaded(true);


			//std::cout << "Audio Manager : " << clip.GetID() << "  : Loaded Successfully " << std::endl;



			return true;
		}

		

		void AudioManager::UnloadSound(const AudioClip& clip)
		{
			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			if (itr == m_sounds.end())
			{
				// no sound found in the cache with the given name 
				//std::cout << "Audio Manager : " << clip.GetID() << "  : Unload Failed : Sound wasnt loaded " << std::endl;
				return;
			}

			// Release the Bound Sound
			CHECKFMODERR(itr->second->release());

			// Remove the sound from the cache
			m_sounds.erase(itr);

		}


		int AudioManager::PlayNewSound(const AudioClip& clip)
		{
			
			// Iterator for finding the loaded sound from clip list
			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			// If the give nsound is not found 
			if (itr == m_sounds.end())
			{
				std::cout << "Sound doesnt exist : " << clip.GetID() << std::endl;
				return m_nextChannelId;
			}

			

			// Setting Looping Bit Flag
			itr->second->setMode(clip.GetLooping() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);


			std::shared_ptr<Channel> selectedChannel;

			// Calculte the next Channel Id if needed
			int channelId = m_nextChannelId;
			

			if (m_useMultipleChannels)
			{
				// Calculte the next Channel Id if needed
				
				m_nextChannelId = (m_nextChannelId + 1) % 10;
				selectedChannel = m_channels[channelId];

				// Set the channel Volume to default
				selectedChannel->SetVolume(0.5f);
				selectedChannel->SetPitch(1.0f);
				selectedChannel->SetPan(0);
		

			}
			else 
			{
				selectedChannel = m_channels[0];
			}
			
			// If theres somehthing playing in the channel already, stop it
			if (selectedChannel->IsActive())
			{
				selectedChannel->StopPlaying();
			}
		
			// Set up the playback but pause the sound initially
			//CHECKFMODERR(m_audiosystem->playSound(itr->second, 0, true, &channelRef));
			CHECKFMODERR(m_audiosystem->playSound(itr->second, 0, true, &selectedChannel->m_fmodChannel));


			// Start the play back
			selectedChannel->SetPaused(false);

			// Storing the maximum length so that we can display it in the UI
			itr->second->getLength(&m_CurrentClipLength, FMOD_TIMEUNIT_MS);

		
			return channelId;
		}


		void AudioManager::PlayPauseToggle(int id)
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return;
			}

			
			bool isPaused = GetChannelPaused(id);

 			// Toggles the paused state
			m_channels[id]->SetPaused(!isPaused);
			
			
		}

		

	

		const bool AudioManager::GetChannelPaused(const int id) const
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return false;
			}
			
			return m_channels[id]->GetPaused();

		}

		const unsigned int AudioManager::GetSoundCliplength() const
		{
			return m_CurrentClipLength;
		}

		const float AudioManager::GetChannelVolume(const int id) const
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return 1.0f;
			}
	
			return m_channels[id]->GetVolume();
		}

		const float AudioManager::GetChannelPitch(const int id) const 
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return 1.0f;
			}

			return m_channels[id]->GetPitch();
				
		}

		const float AudioManager::GetChannelPan(const int id) const
		{// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return 0.0f;
			}

			return m_channels[id]->GetPan();
		}

		const unsigned int AudioManager::GetPlaybackPosition(const int id) const
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return 0;
			}

			return m_channels[id]->GetSeekPosition();


		}

		

		void AudioManager::SetChannelVolume(const float value, const int id)
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return;
			}

			m_channels[id]->SetVolume(value);

			

		}

		void AudioManager::SetChannelPitch(const float value, const int id)
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return;
			}
			
			m_channels[id]->SetPitch(value);

		}

		void AudioManager::SetChannelPan(const float value, const int id)
		{
			// making sure we are not tryign to get values from an non exisiting channel
			if (id >= m_channels.size())
			{
				return;
			}

			m_channels[id]->SetPan(value);
		}

		

		

		
		

	
#pragma endregion //Audio Manager Functionality
		

	}
}
