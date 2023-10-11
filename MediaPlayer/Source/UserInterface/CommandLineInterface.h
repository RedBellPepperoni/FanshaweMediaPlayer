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

			// Helps to snap text in the UI
			enum textAlign
			{
				Left,
				Right,
				Center
			};


			// Defines the layout for the UI display
			enum layout
			{
				
				Console,  // Full screen usgae
				Player,   // Uses the first half ofthe screen (left side)
				Selector  // uses teh other half (right side)
			};


			

		public:

			struct DisplayData
			{
				std::string audioTitle;

				// The current channels's volume
				float chVolume;

				// the total playtime of the current audio 
				unsigned int maxPlaytime;

				// The current seek position of the playback
				unsigned int currentPlaytime;

				// current Pan value
				float pan;

				// some song is loaded and the channel is active (either playing or paused, but has an audio)
				bool isActive;

				// The channel is paused or playing
				bool isPlaying;

				// The error value to display to the Console
				std::string consoleError;

				// name list cahce for all the currently loaded songs
				std::vector<std::string> songnameList;

				int selectedSongIndex = 0;

			};



			CommandLineInterface();
			~CommandLineInterface() = default;

			void Init();


			void ShowSplashScreen();
			void ShowMediaPlayer();
			
			void DrawFrame(bool ShowSplash);

			void SwapBuffers();

			COORD GetScreenSize();

			void ClearScreen();

		private:

			void OutputTextLine(layout Layout , int lineNumber,const std::string text ,int textattrib = 30 ,int attribute = 30, textAlign align = textAlign::Center );
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

