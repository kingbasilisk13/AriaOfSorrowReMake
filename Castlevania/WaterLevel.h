#pragma once
#include "Level.h"

class TextureManager;

class WaterLevel final: public Level
{
public:
	WaterLevel(float scale, TextureManager* textureManager, const std::string& verticesPath);
	
	~WaterLevel() = default;

#pragma region RuleOf5
	WaterLevel(const WaterLevel&) = delete;
	
	WaterLevel& operator=(const WaterLevel&) = delete;
	
	WaterLevel(WaterLevel&&) = delete;
	
	WaterLevel& operator=(WaterLevel&&) = delete;
#pragma endregion
	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;
 
	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

	virtual void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const override;
	
	virtual bool IsOnGround(const Rectf& actorShape) const override;

private:
	const Rectf m_PoolOfWater{ Rectf{ 124.f, 13.f, 300.f, 60.f } };

	std::vector<Rectf> m_pPlatforms;

	const std::vector<Point2f> CalculateVertices(const Rectf& platform) const;

	void AddWallChandeliers() const;
};



