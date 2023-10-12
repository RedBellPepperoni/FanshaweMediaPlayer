#include "CommandLineInterface.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <math.h>



namespace FanshaweGameEngine
{
	namespace UI
	{
		
	
		float colorchange = 0;
		float colorchangetwo = 0;

		CommandLineInterface::CommandLineInterface()
		{
		}


		void CommandLineInterface::Init()
		{
			
			// Initial Console Buffer;
			m_firstBuffer = GetStdHandle(STD_OUTPUT_HANDLE);


			// Create an additional buffer for switching.
			m_secondBuffer = CreateConsoleScreenBuffer(
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ,
				nullptr,
				CONSOLE_TEXTMODE_BUFFER,
				nullptr);

			// Assign switchable back buffer.
			m_backBuffer = m_secondBuffer;

		    // Set the Screen Dimensions
			m_screenSize = GetScreenSize();
			
			// Assign the framebuffer size according to screen dimensions
			SetConsoleScreenBufferSize(m_firstBuffer, m_screenSize);
			SetConsoleScreenBufferSize(m_secondBuffer, m_screenSize);

			// allocate the buffer size as per the ttoal number od characters
			m_frameData.resize(m_screenSize.X * m_screenSize.Y);

			// getting the drawable area
			m_srctWriteRect.Left = m_srctWriteRect.Top = 0;
			m_srctWriteRect.Right = m_screenSize.X - 1;
			m_srctWriteRect.Bottom = m_screenSize.Y - 1;

			SetConsoleWindowInfo(m_firstBuffer, TRUE, &m_srctWriteRect);
			SetConsoleWindowInfo(m_secondBuffer, TRUE, &m_srctWriteRect);

			// Right now FPS is fixed to 60
			deltaTime = (1.0f / m_fps);
			
			
		}

		void CommandLineInterface::ShowSplashScreen()
		{
			layout splashLayout = layout::Console;

			

	
			OutputCharLine(splashLayout,0, '=');
			OutputCharLine(splashLayout, 1, ' ');
			OutputCharLine(splashLayout, 2, '.');
			OutputCharLine(splashLayout, 3, ' ',3);


			OutputTextLine(splashLayout, 4, "This is a Media Player made using a C++ Command line interface",3,3);

			OutputCharLine(splashLayout, 5, ' ',3);

			OutputTextLine(splashLayout, 6, "This also a small experiment to see if a cmd GUI is possible",3,3);

			OutputCharLine(splashLayout, 7, '.',3);
			OutputCharLine(splashLayout, 8, ' ');


			

		


			OutputTextLine(splashLayout, 9, "[ DISCLAIMER ]", 90, 90);
			OutputCharLine(splashLayout, 10, '-',90);
			OutputTextLine(splashLayout, 11, " Please do not resize the console window, since that will break the UI",90,90);
			OutputCharLine(splashLayout, 12, '-',90);
			OutputCharLine(splashLayout, 13, ' ',90);
			OutputCharLine(splashLayout, 14, ' ');

			OutputTextLine(splashLayout, 15, "  The Media Player uses the Audio manager to play sounds ");
			OutputTextLine(splashLayout, 16, " Audio manager uses FMOD API to play sounds from the disk");
			OutputCharLine(splashLayout, 17, ' ');
			OutputTextLine(splashLayout, 18, "[ Supported Methods ]");
			OutputCharLine(splashLayout, 19, ' ');
			OutputTextLine(splashLayout, 20, " <> Audio Loading/Unloading <>");
			OutputTextLine(splashLayout, 21, " <> Audio Play/Pause <>" );
			OutputTextLine(splashLayout, 22, " <> Clip Selection <>" );
			OutputTextLine(splashLayout, 23, " <> 2D Audio Panning <>" );
			OutputTextLine(splashLayout, 24, " <> Pitch Editing");
			OutputTextLine(splashLayout, 25, " <> Volume Selection <>");
			OutputCharLine(splashLayout, 26, ' ');


			colorchange = colorchange + deltaTime * 25.0f;

			int colorAtt = round(colorchange);
			colorAtt = colorAtt % 5;

			OutputTextLine(splashLayout, 27, "Press SpaceBar to Continue . . .", colorAtt,colorAtt );
			OutputCharLine(splashLayout, 28, ' ');
			OutputCharLine(splashLayout, 29, '=');
			



		}

