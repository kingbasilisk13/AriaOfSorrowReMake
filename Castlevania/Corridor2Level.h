#pragma once
#include "Level.h"

class Corridor2Level final : public Level
{
public:
	Corridor2Level(float scale, TextureManager* textureManager, const std::string& verticesPath);

	~Corridor2Level() = default;

#pragma region RuleOf5
	Corridor2Level(const Corridor2Level&) = delete;
	
	Corridor2Level& operator=(const Corridor2Level&) = delete;

	Corridor2Level(Corridor2Level&&) = delete;
	
	Corridor2Level& operator=(Corridor2Level&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

private:
	void DrawParalexDrawBackground(const Point2f& camera) const;
	
	void DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const;

	void SpawnBoneThrowers() const;
};

