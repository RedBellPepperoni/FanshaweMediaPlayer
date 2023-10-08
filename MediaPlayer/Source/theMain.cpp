#include "Player/MediaPlayer.h"
#include <stdlib.h>







int main(int argc, char* argv)
{
	// initializing the Runtime Media Player 
	FanshaweGameEngine::Runtime::MediaPlayer mediaPlayer;

	// initializing the Player
	mediaPlayer.Init();


	mediaPlayer.LoadClip("Media\\jaguar.wav");
	mediaPlayer.LoadClip("D:\\Fanshawe\\Projects\\Classes\\Term-1\\September-October\\MediaFundamentals\\MediaPlayer\\Media\\singing.wav");

	//mediaPlayer.SetClipLooping(1,true);
	mediaPlayer.PlayClip(8);


	mediaPlayer.SetClipLooping(9, true);
	mediaPlayer.PlayClip(8);




	// Game loop
	while (mediaPlayer.GetRunning())
	{
		mediaPlayer.Update();
		
	}

}