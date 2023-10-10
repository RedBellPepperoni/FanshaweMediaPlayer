#include "AudioManager.h"
#include <fmod_errors.h>
#include <iostream>
#include "Source/Utils/AudioClip.h"
#include "Source/Utils/Channel.h"

namespace FanshaweGameEngine
{
	namespace Audio
	{

#pragma region Error Checking

		// ================= ERROR CHECKING Function for FMOD error results ================================

		void AudioManager::ErrorCheckFMod(FMOD_RESULT result, int line)
		{


			if (result != FMOD_OK)
			{
				std::cout << "FMOD ERROR: AudioManager [Line : " << line << "] " << result << "  - " << FMOD_ErrorString(result) << std::endl;
			}

		}

#pragma endregion //Error Checking



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

			// Creating the Fmod system
			CHECKFMODERR(FMOD::System_Create(&GetCurrent()->m_audiosystem));
			CHECKFMODERR(GetCurrent()->m_audiosystem->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, nullptr));


			for (int i = 0; i < MAX_AUDIO_CHANNELS; i++)
			{
				GetCurrent()->m_channels.push_back(new Channel);
			}
			

			
		}

		void AudioManager::Update()
		{
			// manager isnt initialized
			if (!audiomanagerImp)
			{
				return;
			}


			



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

#pragma endregion //Audio Manager Setup

#pragma region Audio Manager Functionality

		// ========================= FUNCTIONALITY IMPLEMENTATION =================================

		bool AudioManager::LoadSound(AudioClip& clip)
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

			// Create a new sound using FMOD craete
			CHECKFMODERR(m_audiosystem->createSound(clip.GetPath(), FMOD_DEFAULT, nullptr, &newSound));


			// if the new sound is valid, we add it to the cache
			if (newSound)
			{
				m_sounds[clip.GetID()] = newSound;
			}

			// Updating the data in the audio clip to show that the clip ha successfully loaded
			clip.SetLoaded(true);

			std::cout << "Audio Manager : " << clip.GetID() << "  : Loaded Successfully " << std::endl;



			return true;
		}

		void AudioManager::UnloadSound(const AudioClip& clip)
		{
			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			if (itr == m_sounds.end())
			{
				// no sound found in the cache with the given name 
				std::cout << "Audio Manager : " << clip.GetID() << "  : Unload Failed : Sound wasnt loaded " << std::endl;
				return;
			}

			// Release the Bound Sound
			CHECKFMODERR(itr->second->release());

			// Remove the sound from the cache
			m_sounds.erase(itr);

		}


		int AudioManager::PlayNewSound(const AudioClip& clip)
		{
			

			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			// If the give nsound is not found 
			if (itr == m_sounds.end())
			{
				std::cout << "Sound doesnt exist : " << clip.GetID() << std::endl;
				return m_nextChannelId;
			}


			// Setting Looping Bit Flag
			itr->second->setMode(clip.GetLooping() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);


			

			int channelId = m_nextChannelId;
			m_nextChannelId = (m_nextChannelId + 1) % 10;
			Channel* channel = m_channels[channelId];

			
			
			
			// Set up the playback but pause the sound initially
			CHECKFMODERR(m_audiosystem->playSound(itr->second, 0, true, &channel->m_fmodChannel));

			// Set the channel Volume to default
			channel->SetVolume(0.5f);
			
			// if the clip is going to loop, we need to store a reference to it 
			//if (clip.GetLooping())
			//{
			//	m_loopChannels.insert({ clip.GetID(), newChannel });
			//}

			// Start the play back
			CHECKFMODERR(channel->m_fmodChannel->setPaused(false));

			itr->second->getLength(&m_CurrentClipLength, FMOD_TIMEUNIT_MS);

		
			return channelId;

			
		}


		void AudioManager::PlayPauseToggle(int id)
		{
			if (id >= m_channels.size())
			{
				return;
			}

			
			bool isPaused = GetChannelPaused(id);
 			if (isPaused)
			{
				CHECKFMODERR(m_channels[id]->m_fmodChannel->setPaused(false));
				return;
			}
			
			CHECKFMODERR(m_channels[id]->m_fmodChannel->setPaused(true));
			
		}

		

	

		bool AudioManager::GetChannelPaused(const int id)
		{
			if (id >= m_channels.size())
			{
				return false;
			}

			bool paused;

			CHECKFMODERR(m_channels[id]->m_fmodChannel->getPaused(&paused));

			return paused;

		}

		unsigned int AudioManager::GetSoundCliplength()
		{
			return m_CurrentClipLength;
		}

		float AudioManager::GetChannelVolume(const int id)
		{
			float volume;
			CHECKFMODERR(m_channels[id]->m_fmodChannel->getVolume(&volume));
			return volume;
		}

		

		

		void AudioManager::SetChannelVolume(const int id, const float value)
		{
			if (id >= m_channels.size())
			{
				return;
			}

			m_channels[id]->SetVolume(value);
			CHECKFMODERR(m_channels[id]->m_fmodChannel->setVolume(m_channels[id]->GetVolume()));

		}

		void AudioManager::SetChannelPitch(const int id, const float value)
		{
			if (id >= m_channels.size())
			{
				return;
			}
			
			CHECKFMODERR(m_channels[id]->m_fmodChannel->setPitch(value));

		}

		void AudioManager::SetChannelPan(const int id, const float value)
		{
		}

		void AudioManager::GetPlaybackPosition(int id, unsigned int& currentValue)
		{
			if (id >= m_channels.size() || m_channels[id]->m_fmodChannel == nullptr)
			{
				return;
			}

			CHECKFMODERR(m_channels[id]->m_fmodChannel->getPosition(&currentValue, FMOD_TIMEUNIT_MS));
			

		}

		

		
		

	
#pragma endregion //Audio Manager Functionality
		

	}
}
