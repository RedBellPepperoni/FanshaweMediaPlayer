#include "MediaPlayer.h"
#include "Manager/AudioManager.h"
#include "Utils/AudioClip.h"
#include "Input/InputManager.h"
#include <iostream>



namespace FanshaweGameEngine
{
	// Forward Declarartion
	using Audio::AudioManager;
	using Input::InputManager;
	

	namespace Runtime
	{
		


		MediaPlayer::MediaPlayer()
		{
			m_cli = nullptr;
			m_cli = new UI::CommandLineInterface();
		}
		MediaPlayer::~MediaPlayer()
		{
			delete m_cli;
		}

		void MediaPlayer::Init()
		{
			// initializing the Audio manager singleton
			AudioManager::Init();

			m_isRunning = true;

			//DrawCommandLineInterface();

			m_cli->Init();

			state = PlayerState::Splash;
			
		}


		void MediaPlayer::Update()
		{

			//DrawCommandLineInterface();

			InputManager::ProcessInput();



			// See if the Quit key is pressed, it has the highest priority
			if (InputManager::GetKeyPressed(Input::Key::Quit))
			{
				// Quitting the application instantly
				Quit();
				return;
			}

			if (InputManager::GetKeyPressed(Input::Key::SelectClip))
			{
				std::cout << "Enter Pressed" << std::endl;

				// play the selected clip from begining
				PlayNewClip(m_selectedClipIndex);

			}

			if (InputManager::GetKeyPressed(Input::Key::NextClip))
			{
				// Go to the next clip index
				m_selectedClipIndex += 1;

				// Out of bounds error check, if there is no next clip we loop back to the first clip in the list
				if (m_selectedClipIndex >= m_clipList.size())
				{
					m_selectedClipIndex = 0;
				}

				//std::cout << "Selected Clip : " << m_clipList[m_selectedClipIndex].GetID() << std::endl;

			}

			if (InputManager::GetKeyPressed(Input::Key::PrevClip))
			{
				// Go to the next clip index
				m_selectedClipIndex -= 1;

				// Out of bounds error check, if there is no previous clip we loop back to the last clip in the list
				if (m_selectedClipIndex < 0)
				{
					m_selectedClipIndex = (m_clipList.size() -1);
				}

				std::cout << "Selected Clip : " << m_clipList[m_selectedClipIndex].GetID() << std::endl;

			}



			if (InputManager::GetKeyPressed(Input::Key::PlayPause))
			{
				if (state == PlayerState::Splash)
				{
					state = PlayerState::Player;
				}

				else if (state == PlayerState::Player)
				{
					PlayPause();
				}
			}


			if (InputManager::GetKeyPressed(Input::Key::VolumeUp))
			{
				float currentvolume = AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex);
				SetClipVolume(currentvolume += 0.25f);
			}

			if (InputManager::GetKeyPressed(Input::Key::VolumeDown))
			{
				float currentvolume = AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex);
				SetClipVolume(currentvolume -= 0.25f);

			}


			if (InputManager::GetKeyPressed(Input::Key::VolumeUp))
			{
				
			}

			AudioManager::Update();


			unsigned int getClipSeek = 0;
			AudioManager::GetCurrent()->GetPlaybackPosition(m_latestChannelIndex, getClipSeek);
			
			m_cli->displayData.currentPlaytime = getClipSeek;

			


			if (m_cli->displayData.currentPlaytime >= m_cli->displayData.maxPlaytime)
			{
				m_cli->displayData.consoleError = "Song Has Ened";
				m_cli->displayData.isActive = false;

			}
			

