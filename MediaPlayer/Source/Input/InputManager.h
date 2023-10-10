#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace FanshaweGameEngine
{
	namespace Input
	{
		enum Key
		{
			PlayPause,	//  Space Bar : toggleable
			Quit,		// Escape

			LoadClip,	// Tab
			NextClip,	// Down Arrow
			PrevClip,	// Up Arrow
			RemoveClip,	// Delete

			VolumeUp,	// numpad Plus 
			VolumeDown,	// numpad minus

			SelectClip,  // Enter
			PitchUp,
			PitchDown


		};

		

		class InputManager
		{

		private:

			// The keys that are pressed 
			static unsigned int m_pressedKeys;

			// The keys that are held down, really useful to detect and filer only one key press
			static unsigned int m_heldKeys;

			// The major Key Map we need for this Application
			static unsigned int GetKeyState()
			{
				unsigned int buttonsData = 0;

				if (GetAsyncKeyState(VK_SPACE) & 0x8000) buttonsData |= (1 << Key::PlayPause);
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) buttonsData |= (1 << Key::Quit);
				if (GetAsyncKeyState(VK_TAB) & 0x8000) buttonsData |= (1 << Key::LoadClip);
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) buttonsData |= (1 << Key::NextClip);
				if (GetAsyncKeyState(VK_UP) & 0x8000) buttonsData |= (1 << Key::PrevClip);
				if (GetAsyncKeyState(VK_DELETE) & 0x8000) buttonsData |= (1 << Key::RemoveClip);
				if (GetAsyncKeyState(VK_ADD) & 0x8000) buttonsData |= (1 << Key::VolumeUp);
				if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) buttonsData |= (1 << Key::VolumeDown);
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) buttonsData |= (1 << Key::SelectClip);
				if (GetAsyncKeyState(VK_PRIOR) & 0x8000) buttonsData |= (1 << Key::PitchUp);
				if (GetAsyncKeyState(VK_NEXT) & 0x8000) buttonsData |= (1 << Key::PitchDown);

				return buttonsData;

			}

		public:

			static const std::string GetKeyName(Key key)
			{
				switch (key)
				{
				case FanshaweGameEngine::Input::PlayPause:	return "Spacebar";
					break;
				case FanshaweGameEngine::Input::Quit:	return "Escape";
					break;
				case FanshaweGameEngine::Input::LoadClip: return "Tab";
					break;
				case FanshaweGameEngine::Input::NextClip:	return "Down Arrow";
					break;
				case FanshaweGameEngine::Input::PrevClip: return "Up Arrow";
					break;
				case FanshaweGameEngine::Input::RemoveClip: return "Delete";
					break;
				case FanshaweGameEngine::Input::VolumeUp: return "Keypad +";
					break;
				case FanshaweGameEngine::Input::VolumeDown: return "Keypad -";
					break;
				case FanshaweGameEngine::Input::SelectClip: return "Enter";
					break;
				case FanshaweGameEngine::Input::PitchUp: return "PageUp";
					break;
				case FanshaweGameEngine::Input::PitchDown: return "PageDown";
					break;

				default: return "";
					break;


				}
			}


			static const bool GetKeyPressed(Key key)
			{
				return ((m_pressedKeys & (1 << key))!= 0);
			}


			static void ProcessInput()
			{
				// Storing the data of all the pressed keys this frame

				unsigned int updatedKeyStates = GetKeyState();

				// XORing the held keys, this helps in geeting the key event only once when pressed. the key wont update until its released
				m_pressedKeys = (m_heldKeys ^ updatedKeyStates) & updatedKeyStates;

				// Storing the all the keys which are currently held down
				m_heldKeys = updatedKeyStates;

			}



		};

		unsigned int InputManager::m_pressedKeys = 0;
		unsigned int InputManager::m_heldKeys = 0;
		


		
		
	}
}


