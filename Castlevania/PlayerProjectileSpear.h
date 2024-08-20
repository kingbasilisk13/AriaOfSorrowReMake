#pragma once
#include <utils.h>
#include <vector>

class TextureManager;

class PlayerProjectileSpear final
{
public:
	PlayerProjectileSpear(Point2f center, TextureManager* textureManager, bool moveLeft = false);

	~PlayerProjectileSpear() = default;

#pragma region RuleOf5
	PlayerProjectileSpear(const PlayerProjectileSpear&) = delete;
	
	PlayerProjectileSpear& operator=(const PlayerProjectileSpear&) = delete;
	
	PlayerProjectileSpear(PlayerProjectileSpear&&) = delete;
	
	PlayerProjectileSpear& operator=(PlayerProjectileSpear&&) = delete;
#pragma endregion
	
	void Update(float elapsedSec);

	void Draw() const;

	const std::vector<Point2f>& getTransVertices() const;
	
	int GetDamage() const;
	
	int GetManaConsumption() const;

	bool IsObjectDead() const;

	void StopMovement();

private:
	TextureManager* m_pTextureManager;

	Point2f m_SpearBack;

	const Point2f m_SpearBackOrigin;

	bool	m_IsMoving,
			m_IsMovingLeft,
			m_IsObjectDead,
			m_DrawObject;

	float	m_TimeAlive,
			m_TimeBetweenDrawings,
			m_Angle;

	std::vector<Point2f>	m_VerticesOrigin,
							m_TransVertices;


#pragma region HelprFunctions
	void HandleMovement(float elapsedSec);
	
	void UpdateShape();
	
	void CalculateBothEndsSpear();

	void UpdateFadingObject();
#pragma endregion
};

