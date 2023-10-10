#include "Player/MediaPlayer.h"
#include <stdlib.h>
#include <memory>





int main(int argc, char* argv)
{
	// initializing the Runtime Media Player 
	std::unique_ptr<FanshaweGameEngine::Runtime::MediaPlayer> mediaPlayer = std::make_unique<FanshaweGameEngine::Runtime::MediaPlayer>();

	

	// initializing the Player
	mediaPlayer->Init();


	mediaPlayer->LoadClip("Media\\jaguar.wav");
	mediaPlayer->LoadClip("Media\\singing.wav");
	mediaPlayer->LoadClip("Media\\drumloop.wav");
	mediaPlayer->LoadClip("Media\\swish.wav");

	//mediaPlayer.SetClipLooping(1,true);
	//mediaPlayer.PlayClip(8);


	//mediaPlayer.SetClipLooping(9, true);
	//mediaPlayer.PlayClip(8);


	
	// Game loop
	while (mediaPlayer->GetRunning())
	{
		mediaPlayer->Update();
		
	}

}

