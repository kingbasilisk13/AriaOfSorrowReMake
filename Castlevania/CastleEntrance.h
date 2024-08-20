#pragma once
#include "Level.h"
class TextureManager;

class CastleEntrance final : public Level
{
public:
	CastleEntrance(float scale, TextureManager* textureManager, const std::string& verticesPath);
	
	~CastleEntrance() = default;

#pragma region RuleOf5
	CastleEntrance(const CastleEntrance&) = delete;
	
	CastleEntrance& operator=(const CastleEntrance&) = delete;
	
	CastleEntrance(CastleEntrance&&) = delete;
	
	CastleEntrance& operator=(CastleEntrance&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

private:
	void DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const;
};

