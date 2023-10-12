# Fanshawe Media Player <br>
 Term-1 : Assignment to create an Audio Manager using FMOD API and a Media player to control the Audio manager <br>

 > The Goal of this Project was to create a media player using the Fmod API
 > I have added my own twist of creating a UI using a double buffered terminal display using windows console.

   
### Installation notes:<br>


> Step 1: Pull the Repository<br>
>
> Step 2: Open the MediaFanshawr.sln and compile both the Projects [AudioEngine](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/AudioEngine/AudioEngine.vcxproj "AudioEngine") and [MediaPLayer](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/MediaPlayer/MediaPlayer.vcxproj "MediaPlayer")<br>
>
> Step 3: Add 'fmod.dll' from the [Externals](https://github.com/RedBellPepperoni/FanshaweMediaPlayer/tree/main/External/fmod/core/lib "Fmod Library") folder to the folder containing the exe
>
> Step 4: Create a 'Media' folder and add audio files to it (recognised filetypes : .wav, .mp3, .ogg, .flac)
> 
> Step 5: Launch the MediaPlayer.exe , it should load all the audio files from teh Media folder automatically
<br>



### Usage:<br>

> Set [MediaPLayer](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/MediaPlayer/MediaPlayer.vcxproj "MediaPlayer") as the startup project and build+run to run Media Player Lib<br>
>
> The Disclaimer Should Pop up : Press 'Spacebar' to Continue to the Player<br>
>
> Use 'PageUp' and 'PageDown' to select the song that you want to play and Press 'Enter' to play the selected song.
>
> Use 'Up Arrow' and 'Down Arrow' to increase and decrease the Pitch of the audio respectively
>
> Use 'Left Arrow' and 'Right Arrow' to Pan the audio to the left and right
>
> Use 'Numpad +' and 'Numpad -' to increase or decrease the playback volume.

<br>


### Features<br>


> Audio Loading/Unloading using FMOD api
> 
> Audio Play/Pause
>
> Audio Clip selection
>
> Audio Panning (Left/Right)
>
> Audio Pitch Contol
>
> Audio Volume Control
> 

 The Splash screen made using the Double buffered console terminal 
<br>
<br>

 ![](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/mediaDisclaimer.gif)
<br>
<br>


 The Media playback panel with the seek slider showing the progress of the song
<br>
<br>

 ![](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/mediaPlayback.gif)
<br>
<br>

 The Media selector panel showing the song selection
<br>
<br>

 ![](https://github.com/RedBellPepperoni/MediaFundamentals/blob/main/mediaSelect.gif)
<br>
<br>
