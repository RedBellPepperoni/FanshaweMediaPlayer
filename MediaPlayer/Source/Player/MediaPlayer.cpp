#include "MediaPlayer.h"
#include "Manager/AudioManager.h"
#include "Utils/AudioClip.h"
#include "Input/InputManager.h"
#include <iostream>



namespace FanshaweGameEngine
{
	using Audio::AudioManager;
	using Input::InputManager;
	

	namespace Runtime
	{
		

		MediaPlayer::MediaPlayer()
		{
		}
		MediaPlayer::~MediaPlayer()
		{
		}

		void MediaPlayer::Init()
		{
			// initializing the Audio manager singleton
			AudioManager::Init();

			m_isRunning = true;

			DrawCommandLineInterface();
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


			if (InputManager::GetKeyPressed(Input::Key::LoadClip))
			{
				//m_clipList[m_currentClipIndex].
			}



			AudioManager::Update();




		}

		void MediaPlayer::Quit()
		{
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

			// The given clip was unique, so loading it
			m_clipList.emplace_back(newClip);

			


		}
		void MediaPlayer::DeleteClip(const std::string& filePath)
		{
		}
		void MediaPlayer::PlayClip(const int index)
		{
			

			if(index >= m_clipList.size() || index < 0)
			{ 
				// Error Clip not found
				return;
			}

			m_currentClipIndex = index;

			AudioManager::GetCurrent()->PlaySound(m_clipList[m_currentClipIndex]);
		}



		void MediaPlayer::SetClipVolume(const float newVolume)
		{
		}

		void MediaPlayer::SetClipLooping(const int index, const bool shouldLoop)
		{
			if (index >= m_clipList.size() || index < 0)
			{
				// Error Clip not found
				return;
			}

			m_currentClipIndex = index;

			m_clipList[m_currentClipIndex].SetLooping(shouldLoop);
		}
		void MediaPlayer::SetClipPan(const float newPlayLocation)
		{
		}

		const bool MediaPlayer::GetRunning() const
		{
			return m_isRunning;
		}
		const std::string& MediaPlayer::GetClipName() const
		{
			return m_clipList[m_currentClipIndex].GetID();
		}


		void ClearScreen()
		{
			HANDLE                     hStdOut;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			DWORD                      count;
			DWORD                      cellCount;
			COORD                      homeCoords = { 0, 0 };

			hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hStdOut == INVALID_HANDLE_VALUE) return;

			/* Get the number of cells in the current buffer */
			if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
			cellCount = csbi.dwSize.X * csbi.dwSize.Y;

			/* Fill the entire buffer with spaces */
			if (!FillConsoleOutputCharacter(
				hStdOut,
				(TCHAR)' ',
				cellCount,
				homeCoords,
				&count
			)) return;

			/* Fill the entire buffer with the current colors and attributes */
			if (!FillConsoleOutputAttribute(
				hStdOut,
				csbi.wAttributes,
				cellCount,
				homeCoords,
				&count
			)) return;

			/* Move the cursor home */
			SetConsoleCursorPosition(hStdOut, homeCoords);
		}


		void MediaPlayer::DrawCommandLineInterface()
		{
			ClearScreen();




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
