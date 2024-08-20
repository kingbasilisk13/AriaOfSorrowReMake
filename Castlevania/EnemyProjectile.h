#pragma once
#include "GameObject.h"

class EnemyProjectile final : public GameObject
{
public:
	EnemyProjectile(TextureManager* textureManager, const Rectf& projectileShape, bool thrownToTheLeft);
	
	~EnemyProjectile() = default;

#pragma region RuleOf5
	EnemyProjectile(const EnemyProjectile&) = delete;
	
	EnemyProjectile& operator=(const EnemyProjectile&) = delete;
	
	EnemyProjectile(EnemyProjectile&&) = delete;
	
	EnemyProjectile& operator=(EnemyProjectile&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;
	
	virtual void Draw() const override;

	virtual void AttackObject(const Rectf& attackShape, int damage) override;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

private:
#pragma region Variables
	
	float m_TimeAliveProjectile{ 0 };

	const bool m_ThrownToTheLeft;

	const float m_ProjectileYOrigin;

	void AttackPlayer(Player* player);
};

