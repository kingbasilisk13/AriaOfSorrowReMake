#include "pch.h"
#include "AudioManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"


AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	for( const std::pair<std::string, SoundEffect*>& soundEffects : m_SoundEffectMap )
	{
		delete soundEffects.second;
	}

	for( const std::pair<std::string, SoundStream*>& soundStreams : m_SoundStreamMap )
	{
		delete soundStreams.second;
	}
}

void AudioManager::PlaySoundEffect(std::string filePath, bool loop)
{
	SoundEffect* result;

	std::map<std::string, SoundEffect*>::iterator location{ m_SoundEffectMap.find( filePath ) };

	if( location != m_SoundEffectMap.end() )
	{
		result = location->second;
	}
	else
	{
		result = new SoundEffect( filePath );
		m_SoundEffectMap[filePath] = result;
	}

	if( m_SoundEffectMap[filePath]->IsLoaded() )
	{
		m_SoundEffectMap[filePath]->SetVolume( m_SoundEffectVolume );
		if( loop )
		{
			m_SoundEffectMap[filePath]->Play( -1 );
		}
		else
		{
			m_SoundEffectMap[filePath]->Play( 0 );
		}
	}
}

void AudioManager::PlaySoundStream(std::string filePath, bool loop)
{
	SoundStream* result;

	std::map<std::string, SoundStream*>::iterator location{ m_SoundStreamMap.find(filePath) };

	if( location != m_SoundStreamMap.end() )
	{
		result = location->second;
	}
	else
	{
		result = new SoundStream( filePath );
		m_SoundStreamMap[filePath] = result;
	}

	SoundStream::SetVolume( m_SoundStreamVolume );
	if( ( m_SoundStreamMap[filePath]->IsLoaded() ) && ( m_CurrentlyPlayingSoundStream != filePath ) )
	{
		m_CurrentlyPlayingSoundStream = filePath;
		m_SoundStreamMap[filePath]->Play( loop );
	}
}

void AudioManager::PauseSoundStream() const
{
	SoundStream::Pause();
}

void AudioManager::ResumeSoundStream() const
{
	SoundStream::Resume();
}

void AudioManager::DecreaseSoundStreamVolume()
{
	m_SoundStreamVolume -= 1;

	if( m_SoundStreamVolume < 0 )
	{
		m_SoundStreamVolume = 0;
	}
	SoundStream::SetVolume( m_SoundStreamVolume );

}

void AudioManager::IncreaseSoundStreamVolume()
{
	m_SoundStreamVolume += 1;

	if( m_SoundStreamVolume > m_MaximumMusicVolume )
	{
		m_SoundStreamVolume = m_MaximumMusicVolume;
	}

	SoundStream::SetVolume(m_SoundStreamVolume);
}

void AudioManager::SetSoundStreamVolume(int volume)
{
	m_SoundStreamVolume = volume;
}

int AudioManager::GetSoundStreamVolume() const
{
	return m_SoundStreamVolume;
}

void AudioManager::DecreaseSoundEffectVolume()
{
	m_SoundEffectVolume -= 1;

	if( m_SoundEffectVolume < 0 )
	{
		m_SoundEffectVolume = 0;
	}

	PlaySoundEffect( testSound, false );
}

void AudioManager::IncreaseSoundEffectVolume()
{
	m_SoundEffectVolume += 1;

	if( m_SoundEffectVolume > m_MaximumSoundeffectVolume )
	{
		m_SoundEffectVolume = m_MaximumSoundeffectVolume;
	}

	PlaySoundEffect( testSound, false );
}

void AudioManager::SetSoundEffectVolume(int volume)
{
	m_SoundEffectVolume = volume;
}

int AudioManager::GetSoundEffectVolume() const
{
	return m_SoundEffectVolume;
}

int AudioManager::GetMaxMusicVolume() const
{
	return m_MaximumMusicVolume;
}

int AudioManager::GetMaxSoundeffectVolume() const
{
	return m_MaximumSoundeffectVolume;
}