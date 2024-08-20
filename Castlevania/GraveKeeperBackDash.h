#pragma once
#include "GameObject.h"

class Texture;
class Player;
class TextureManager;

class GraveKeeperBackDash final : public GameObject
{
public:
	GraveKeeperBackDash(TextureManager* textureManager, const Point2f& bottemLeft);
	
	~GraveKeeperBackDash() = default;

#pragma region rule of 5
	GraveKeeperBackDash(const GraveKeeperBackDash&) = delete;
	
	GraveKeeperBackDash& operator=(const GraveKeeperBackDash&) = delete;
	
	GraveKeeperBackDash(GraveKeeperBackDash&&) = delete;
	
	GraveKeeperBackDash& operator=(GraveKeeperBackDash&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;
	
	virtual void Draw() const override;

	virtual void AttackObject(const Rectf& attackShape, int damage) override;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

private:
	bool m_HasBeenHit{ false };

	void ChangeAnimationFrame(float elapsedSec);
};

