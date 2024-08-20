#pragma once
#include <vector>

class Level;
class Player;
class Camera;
class SpriteManager;
class TextureManager;
class AudioManager;

class LevelManager final
{
public:
	LevelManager(const float scale, TextureManager* textureManager, AudioManager* audioManager);

	~LevelManager();

#pragma region RuleOf5
	LevelManager(const LevelManager& other) = delete;
	
	LevelManager& operator=(const LevelManager& other) = delete;
	
	LevelManager(LevelManager&& other) = delete;
	
	LevelManager& operator=(LevelManager&& other) = delete;
#pragma endregion

	void Update(float elapsedSec, Player* player, Camera* camera, float viewportWidth);

	void Draw(const Camera* camera, const float viewportWidth, const float viewportHeight, const float playerHealth) const;

	const Level* GetCurrentLevel() const;
	
	bool IsChanginLevel() const;
	
	bool IsDeathAnimationDone() const;
	
	float GetTransitionTimer() const;

private:
	TextureManager* m_pTextureManager;
	
	AudioManager* m_pAudioManager;

	std::vector<Level*> m_pLevels;

	int m_CurrentLevel{ 0 },
		m_PreviusLevel{ 0 };
	
	bool m_ChanginLevel{ false },
		m_DeathAnimationDone{ false };

	float m_TransitionTime{ 0 };

	void HasPlayerReachedEndLevel(Player* player);
	
	void ChangeLevel(bool rightSpawnLocation, Player* player, Camera* camera);
};

