#pragma once
class TextureManager;
class AudioManager;

class UIManager final
{
public:
	UIManager(TextureManager* textureManager, AudioManager* audioManager);

	~UIManager() = default;

#pragma region rule of 5
	UIManager(const UIManager&) = delete;

	UIManager& operator=(const UIManager&) = delete;

	UIManager(UIManager&&) = delete;

	UIManager& operator=(UIManager&&) = delete;
#pragma endregion

	void Update(float elapsedSec);

	void Draw(float width, float height) const;

	bool GetStartGame() const;

	void SetStartGame();


private:
	TextureManager* m_pTextureManager;

	AudioManager* m_pAudioManager;

	int m_NrOfFrames{ 5 },
		m_AnimFrame{ 0 };

	float m_NrFramesPerSec{ 2 },
		m_AnimTime{ 0 };

	Rectf m_AnimationSource,
		m_MusicSlider,
		m_SoundSlider;

	bool m_StartGame{ false },
		m_ShowTitleScreen{ true },
		m_MusicSelected{ true };

	void UpdateAnimation();

	void DrawTitleScreen(float width, float height) const;

	void DrawVolumeOptions(float width, float height) const;

	void ChangeVolume(const Uint8* pStates);
};