			m_cli->DrawFrame(state == PlayerState::Splash);
			m_cli->SwapBuffers();


		}

		void MediaPlayer::Quit()
		{
			// making sure we unload all the sounds from the FMOD API before exiting teh application
			UnloadAllSounds();

			// Clearing out the data in the Clip list
			m_clipList.clear();

			// Shutting down the audio manager
			AudioManager::Shutdown();

			m_isRunning = false;
		}

		void MediaPlayer::LoadClip(const std::string& filePath)
		{	
			// Craeting a new Audio Clip with the Following Data;
			AudioClip newClip(filePath , 1.0);

			// Loading the clip if its really unique
			bool clipLoadedSuccessfully = AudioManager::GetCurrent()->LoadSound(newClip);

			if (!clipLoadedSuccessfully)
			{
				// Output Error to UI
				return;
			}

			m_cli->displayData.consoleError = newClip.GetID() + " : Loaded SuccessFully";

			// The given clip was unique, so loading it
			m_clipList.emplace_back(newClip);

			m_cli->displayData.songnameList.emplace_back(newClip.GetID());

			


		}


		

		void MediaPlayer::PlayNewClip(const int index)
		{
	
			if(index >= m_clipList.size() || index < 0)
			{ 
				// Error Clip not found
				return;
			}


			//std::cout << "Playing : " << m_clipList[index].GetID() << std::endl;

			m_playingClipIndex = index;

			m_latestChannelIndex = AudioManager::GetCurrent()->PlayNewSound(m_clipList[m_playingClipIndex]);
	

 			m_cli->displayData.maxPlaytime = AudioManager::GetCurrent()->GetSoundCliplength();
			m_cli->displayData.isActive = true;
			m_cli->displayData.isPlaying = true;
			m_cli->displayData.audioTitle = m_clipList[m_playingClipIndex].GetID();

			m_cli->displayData.consoleError = " Currently Playing : " + m_clipList[m_playingClipIndex].GetID();
		}



		void MediaPlayer::SetClipVolume(const float newVolume)
		{
			float volume = newVolume > 10.0f ? 10.0f : newVolume;
		    volume = newVolume < 0.0f ? 0.0f : newVolume;

			AudioManager::GetCurrent()->SetChannelVolume(m_latestChannelIndex, volume);
		}

		void MediaPlayer::PlayPause()
		{
		
			if (m_cli->displayData.isActive)
			{
				AudioManager::GetCurrent()->PlayPauseToggle(m_latestChannelIndex);

				m_cli->displayData.isPlaying = !AudioManager::GetCurrent()->GetChannelPaused(m_latestChannelIndex);
				m_cli->displayData.consoleError = " Currently Playing : " + m_clipList[m_playingClipIndex].GetID();

				return;
			}
			
		}

		
		void MediaPlayer::SetClipPan(const float newPlayLocation)
		{
			float Pan = newPlayLocation > 1.0f ? 1.0f : newPlayLocation;
			Pan = newPlayLocation < -1.0f ? -1.0f : newPlayLocation;

			AudioManager::GetCurrent()->SetChannelPan(m_latestChannelIndex,Pan);
		}

		void MediaPlayer::SetPitch(const float newPitch)
		{
			float Pitch = newPitch > 2.0f ? 2.0f : newPitch;
			Pitch = newPitch < 0.5f ? 0.5f : newPitch;

			AudioManager::GetCurrent()->SetChannelPitch(m_latestChannelIndex,Pitch);
		}

		const bool MediaPlayer::GetRunning() const
		{
			return m_isRunning;
		}


		void MediaPlayer::UnloadAllSounds()
		{
			// loop through all the currently reference clips
			for (int iterator = 0; iterator < m_clipList.size(); iterator++)
			{
				// Storing the clip nas a temporary ref
				AudioClip tempClip = m_clipList[iterator];

				// if the clip is loaded in the Manager, unload it
				if (tempClip.GetLoaded())
				{	
					std::cout << "Media Player : Unloading Audio Clip " << tempClip.GetID() << std::endl;
					AudioManager::GetCurrent()->UnloadSound(tempClip);
				}
	
			}

			std::cout << "Media Player : All clips Unloaded Successfully" << std::endl;

		}
		const std::string& MediaPlayer::GetClipName() const
		{
			return m_clipList[m_playingClipIndex].GetID();
		}




		void MediaPlayer::DrawCommandLineInterface()
		{
			//ClearScreen();




			std::cout << "==================================================\n"
				      << "=                 Media Player                   =\n"
				      << "==================================================\n"
				      << "= Press : " << InputManager::GetKeyName(Input::Key::PlayPause) << " To Play/Pause                 =\n"
					  << "= Press : " << InputManager::GetKeyName(Input::Key::NextClip) << " To select next clip         =" << std::endl;
			std::cout << "= Press : " << InputManager::GetKeyName(Input::Key::PrevClip) << " To select previous clip       =" << std::endl;
			std::cout << "= Press : " << InputManager::GetKeyName(Input::Key::VolumeUp) << " To increase the Volume        =" << std::endl;
			std::cout << "= Press : " << InputManager::GetKeyName(Input::Key::VolumeDown) << " To decrease the Volume        =" << std::endl;
			std::cout << "==================================================" << std::endl;
			

					

		}


		
	}
}