		void CommandLineInterface::ShowMediaPlayer()
		{
			// Title
			LayoutTitle();
			
			LayoutPLayer();

			LayoutAudioSelection();

			AddMidLine(5, 27);

			// Error Console
			LayoutErrorConsole();
			
			
			

		}

		void CommandLineInterface::DrawFrame(bool ShowSplash)
		{
			
			ClearFrameBuffer();

			if (ShowSplash)
			{
				ShowSplashScreen();
			}

			else
			{
				ShowMediaPlayer();
			}

			

			SwapBuffers();

		}

		void CommandLineInterface::SwapBuffers()
		{	
			// Clear the Back Buffer
			//WriteConsole(m_backBuffer, &m_frameData.front(), static_cast<short>(m_frameData.size()), nullptr, nullptr);


			// Override the backbuffer with the frame data
			bool success = WriteConsoleOutput(m_backBuffer, m_frameData.data(), m_screenSize, m_TopLeft, &m_srctWriteRect);

			// Set the back buffer as the active buffer
			SetConsoleActiveScreenBuffer(m_backBuffer);


			// Swap Buffers pointer
			m_backBuffer = bufferSwitch ? m_firstBuffer : m_secondBuffer;

			// Toggle the bool so that the other buffer will be used next time
			bufferSwitch = !bufferSwitch;

			// Wait for a time relative to the fixed fps
			Sleep(1.0f / m_fps * 1000.0f);
		}

		COORD CommandLineInterface::GetScreenSize()
		{
			CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
			GetConsoleScreenBufferInfo(m_firstBuffer, &bufferInfo);
			const auto newScreenWidth = bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
			const auto newscreenHeight = bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1;

			return COORD{ static_cast<short>(newScreenWidth), static_cast<short>(newscreenHeight) };
		}


		void CommandLineInterface::OutputTextLine(layout Layout,int lineNumber, const std::string text, int textattrib, int attribute, textAlign align)
		{
			int remainingCharacters = 0;
			

			// Calculate the amount of characters left after reserving space for the border
			switch (Layout)
			{
			case FanshaweGameEngine::UI::CommandLineInterface::Console: remainingCharacters = m_screenSize.X - 4;
				break;
			case FanshaweGameEngine::UI::CommandLineInterface::Player: remainingCharacters = (m_screenSize.X - 4)/2;
				break;
			case FanshaweGameEngine::UI::CommandLineInterface::Selector: remainingCharacters = (m_screenSize.X -4)/2;
				break;
			default:  remainingCharacters = 0;
				break;
			}

		
			// thi is when the text can fit in the space
			if(text.size() < remainingCharacters)
			{
				// calculate the staring index
				int lineStartIndex = lineNumber * m_screenSize.X;

				// Drawing Left Border
				AddBorder(lineStartIndex);

				// If we are drawing for the selector we start from the center of the line
				if (Layout == FanshaweGameEngine::UI::CommandLineInterface::Selector)
				{
					lineStartIndex = lineStartIndex + m_screenSize.X / 2 - 2;	
				}

				int textItr = 0;
				int center = 0;


				switch (align)
				{

				case FanshaweGameEngine::UI::CommandLineInterface::Left:

					textItr = 0;

					for (int i = lineStartIndex + 2; i < (lineStartIndex + 2 + remainingCharacters); i++)
					{

						if (textItr < text.size())
						{
							m_frameData[i].Char.UnicodeChar = text[textItr];
							m_frameData[i].Attributes = textattrib;
							textItr++;
						}
						else
						{
							m_frameData[i].Char.UnicodeChar = ' ';
							m_frameData[i].Attributes = attribute;
						}
					}


					break;


				case FanshaweGameEngine::UI::CommandLineInterface::Right:

					 textItr = text.size() - 1;
					for (int i = lineStartIndex + remainingCharacters; i > lineStartIndex + 1 ; i--)
					{

						if (textItr >= 0)
						{
							m_frameData[i].Char.UnicodeChar = text[textItr];
							m_frameData[i].Attributes = textattrib;
							textItr--;
						}
						else
						{
							m_frameData[i].Char.UnicodeChar = ' ';
							m_frameData[i].Attributes = attribute;
						}
					}



					break;
				case FanshaweGameEngine::UI::CommandLineInterface::Center:

					textItr = 0;
					center = (lineStartIndex + 2) + floor((remainingCharacters - text.size())/2);
					for (int i = lineStartIndex + 2; i < lineStartIndex + 2 + remainingCharacters; i++)
					{
						

						if (textItr < text.size() && i >= center)
						{
							m_frameData[i].Char.UnicodeChar = text[textItr];
							m_frameData[i].Attributes = textattrib;
							textItr++;
						}
						else
						{
							m_frameData[i].Char.UnicodeChar = ' ';
							m_frameData[i].Attributes = attribute;
						}
					}



					break;
				default:
					break;
				}

				
			}


		}


