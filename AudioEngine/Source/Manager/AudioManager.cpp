#include "AudioManager.h"
#include <fmod_errors.h>
#include <iostream>
#include "Source/Utils/AudioClip.h"

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
			

			
		}

		void AudioManager::Update()
		{
		}

		void AudioManager::Shutdown()
		{
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


			FMOD::Sound* newSound = nullptr;

			// Create a new sound using FMOD craete
			CHECKFMODERR(m_audiosystem->createSound(clip.GetPath(), FMOD_DEFAULT, nullptr, &newSound));

			//
		//	CHECKFMODERR(newSound->setMode(clip.GetLooping()  ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF));

			// if the new sound is valid, we add it to the cache
			if (newSound)
			{
				m_sounds[clip.GetID()] = newSound;
			}


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


		void AudioManager::PlaySound(const AudioClip& clip)
		{
			

			typename SoundMap::iterator itr = m_sounds.find(clip.GetID());

			// If the give nsound is not found 
			if (itr == m_sounds.end())
			{
				std::cout << "Sound doesnt exist : " << clip.GetID() << std::endl;
				return;
			}

			// Setting Looping Bit Flag
			itr->second->setMode(clip.GetLooping() ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

			FMOD::Channel* newChannel = nullptr;

			// Set up the playback but pause the sound initially
			CHECKFMODERR(m_audiosystem->playSound(itr->second, 0, true, &newChannel));

			// Set the channel Volume fro mteh stored volume per Audio Clip
			newChannel->setVolume(clip.GetVolume());

			// if the clip is going to loop, we need to store a reference to it 
			if (clip.GetLooping())
			{
				m_loopChannels.insert({ clip.GetID(), newChannel });
			}

			// Start the play back
			CHECKFMODERR(newChannel->setPaused(false));

			
		}

	
#pragma endregion //Audio Manager Functionality
		

	}
}
