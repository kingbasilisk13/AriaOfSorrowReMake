#pragma once
#include "Enemy.h"

class Texture;
class Player;
class TextureManager;

class Zombie final: public Enemy
{
public:
	enum class ActionState
	{
		spawning = 27,
		walkingHeadDown = 65,
		walkingHeadUp = 106,
		dying = 188,
		deSpawning = 0
	};

	Zombie(const Rectf& spawnArea, TextureManager* textureManager, const Rectf& playerLocation, const Point2f& cameraBottemLeft, float windowWidth);
	
	~Zombie() = default;

#pragma region rule of 5
	Zombie(const Zombie&) = delete;
	
	Zombie& operator=(const Zombie&) = delete;
	
	Zombie(Zombie&&) = delete;
	
	Zombie& operator=(Zombie&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;
	
	virtual void Draw() const override;

	virtual void AttackObject(const Rectf& attackShape, int damage) override;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

private:
#pragma region Variables
	Rectf m_SpawnArea;

	ActionState m_ActionState{ ActionState::spawning };
	
	bool m_WalkingLeft,
		m_StartWalking{ false };

	float m_ParticlesAnimTime{ 0 };
	
	int m_ParticlesAnimFrame{ 0 };

	const float m_Horizontal{ 20.f };
#pragma endregion

#pragma region HelperFunctions
	void AttackPlayer(Player* player);

	void HandleMovement(float elapsedSec);

	void ChangeActionState();

	void ChangeAnimationFrame(float elapsedSec);

	void ChangeClipValues();
#pragma endregion	
};

