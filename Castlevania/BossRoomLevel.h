#pragma once
#include "Level.h"

class BossRoomLevel final : public Level
{
public:
	BossRoomLevel(float scale, TextureManager* textureManager, const std::string& verticesPath);

	~BossRoomLevel() = default;

#pragma region RuleOf5
	BossRoomLevel(const BossRoomLevel&) = delete;
	
	BossRoomLevel& operator=(const BossRoomLevel&) = delete;

	BossRoomLevel(BossRoomLevel&&) = delete;
	
	BossRoomLevel& operator=(BossRoomLevel&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

private:
	void DrawParalexDrawBackground(const Point2f& camera) const;
	
	void DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const;
};
