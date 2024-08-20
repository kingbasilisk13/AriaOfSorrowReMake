#pragma once
#include "Enemy.h"

class BoneThrower final : public Enemy
{
public:

	enum class ActionState
	{
		idle = 0,
		throwing = 41 
	};

	BoneThrower(TextureManager* textureManager, const Point2f& spawnLocation);
	
	~BoneThrower() = default;

#pragma region rule of 5
	BoneThrower(const BoneThrower&) = delete;
	
	BoneThrower& operator=(const BoneThrower&) = delete;
	
	BoneThrower(BoneThrower&&) = delete;
	
	BoneThrower& operator=(BoneThrower&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;
	
	virtual void Draw() const override;

	virtual void AttackObject(const Rectf& attackShape, int damage) override;

	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

private:
#pragma region Variables
	ActionState m_ActionState{ ActionState::idle };

	float m_TimeUntilNextAttack{ 0 };
		
	bool m_LookingLeft{ false },
		m_ProjectileAlreadyThrown{ false };

#pragma endregion

#pragma region HelperFunctions
	void ChangeAnimationFrame(float elapsedSec);

	void ChangeClipValues();
#pragma endregion	

};