		void CommandLineInterface::OutputCharLine(layout Layout, int lineNumber, const char Char, int attribute)
		{
			int remainingCharacters = 0;
			// Calculate the amount of characters left after reserving space for the border

			switch (Layout)
			{
			case FanshaweGameEngine::UI::CommandLineInterface::Console: remainingCharacters = m_screenSize.X - 4;
				break;
			case FanshaweGameEngine::UI::CommandLineInterface::Player: remainingCharacters = (m_screenSize.X / 2) - 2;
				break;
			case FanshaweGameEngine::UI::CommandLineInterface::Selector: remainingCharacters = (m_screenSize.X / 2) - 2;
				break;
			default:
				break;
			}




			
			if(lineNumber >= m_screenSize.Y)
			{
				return;
			}

			int lineStartIndex = lineNumber * m_screenSize.X;

			AddBorder(lineStartIndex);

			

			if (Layout == FanshaweGameEngine::UI::CommandLineInterface::Selector)
			{
				lineStartIndex = lineStartIndex + m_screenSize.X / 2 - 2;
				
			}




			for (int i = lineStartIndex + 2; i < (lineStartIndex + 2 + remainingCharacters); i++)
			{

				m_frameData[i].Char.UnicodeChar = Char;
				m_frameData[i].Attributes = attribute;
					
			}
		}

		std::string CommandLineInterface::GetTimeString(int miliseconds)
		{
			std::string finalTime;
			std::string textSec;
			std::string textMin;


			int timeSeconds = miliseconds / 1000;

			// the number of minutes
			int timeMin = timeSeconds / 60;

			// number of seconds
			int remainderSec = timeSeconds % 60;

			textSec = std::to_string(remainderSec);
			textMin = std::to_string(timeMin);

			if (remainderSec < 10)
			{
				textSec = '0' + textSec;
			}
			
			if (timeMin > 59)
			{
				textMin = "59";
			}
			if (timeMin < 10)
			{
				textMin = '0' + textMin;
			}

			finalTime = textMin + ':' + textSec;

			return finalTime;



		}

		void CommandLineInterface::AddBorder(int lineStartIndex)
		{

			m_frameData[lineStartIndex].Char.UnicodeChar = border;
			m_frameData[lineStartIndex].Attributes = 30;
			m_frameData[lineStartIndex + 1].Char.UnicodeChar = border;
			m_frameData[lineStartIndex + 1].Attributes = 30;
			/* m_frameData[lineStartIndex + 2].Char.UnicodeChar = space;
			m_frameData[lineStartIndex + 2].Attributes = 30;
			m_frameData[lineStartIndex + 3].Char.UnicodeChar = space;
			m_frameData[lineStartIndex + 3].Attributes = 30;*/

			m_frameData[lineStartIndex + m_screenSize.X - 1].Char.UnicodeChar = border;
			m_frameData[lineStartIndex + m_screenSize.X - 1].Attributes = 30;
			m_frameData[lineStartIndex + m_screenSize.X - 2].Char.UnicodeChar = border;
			m_frameData[lineStartIndex + m_screenSize.X - 2].Attributes = 30;
			/*m_frameData[lineStartIndex + m_screenSize.X - 3].Char.UnicodeChar = space;
			m_frameData[lineStartIndex + m_screenSize.X - 3].Attributes = 30;
			m_frameData[lineStartIndex + m_screenSize.X - 4].Char.UnicodeChar = space;
			m_frameData[lineStartIndex + m_screenSize.X - 4].Attributes = 30;*/
		}

