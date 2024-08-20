#pragma once
#include "Level.h"

class Corridor1Level final: public Level
{
public:
	Corridor1Level(float scale, TextureManager* textureManager, const std::string& verticesPath);

	~Corridor1Level() = default;

#pragma region RuleOf5
	Corridor1Level(const Corridor1Level&) = delete;
	
	Corridor1Level& operator=(const Corridor1Level&) = delete;

	Corridor1Level(Corridor1Level&&) = delete;
	
	Corridor1Level& operator=(Corridor1Level&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) override;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const override;

	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const override;

private:
	void DrawParalexDrawBackground(const Point2f& camera) const;
	
	void DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const;

	void AddWallChandeliers() const;
};

