#pragma once
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>


namespace FanshaweGameEngine
{
	namespace Audio
	{
		//AudioMananger;
	}

	namespace UI
	{

		// An Experimental class to test out UI in cmd promt using windows console api

		class CommandLineInterface
		{

			enum textAlign
			{
				Left,
				Right,
				Center
			};

			enum layout
			{
				Console,
				Player,
				Selector
			};


			

		public:

			struct DisplayData
			{
				std::string audioTitle;

				float chVolume;
				unsigned int maxPlaytime;
				unsigned int currentPlaytime;
				float pan;

				bool isActive;
				bool isPlaying;

				std::string consoleError;

				std::vector<std::string> songnameList;

			};



			CommandLineInterface();
			~CommandLineInterface() = default;

			void Init();


			void ShowSplashScreen();
			void ShowMediaPlayer();
			
			void DrawFrame(bool ShowSplash);

			void SwapBuffers();

			COORD GetScreenSize();

		private:

			void OutputTextLine(layout Layout , int lineNumber,const std::string text ,int attribute = 30,textAlign align = textAlign::Center );
			void OutputCharLine(layout Layout , int lineNumber, const char Char, int attribute = 30);

			
			

			void AddBorder(int lineStartIndex);
			void AddMidLine(int lineindex, int lineendIndex);
			void ClearFrameBuffer();

			void LayoutTitle();
			void LayoutErrorConsole();

			void LayoutPLayer();
			void LayoutAudioSelection();

			void UpdateAudioSeek(int lineNumber);
			

			const char border = '|';
			const char space = ' ';
			

			COORD m_screenSize;
			COORD m_TopLeft;

			SMALL_RECT m_srctWriteRect;

			const unsigned int m_fps = 60;
			float deltaTime;

			// Get the intial console buffer.
			HANDLE m_firstBuffer = nullptr;

			// Secondary Buffer to simulate Dual Buffer Rendering
			HANDLE m_secondBuffer = nullptr;

			HANDLE m_backBuffer = nullptr;


			bool bufferSwitch = true;

			std::vector<CHAR_INFO> m_frameData;

			short m_cursor = 0;

		public:
			DisplayData displayData;
		};
	}
}

