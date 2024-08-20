#pragma once
#include <utils.h>
#include "GameObject.h"

class Texture;
class Level;
class TextureManager;

class Gold final : public GameObject
{
public:
	enum class GoldCoinType
	{
		blue = 1,
		orange = 2,
		green = 3, 
		yellow = 4
	};

	Gold(TextureManager* textureManager, const Point2f& bottemLeft);
	
	~Gold() = default;

#pragma region RuleOf5
	Gold(const Gold&) = delete;
	
	Gold& operator=(const Gold&) = delete;
	
	Gold(Gold&&) = delete;
	
	Gold& operator=(Gold&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;
	
	virtual void Draw() const override;
	
	virtual void AttackObject(const Rectf& attackShape, int damage) override;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

private:
#pragma region Variables
	GoldCoinType m_GoldCoinType;
	
	Vector2f m_Velocity;

	int m_ChangeFrameValue{ 1 };

	const float m_ObjectLifeTime{ 5.f };
	
	float m_ObjectDespawnTime{ m_ObjectLifeTime * 0.1f },
		m_ObjectTimeLived{ 0 },
		m_ObjectTimeDespawning{ 0 };
	
	bool m_DrawObject{ true };
#pragma endregion

#pragma region HelperFunctions
	void ChangeAnimationFrame(float elapsedSec);

	void ChangeObjectLifeTime(float elapsedSec);

	void HandleMovement(float elapsedSec, const Level* level);

	void IsOverlapping(Player* player);

	int GetCoinValue() const;
#pragma endregion
};

