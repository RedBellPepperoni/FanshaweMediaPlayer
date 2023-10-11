#include "MediaPlayer.h"
#include "Manager/AudioManager.h"
#include "Utils/AudioClip.h"
#include "Input/InputManager.h"
#include "UserInterface/CommandLineInterface.h"
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
			m_cmdUI = nullptr;
			m_cmdUI = std::make_unique<UI::CommandLineInterface>();
		}
		MediaPlayer::~MediaPlayer()
		{
			
		}


		void MediaPlayer::Init()
		{
			// initializing the Audio manager singleton
			AudioManager::Init();


			m_isRunning = true;


			// Initialing the command line UI
			m_cmdUI->Init();

			// Setting so that the first screen loaded is the splash screen
			state = PlayerState::Splash;
			
		}


		void MediaPlayer::Update()
		{

			// Process all the Key presses and store their tate using the windows api
			InputManager::ProcessInput();


			// Draw the Frame for the depending on the screen (draw the spalsh initialy)
			m_cmdUI->DrawFrame(state == PlayerState::Splash);

			// Double buffer swaping to eliminate screen tearing
			m_cmdUI->SwapBuffers();


			// See if the Quit key is pressed, it has the highest priority
			if (InputManager::GetKeyPressed(Input::Key::Quit))
			{
				// Quitting the application instantly
				Quit();
				return;
			}


			if (InputManager::GetKeyPressed(Input::Key::PlayPause))
			{
				// The splash scrren is active , so a space bar press will take us to the media player
				if (state == PlayerState::Splash)
				{
					state = PlayerState::Player;
				}

				// We are aleady using the media player, so now use the actual play/pause functionality
				else if (state == PlayerState::Player)
				{
					PlayPause();
				}
			}

			// We havent loaded the media player (still on splash scren)
			if (state == PlayerState::Splash)
			{
				// Just here to stop inputs detection till the splashscreen is skipped
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

				m_cmdUI->displayData.selectedSongIndex = m_selectedClipIndex;

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

				m_cmdUI->displayData.selectedSongIndex = m_selectedClipIndex;

				//std::cout << "Selected Clip : " << m_clipList[m_selectedClipIndex].GetID() << std::endl;

			}

			if (InputManager::GetKeyPressed(Input::Key::VolumeUp))
			{


				float currentvolume = AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex);
				SetClipVolume(currentvolume += 0.20f);

				m_cmdUI->displayData.consoleError = "Volume+ : " + std::to_string(AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex));
			}

			if (InputManager::GetKeyPressed(Input::Key::VolumeDown))
			{
				float currentvolume = AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex);
				SetClipVolume(currentvolume -= 0.20f);

				m_cmdUI->displayData.consoleError = "Volume- : " + std::to_string(AudioManager::GetCurrent()->GetChannelVolume(m_latestChannelIndex));

			}

			if (InputManager::GetKeyPressed(Input::Key::PitchUp))
			{
				float currentPitch = AudioManager::GetCurrent()->GetChannelPitch(m_latestChannelIndex);
				SetClipPitch(currentPitch += 0.10f);

				m_cmdUI->displayData.consoleError = "Pitch+ : " + std::to_string(AudioManager::GetCurrent()->GetChannelPitch(m_latestChannelIndex));
			}

			if (InputManager::GetKeyPressed(Input::Key::PitchDown))
			{
				float currentPitch = AudioManager::GetCurrent()->GetChannelPitch(m_latestChannelIndex);
				SetClipPitch(currentPitch -= 0.10f);

				m_cmdUI->displayData.consoleError = "Pitch- : " + std::to_string(AudioManager::GetCurrent()->GetChannelPitch(m_latestChannelIndex));
			}

			if (InputManager::GetKeyPressed(Input::Key::PanLeft))
			{
				float newPan = AudioManager::GetCurrent()->GetChannelPan(m_latestChannelIndex) - 0.1f;
				SetClipPan(newPan);

				m_cmdUI->displayData.consoleError = "Pan Left : " + std::to_string(AudioManager::GetCurrent()->GetChannelPan(m_latestChannelIndex));
			}

			if (InputManager::GetKeyPressed(Input::Key::PanRight))
			{
				float newPan = AudioManager::GetCurrent()->GetChannelPan(m_latestChannelIndex) + 0.1f;
				SetClipPan(newPan);

				m_cmdUI->displayData.consoleError = "Pan Right : " + std::to_string(AudioManager::GetCurrent()->GetChannelPan(m_latestChannelIndex));
			}


			// Updates the audio playback 
			AudioManager::Update();


			
			

			// Setting the playback position
			m_cmdUI->displayData.currentPlaytime = AudioManager::GetCurrent()->GetPlaybackPosition(m_latestChannelIndex);


			// Checking if the song that was playing has ended
			if (m_cmdUI->displayData.currentPlaytime >= m_cmdUI->displayData.maxPlaytime)
			{
				m_cmdUI->displayData.consoleError = "No Audio Active";
				m_cmdUI->displayData.isActive = false;

			}
			


		}

		void MediaPlayer::Quit()
		{
			// making sure we unload all the sounds from the FMOD API before exiting teh application
			UnloadAllSounds();

			// Clearing out the data in the Clip list
			m_clipList.clear();

			// Clearing teh console screen before we end
			m_cmdUI->ClearScreen();

			// Shutting down the audio manager
			AudioManager::Shutdown();

			m_isRunning = false;
		}

		void MediaPlayer::LoadClip(const std::string& filePath, bool isStreamed)
		{	
			// Craeting a new Audio Clip with the Following Data;
			AudioClip newClip(filePath , 1.0);

			bool clipLoadedSuccessfully;

			clipLoadedSuccessfully = AudioManager::GetCurrent()->LoadSound(newClip , isStreamed ? AudioManager::LoadType::STREAMED : AudioManager::LoadType::SAMPLED );
			
			// Loading the clip if its really unique
			

			if (!clipLoadedSuccessfully)
			{
				// Output Error to UI
				return;
			}

			
			m_cmdUI->displayData.consoleError = newClip.GetID() + " : Clip Loaded SuccessFully";

			// The given clip was unique, so loading it
			m_clipList.emplace_back(newClip);

			m_cmdUI->displayData.songnameList.emplace_back(newClip.GetID());


		}

		


		

		void MediaPlayer::PlayNewClip(const int index)
		{
	
			if(index >= m_clipList.size() || index < 0)
			{ 
				// Error Clip not found
				return;
			}


			m_playingClipIndex = index;


			m_latestChannelIndex = AudioManager::GetCurrent()->PlayNewSound(m_clipList[m_playingClipIndex]);
	

 			m_cmdUI->displayData.maxPlaytime = AudioManager::GetCurrent()->GetSoundCliplength();
			m_cmdUI->displayData.isActive = true;
			m_cmdUI->displayData.isPlaying = true;
			m_cmdUI->displayData.audioTitle = m_clipList[m_playingClipIndex].GetID();

			m_cmdUI->displayData.consoleError = " Currently Playing : " + m_clipList[m_playingClipIndex].GetID();
			
		}



		void MediaPlayer::SetClipVolume(const float newVolume)
		{
			float volume = newVolume >= 10.0f ? 10.0f : newVolume;
		    volume = newVolume <= 0.0f ? 0.0f : newVolume;

			AudioManager::GetCurrent()->SetChannelVolume(volume, m_latestChannelIndex);

		}

		void MediaPlayer::PlayPause()
		{
		
			if (m_cmdUI->displayData.isActive)
			{
				AudioManager::GetCurrent()->PlayPauseToggle(m_latestChannelIndex);
				m_cmdUI->displayData.isPlaying = !AudioManager::GetCurrent()->GetChannelPaused(m_latestChannelIndex);


				m_cmdUI->displayData.consoleError = " Currently Playing : " + m_clipList[m_playingClipIndex].GetID();

				return;
			}
			
		}

		
		void MediaPlayer::SetClipPan(const float newPan)
		{
			AudioManager::GetCurrent()->SetChannelPan (newPan, m_latestChannelIndex);
		}

		void MediaPlayer::SetClipPitch(const float newPitch)
		{
			AudioManager::GetCurrent()->SetChannelPitch(newPitch, m_latestChannelIndex);
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


		
	}
}
