#include "Player/MediaPlayer.h"
#include <stdlib.h>
#include <memory>





int main(int argc, char* argv)
{
	// initializing the Runtime Media Player 
	std::unique_ptr<FanshaweGameEngine::Runtime::MediaPlayer> mediaPlayer = std::make_unique<FanshaweGameEngine::Runtime::MediaPlayer>();

	

	// initializing the Player
	mediaPlayer->Init();

	// loading a streamed clip
	mediaPlayer->LoadClip("Media\\EnchantedFestival.mp3",true);

	// loading a sampled clip
	mediaPlayer->LoadClip("Media\\jaguar.wav", false);

	// all others are sampled
	mediaPlayer->LoadClip("Media\\singing.wav");
	mediaPlayer->LoadClip("Media\\stereo.ogg");
	mediaPlayer->LoadClip("Media\\8bit_jingle.wav");
	mediaPlayer->LoadClip("Media\\TheLastEncounter.mp3");
	mediaPlayer->LoadClip("Media\\8bit_Mercury.wav");
	mediaPlayer->LoadClip("Media\\Mars.wav");
	mediaPlayer->LoadClip("Media\\Map.wav");
	mediaPlayer->LoadClip("Media\\Venus.wav");




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

