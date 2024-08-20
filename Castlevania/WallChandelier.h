#pragma once
#include "GameObject.h"

class Texture;
class TextureManager;

class WallChandelier final : public GameObject
{
public:
	WallChandelier(TextureManager* textureManager, const Point2f& center);
	
	~WallChandelier() = default;

#pragma region RuleOf5
	WallChandelier(const WallChandelier&) = delete;
	
	WallChandelier& operator=(const WallChandelier&) = delete;
	
	WallChandelier(WallChandelier&&) = delete;
	
	WallChandelier& operator=(WallChandelier&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) override;

	virtual void Draw() const override;
	
	virtual void AttackObject(const Rectf& attackShape, int damage) override;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) override;

	virtual bool DoesObjectDropItem() override;
	
	virtual GameObject* GetDroppedItem() const override;

	const Rectf& GetShape() const;

private:
#pragma region Variables
	int m_ChangeLoop{ 1 };
	
	float m_TimerFlickering{ 0 };
	
	bool m_IsBacklightVisible{ true };
#pragma endregion
};

