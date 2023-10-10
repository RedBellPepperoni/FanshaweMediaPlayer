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


			OutputTextLine(splashLayout, 4, "This is a Media Player made using a C++ Command line interface",3);

			OutputCharLine(splashLayout, 5, ' ',3);

			OutputTextLine(splashLayout, 6, "This also a small experiment to see if a cmd GUI is possible",3);

			OutputCharLine(splashLayout, 7, '.',3);
			OutputCharLine(splashLayout, 8, ' ');

			OutputTextLine(splashLayout, 9, "[ DISCLAIMER ]",  90);
			OutputCharLine(splashLayout, 10, '-',90);
			OutputTextLine(splashLayout, 11, " Please do not resize the console window, since that will break the UI",90);
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

			OutputTextLine(splashLayout, 27, "Press SpaceBar to Continue . . .", colorAtt );
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

			bool success = WriteConsoleOutput(m_backBuffer, m_frameData.data(), m_screenSize, m_TopLeft, &m_srctWriteRect);

			// Swap to the back buffer
			SetConsoleActiveScreenBuffer(m_backBuffer);

			// Swap Buffers
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


		void CommandLineInterface::OutputTextLine(layout Layout,int lineNumber, const std::string text,int attribute,textAlign align)
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
							m_frameData[i].Attributes = attribute;
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
							m_frameData[i].Attributes = 30;
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
							m_frameData[i].Attributes = attribute;
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
			OutputTextLine(titleLayout, 2, "[MEDIA PLAYER : FMOD API]", 3);
			OutputCharLine(titleLayout, 3, ' ', 3);
			OutputCharLine(titleLayout, 4, '=');


			


		}

		void CommandLineInterface::LayoutErrorConsole()
		{
			layout consoleLayout = layout::Console;

			OutputCharLine(consoleLayout,25, '_',3);
			OutputCharLine(consoleLayout, 26, ' ', 3);
			OutputTextLine(consoleLayout, 27, displayData.consoleError, 3);
			OutputCharLine(consoleLayout, 28, ' ', 3);
			OutputCharLine(consoleLayout, 29, '_',3);
		}






		void CommandLineInterface::LayoutPLayer()
		{
			
			const int controlattrib = 14;

			std::string audioClipName = displayData.audioTitle;

			layout playerLayout = layout::Player;
			//OutputCharLine(playerLayout, 5, ' ', controlattrib);
			OutputTextLine(playerLayout, 5, displayData.isActive ? "[ Now Playing ]" : "[Paused]", controlattrib, textAlign::Center);
			OutputCharLine(playerLayout, 6, '-', controlattrib);
			OutputCharLine(playerLayout, 7, ' ', controlattrib);
			OutputTextLine(playerLayout, 8, audioClipName, controlattrib);
			OutputCharLine(playerLayout, 9, '_', controlattrib);

			//

			OutputTextLine(playerLayout, 10, "|  *    .  *       .             *       |", controlattrib);
			OutputTextLine(playerLayout, 11, "|                         *              |", controlattrib);
			OutputTextLine(playerLayout, 12, "| *   .        *       .       .       * |", controlattrib);
			OutputTextLine(playerLayout, 13, "|   .     *                              |", controlattrib);
			OutputTextLine(playerLayout, 14, "|          .     .  *        *           |", controlattrib);
			
	
			OutputCharLine(playerLayout, 15, '-', controlattrib);
			OutputCharLine(playerLayout, 16, ' ', controlattrib);

			

			// The Audio - seek/ Slider
			const std::string seekLayout = "      [---------------------------------------]      ";

			// basic Layout
			OutputTextLine(playerLayout, 17, seekLayout, controlattrib, textAlign::Center);

			UpdateAudioSeek(17);

			

			OutputCharLine(playerLayout, 18, '_', controlattrib);
			OutputCharLine(playerLayout, 19, ' ', controlattrib);
			OutputTextLine(playerLayout, 20, "    /           |\\           \\         _|_       ___", controlattrib, textAlign::Left);
			OutputTextLine(playerLayout, 21, "    \\           |/           /          |           ", controlattrib, textAlign::Left);
			OutputCharLine(playerLayout, 22, ' ', controlattrib);




			OutputTextLine(playerLayout, 23, " PanLeft    Play\\Pause    PanRight     Vol+      Vol-", controlattrib, textAlign::Left);
			OutputTextLine(playerLayout, 24, "  (Left)     (Spacebar)     (Right)     (Up)     (Down)", controlattrib, textAlign::Left);
		}

		void CommandLineInterface::LayoutAudioSelection()
		{
			const int controlattrib = 12;

			std::string audioClipName = displayData.audioTitle;

			layout playerLayout = layout::Selector;
			//OutputCharLine(playerLayout, 5, ' ', controlattrib);
			OutputTextLine(playerLayout, 5, "[ Select Audio Clip ]", controlattrib, textAlign::Center);
			OutputCharLine(playerLayout, 6, '-', controlattrib);
			OutputCharLine(playerLayout, 7, ' ', controlattrib);
				
			

			std::string song1 = " ";
			std::string song2 = " ";
			std::string song3 = " ";
			std::string song4 = " ";
			std::string song5 = " ";
			std::string song6 = " ";
			std::string song7 = " ";
			std::string song8 = " ";

			song1 = displayData.songnameList.size() > 0 ? displayData.songnameList[0] : " ";
			song2 = displayData.songnameList.size() > 1 ? displayData.songnameList[1] : " ";
			song3 = displayData.songnameList.size() > 2 ? displayData.songnameList[2] : " ";
			song4 = displayData.songnameList.size() > 3 ? displayData.songnameList[3] : " ";
			song5 = displayData.songnameList.size() > 4 ? displayData.songnameList[4] : " ";
			song6 = displayData.songnameList.size() > 5 ? displayData.songnameList[5] : " ";
			song7 = displayData.songnameList.size() > 6 ? displayData.songnameList[6] : " ";
			song8 = displayData.songnameList.size() > 7 ? displayData.songnameList[7] : " ";

			//
			OutputTextLine(playerLayout, 8, song1, controlattrib);
			OutputTextLine(playerLayout, 9, song2, controlattrib);
			OutputTextLine(playerLayout, 10, song3, controlattrib);
			OutputTextLine(playerLayout, 11, song4, controlattrib);
			OutputTextLine(playerLayout, 12, song5, controlattrib);
			OutputTextLine(playerLayout, 13, song6, controlattrib);
			OutputTextLine(playerLayout, 14, song7, controlattrib);
			OutputTextLine(playerLayout, 15, song8, controlattrib);
			
			



			OutputCharLine(playerLayout, 16, ' ', controlattrib);
			OutputCharLine(playerLayout, 17, ' ', controlattrib);

			


			OutputCharLine(playerLayout, 18, '_', controlattrib);
			OutputCharLine(playerLayout, 19, ' ', controlattrib);
			OutputCharLine(playerLayout, 20, ' ', controlattrib);
			OutputCharLine(playerLayout, 21, ' ', controlattrib);
			OutputCharLine(playerLayout, 22, ' ', controlattrib);
			OutputCharLine(playerLayout, 23, ' ', controlattrib);
			OutputCharLine(playerLayout, 24, ' ', controlattrib);
			



			
		
		}

		void CommandLineInterface::UpdateAudioSeek(int lineNumber)
		{
			const int controlattrib = 14;
			layout playerLayout = layout::Player;

			
			

			const std::string seekLayout = "      [---------------------------------------]      ";

			// basic Layout
			//OutputTextLine(playerLayout, , seekLayout , controlattrib, textAlign::Center);
			
			int editIndex = (lineNumber * m_screenSize.X) + 11;

			// Divide by zero error solving
			if (displayData.maxPlaytime <= 0)
			{
				return;
			}


			/*int maxTimeSec = displayData.maxPlaytime / 100;
			int maxtimeMin = maxTimeSec / 60;
			int finalRemainderSec = maxTimeSec - (maxtimeMin * 60);*/



			float tempSeekCalc = (float)displayData.currentPlaytime* 40.0f / (float)displayData.maxPlaytime;

			int seekPosition = (int)tempSeekCalc;
			seekPosition = seekPosition % 40;
		

			m_frameData[editIndex + seekPosition].Char.UnicodeChar = '#';
			m_frameData[editIndex + seekPosition].Attributes = controlattrib;

			

			
		}

		


	}
}



