#pragma once
#include "Level.h"

class StoneHallwayLevel final : public Level
{
public:
	StoneHallwayLevel(float scale, TextureManager* textureManager, const std::string& verticesPath);

	~StoneHallwayLevel() = default;

#pragma region RuleOf5
	StoneHallwayLevel(const StoneHallwayLevel&) = delete;

	StoneHallwayLevel& operator=(const StoneHallwayLevel&) = delete;

	StoneHallwayLevel(StoneHallwayLevel&&) = delete;

	StoneHallwayLevel& operator=(StoneHallwayLevel&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

private:
};