		void CommandLineInterface::AddMidLine(int lineindex, int lineendIndex)
		{

			for (int i = lineindex;  i < lineendIndex; i++)
			{
				m_frameData[i*m_screenSize.X + m_screenSize.X/2 -1].Char.UnicodeChar = border;
				m_frameData[i*m_screenSize.X + m_screenSize.X/2 -1].Attributes = 30;
				m_frameData[i * m_screenSize.X + m_screenSize.X/ 2 ].Char.UnicodeChar = border;
				m_frameData[i * m_screenSize.X + m_screenSize.X / 2 ].Attributes = 30;
			}
		}

		

		

		void CommandLineInterface::ClearFrameBuffer()
		{
			for (int i = 0; i < m_frameData.size(); i++)
			{
				m_frameData[i].Char.UnicodeChar = ' ';
				m_frameData[i].Attributes = 30;
			}
		}

		void CommandLineInterface::LayoutTitle()
		{
		// Title

			layout titleLayout = layout::Console;
			

			OutputCharLine(titleLayout,0, '=');
			OutputCharLine(titleLayout, 1, ' ', 3);
			OutputTextLine(titleLayout, 2, "[MEDIA PLAYER : FMOD API]", 3,3);
			OutputCharLine(titleLayout, 3, ' ', 3);
			OutputCharLine(titleLayout, 4, '=');


			


		}

		void CommandLineInterface::LayoutErrorConsole()
		{
			layout consoleLayout = layout::Console;
			

			OutputCharLine(consoleLayout,25, '_');
			OutputCharLine(consoleLayout, 26, ' ', 3);
			OutputTextLine(consoleLayout, 27, displayData.consoleError, 3, 3);
			OutputCharLine(consoleLayout, 28, ' ', 3);
			OutputCharLine(consoleLayout, 29, '_');
		}






