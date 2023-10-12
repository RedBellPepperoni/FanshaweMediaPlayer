#include "Player/MediaPlayer.h"
#include <stdlib.h>
#include <memory>
#include <filesystem>
#include <iostream>

std::vector<std::filesystem::path> validExt = { ".wav",".mp3",".ogg",".flac" };
std::string mediaPath = "Media\\";

void LoadAllFiles(std::vector<std::string>& mediafiles)
{
	
	for (const auto& entry : std::filesystem::directory_iterator(mediaPath))
	{
		std::string extention = entry.path().extension().string();

		for (int i = 0; i < validExt.size(); i++)
		{
			if (extention == validExt[i])
			{
				mediafiles.emplace_back(mediaPath +  entry.path().filename().string());
				
			}
		}
	}

	

}



int main(int argc, char* argv)
{
	// initializing the Runtime Media Player 
	std::unique_ptr<FanshaweGameEngine::Runtime::MediaPlayer> mediaPlayer = std::make_unique<FanshaweGameEngine::Runtime::MediaPlayer>();


	std::vector<std::string> audioFileList;

	LoadAllFiles(audioFileList);

	// initializing the Player
	mediaPlayer->Init();



	/// Manual File Loading : May or maynot find the file

	// loading a streamed clip 
	mediaPlayer->LoadClip("Media\\EnchantedFestival.mp3", true);

	// loading a sampled clip
	mediaPlayer->LoadClip("Media\\jaguar.wav", false);



	/// You can automatically load files : Better system
	/// This feteches all the compatible files and then loads them
	/// When loading 2 files with the same name, the latest one is ignored

	for (int i = 0; i < audioFileList.size(); i++)
	{	
		std::cout << "Loading : " << audioFileList[i] << std::endl;
		mediaPlayer->LoadClip(audioFileList[i]);
	}


	
	//// all others are sampled
	//mediaPlayer->LoadClip("Media\\singing.wav");
	//mediaPlayer->LoadClip("Media\\stereo.ogg");
	//mediaPlayer->LoadClip("Media\\8bit_jingle.wav");
	//mediaPlayer->LoadClip("Media\\TheLastEncounter.mp3");
	//mediaPlayer->LoadClip("Media\\8bit_Mercury.wav");
	//mediaPlayer->LoadClip("Media\\Mars.wav");
	//mediaPlayer->LoadClip("Media\\Map.wav");
	//mediaPlayer->LoadClip("Media\\Venus.wav");


	

	
	// Game loop
	while (mediaPlayer->GetRunning())
	{
		mediaPlayer->Update();
		
	}

}

