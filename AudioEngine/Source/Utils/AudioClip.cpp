#include "AudioClip.h"

// C++ 17 filesystem for easier hadling of file paths, names and extentions
#include <filesystem>


namespace FanshaweGameEngine
{
	namespace Audio
	{

		AudioClip::AudioClip()
			: m_filePath("")	
			, m_shouldLoop(false)
			, m_hasLoaded(false)
		{
			m_uniqueId = "";
		}

		AudioClip::AudioClip(const std::string& filePath, float volume, bool shouldLoop)
			: m_filePath(filePath.c_str())
			, m_shouldLoop(shouldLoop)
			, m_hasLoaded(false)
		{
			// Storing the Filename (with ext) not including the entire path as our uniqueId
			m_uniqueId = std::filesystem::u8path(m_filePath).filename().string();
		}
	

		AudioClip::~AudioClip()
		{
		}

		const std::string AudioClip::GetID() const
		{
			return m_uniqueId;
		}

		const char* AudioClip::GetPath() const
		{
			return m_filePath;
		}

		const bool AudioClip::GetLoaded() const
		{
			return m_hasLoaded;
		}

		const bool AudioClip::GetLooping() const
		{
			return m_shouldLoop;
		}


		void AudioClip::SetLoaded(const bool hasloaded)
		{
			m_hasLoaded = hasloaded;
		}

		void AudioClip::SetLooping(const bool shouldLoop)
		{
			m_shouldLoop = shouldLoop;
		}

	}
}
