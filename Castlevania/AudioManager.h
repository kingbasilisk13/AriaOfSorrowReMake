#pragma once
#include <iostream>
#include <string>
#include <map>

class SoundEffect;
class SoundStream;

class AudioManager final
{
public:
	AudioManager();
	
	~AudioManager();

#pragma region RuleOf5
	AudioManager(const AudioManager&) = delete;
	
	AudioManager& operator=(const AudioManager&) = delete;
	
	AudioManager(AudioManager&&) = delete;
	
	AudioManager& operator=(AudioManager&&) = delete;
#pragma endregion

#pragma region SoundEffects
	const std::string testSound		{ "Audio/SoundEffects/RandomSound1.mp3" };
	const std::string playerSwing	{ "Audio/SoundEffects/Swing.mp3" };
	const std::string playerThrow	{ "Audio/SoundEffects/Throw.mp3" };
	const std::string damageSound1	{ "Audio/SoundEffects/DamageSound1.mp3" };
	const std::string damageSound2	{ "Audio/SoundEffects/DamageSound2.mp3" };
	const std::string deathScream	{ "Audio/SoundEffects/DeathScream.mp3" };
	const std::string fallDown		{ "Audio/SoundEffects/FallDown.mp3" };
	const std::string jumpUp		{ "Audio/SoundEffects/JumpUp.mp3" };
	const std::string randomSound1	{ "Audio/SoundEffects/RandomSound1.mp3" };
	const std::string randomSound2	{ "Audio/SoundEffects/RandomSound2.mp3" };
	const std::string randomSound3	{ "Audio/SoundEffects/RandomSound3.mp3" };
	const std::string randomSound4	{ "Audio/SoundEffects/RandomSound4.mp3" };
	const std::string randomSound5	{ "Audio/SoundEffects/RandomSound5.mp3" };
#pragma endregion

#pragma region SoundStreams
	const std::string ruinedCastleCorridor	{ "Audio/Music/RuinedCastleCorridor.mp3" };
	const std::string gameOver				{ "Audio/Music/GameOver.mp3" };
	const std::string nameEntry				{ "Audio/Music/NameEntry.mp3" };
	const std::string titleTheme			{ "Audio/Music/TitleTheme.mp3" };

#pragma endregion
	void PlaySoundEffect(std::string filePath, bool loop=false);
	
	void PlaySoundStream(std::string filePath, bool loop = false);

	void PauseSoundStream() const;
	
	void ResumeSoundStream() const;
	
	void DecreaseSoundStreamVolume();
	
	void IncreaseSoundStreamVolume();
	
	void SetSoundStreamVolume(int volume);

	int GetSoundStreamVolume() const;

	void DecreaseSoundEffectVolume();
	
	void IncreaseSoundEffectVolume();
	
	void SetSoundEffectVolume(int volume);

	int GetSoundEffectVolume() const;

	int GetMaxMusicVolume() const;
	
	int GetMaxSoundeffectVolume() const;

private:
	std::map<std::string, SoundEffect*> m_SoundEffectMap{};
	
	std::map<std::string, SoundStream*> m_SoundStreamMap{};

	std::string m_CurrentlyPlayingSoundStream;

	const int m_MaximumMusicVolume{ 200 };
	
	const int m_MaximumSoundeffectVolume{ 800 };

	int m_SoundStreamVolume{ m_MaximumMusicVolume / 2 },
		m_SoundEffectVolume{ m_MaximumSoundeffectVolume / 2 };

};