		void CommandLineInterface::LayoutPLayer()
		{
			
			// number 2 is green on black
			// 14 is yellow on black
			// 30 is white on blue

			//coloe for controls
			const int controlattrib = FOREGROUND_GREEN | FOREGROUND_RED ;
			// the background
			const int songnameAttrib = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY ;
			const int playerAttrib =  FOREGROUND_BLUE | FOREGROUND_GREEN |FOREGROUND_RED | FOREGROUND_INTENSITY| BACKGROUND_GREEN | BACKGROUND_BLUE;
			const int viewAttrib = 14;
			const int playerStateAttrib =  BACKGROUND_GREEN | BACKGROUND_RED ;
			const int seekAttrib = 14;


			std::string audioClipName =  displayData.audioTitle;

			layout playerLayout = layout::Player;
			//OutputCharLine(playerLayout, 5, ' ', controlattrib);
			OutputTextLine(playerLayout, 5, displayData.isPlaying ? "[   Now Playing   ]" : "[      Paused     ]", playerStateAttrib, playerStateAttrib,textAlign::Center);
			OutputCharLine(playerLayout, 6, '-', playerStateAttrib);
			OutputCharLine(playerLayout, 7, ' ', playerAttrib);
			OutputTextLine(playerLayout, 8, audioClipName, songnameAttrib, playerAttrib);
			OutputCharLine(playerLayout, 9, '_', playerAttrib);

			//

			OutputTextLine(playerLayout, 10, "|  *    .  *       .             *       |", viewAttrib, playerAttrib);
			OutputTextLine(playerLayout, 11, "|                         *              |", viewAttrib, playerAttrib);
			OutputTextLine(playerLayout, 12, "| *   .        *       .       .       * |", viewAttrib, playerAttrib);
			OutputTextLine(playerLayout, 13, "|   .     *                              |", viewAttrib, playerAttrib);
			OutputTextLine(playerLayout, 14, "|          .     .  *        *           |", viewAttrib, playerAttrib);
			
	
			OutputCharLine(playerLayout, 15, '-', playerAttrib);
			OutputCharLine(playerLayout, 16, ' ', playerAttrib);

			

			// The Audio - seek/ Slider
			const std::string seekLayout = "[---------------------------------------]";

			// basic Layout
			OutputTextLine(playerLayout, 17, seekLayout, seekAttrib, playerAttrib ,textAlign::Center);


			UpdateAudioSeek(17);

			

			OutputCharLine(playerLayout, 18, '_', playerAttrib);
			OutputCharLine(playerLayout, 19, ' ', controlattrib);


			OutputCharLine(playerLayout, 22, ' ', controlattrib);
			


			if (!displayData.isPlaying)
			{
				OutputTextLine(playerLayout, 20, "     |\\        /\\         \\  /        //         \\\\", controlattrib, controlattrib, textAlign::Left);
				OutputTextLine(playerLayout, 21, "     |/       /  \\         \\/         \\\\         //", controlattrib, controlattrib, textAlign::Left);
				OutputTextLine(playerLayout, 23, "    Play      Pitch+      Pitch-    PanLeft    PanRight", controlattrib, controlattrib, textAlign::Left);
			}
			else
			{
				OutputTextLine(playerLayout, 20, "     ||        /\\         \\  /        //         \\\\", controlattrib, controlattrib, textAlign::Left);
				OutputTextLine(playerLayout, 21, "     ||       /  \\         \\/         \\\\         //", controlattrib, controlattrib, textAlign::Left);
				OutputTextLine(playerLayout, 23, "    Pause     Pitch+      Pitch-    PanLeft    PanRight", controlattrib, controlattrib,textAlign::Left);
			}
			
			

			// Hard coding the values, but can be pulled from the input manager later
			OutputTextLine(playerLayout, 24, " (Spacebar)  (PageUp)  (PageDown)    (Left)    (Right)", controlattrib, controlattrib,textAlign::Left);
		}

		void CommandLineInterface::LayoutAudioSelection()
		{
			const int controlattrib = 12;
			const int selectAttrib = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_INTENSITY;
			const int playerStateAttrib = BACKGROUND_GREEN | BACKGROUND_RED;

			const int highlightAttrib = BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN| FOREGROUND_INTENSITY;


			std::string audioClipName = displayData.audioTitle;

			layout selectorLayout = layout::Selector;
			//OutputCharLine(playerLayout, 5, ' ', controlattrib);
			OutputTextLine(selectorLayout, 5, "[ Select Audio Clip ]", playerStateAttrib, playerStateAttrib, textAlign::Center);
			OutputCharLine(selectorLayout, 6, '-', playerStateAttrib);
			OutputCharLine(selectorLayout, 7, ' ', selectAttrib);
				
			

			std::vector<std::string> songs = { "0", "1","2","3","4","5","6","7","8" };

		

			int offsetedIndex = 0;
			int displayIndex = displayData.selectedSongIndex;

			int count = songs.size();

			if (displayData.selectedSongIndex >= count)
			{
				offsetedIndex = displayData.selectedSongIndex - (songs.size() - 1);
				displayIndex = 8;

				for (int i = 0; i < songs.size(); i++)
				{
				   songs[i] = displayData.songnameList[i + offsetedIndex];
				}
			}
			
			else
			{
				for (int i = 0; i < songs.size(); i++)
				{
					if (displayData.songnameList.size() > i)
					{
						songs[i] = displayData.songnameList[i];
					}
					else
					{
						songs[i] = " ";
					}

				}
			}

			
			

			

			//
			OutputTextLine(selectorLayout, 8, songs[0], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 9, songs[1], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 10, songs[2], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 11, songs[3], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 12, songs[4], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 13, songs[5], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 14, songs[6], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 15, songs[7], selectAttrib, selectAttrib);
			OutputTextLine(selectorLayout, 16, songs[8], selectAttrib, selectAttrib);
			

			
			int offsetSongLineIndex = displayData.selectedSongIndex + 8;

			if (offsetSongLineIndex > 16)
			{
				offsetSongLineIndex = 16;
			}
			else if (offsetSongLineIndex < 8)
			{
				offsetSongLineIndex = 8;
			}

		

			OutputTextLine(selectorLayout, offsetSongLineIndex, songs[displayIndex], highlightAttrib, highlightAttrib);

			
			OutputCharLine(selectorLayout, 17, ' ', selectAttrib);

			

			OutputCharLine(selectorLayout, 18, '_', selectAttrib);
			OutputCharLine(selectorLayout, 19, ' ', controlattrib);


			OutputCharLine(selectorLayout, 22, ' ', controlattrib);



			
			OutputTextLine(selectorLayout, 20, "      //\\\\       \\\\  //        _|_      ___      [=|=] ", controlattrib, controlattrib, textAlign::Left);
			OutputTextLine(selectorLayout, 21, "     //  \\\\       \\\\//          |                [=|=]", controlattrib, controlattrib, textAlign::Left);
			OutputTextLine(selectorLayout, 23, "   Next Song    Prev Song      Vol +    Vol -   PlaySong ", controlattrib, controlattrib, textAlign::Left);
			


			// Hard coding the values, but can be pulled from the input manager later
			OutputTextLine(selectorLayout, 24, "    (PageUp)    (PageDown)    (Num+)   (Num-)   (Enter)", controlattrib, controlattrib, textAlign::Left);
		



			
		
		}

		void CommandLineInterface::UpdateAudioSeek(int lineNumber)
		{
			const int controlattrib = 14;
			layout playerLayout = layout::Player;

			
			

			// basic Layout
			//OutputTextLine(playerLayout, , seekLayout , controlattrib, textAlign::Center);
			
			int editIndex = (lineNumber * m_screenSize.X);

			// Divide by zero error solving
			if (displayData.maxPlaytime <= 0)
			{
				return;
			}


			
			std::string timeSeek = GetTimeString(displayData.currentPlaytime);
	
			for (int i = 0; i < timeSeek.length(); i++)
			{
				m_frameData[editIndex + 3 + i].Char.UnicodeChar = timeSeek[i];
				m_frameData[editIndex + 3 + i].Attributes = controlattrib;
			}
			
			std::string timeMax = GetTimeString(displayData.maxPlaytime);
			
			for (int i = 0; i < timeMax.length(); i++)
			{
				m_frameData[editIndex + 53 + i].Char.UnicodeChar = timeMax[i];
				m_frameData[editIndex + 53 + i].Attributes = controlattrib;
			}
			
			float tempSeekCalc = (float)displayData.currentPlaytime * 40.0f / (float)displayData.maxPlaytime;
			int seekPosition = (int)tempSeekCalc;
			seekPosition = seekPosition % 40;
			
			m_frameData[editIndex + 11 + seekPosition].Char.UnicodeChar = '#';
			m_frameData[editIndex + 11+ seekPosition].Attributes = controlattrib;

			

			
		}

		// 
		// Taken from https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
		// Example 2 to clear the scrolling screen 
		void CommandLineInterface::ClearScreen()
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			SMALL_RECT scrollRect;
			COORD scrollTarget;
			CHAR_INFO fill;

			// Get the number of character cells in the current buffer.
			if (!GetConsoleScreenBufferInfo(m_firstBuffer, &csbi))
			{
				return;
			}

			// Scroll the rectangle of the entire buffer.
			scrollRect.Left = 0;
			scrollRect.Top = 0;
			scrollRect.Right = csbi.dwSize.X;
			scrollRect.Bottom = csbi.dwSize.Y;

			// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
			scrollTarget.X = 0;
			scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

			// Fill with empty spaces with the buffer's default text attribute.
			fill.Char.UnicodeChar = TEXT(' ');
			fill.Attributes = csbi.wAttributes;

			// Do the scroll
			ScrollConsoleScreenBuffer(m_firstBuffer, &scrollRect, NULL, scrollTarget, &fill);

			// Move the cursor to the top left corner too.
			csbi.dwCursorPosition.X = 0;
			csbi.dwCursorPosition.Y = 0;

			SetConsoleCursorPosition(m_firstBuffer, csbi.dwCursorPosition);
		}

		


	}
}



