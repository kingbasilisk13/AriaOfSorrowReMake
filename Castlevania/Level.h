#pragma once
#include <vector>
#include "Vector2f.h"

class Texture;
class Player;
class ObjectManager;
class TextureManager;
class GameObject;

class Level
{
public:
	Level(TextureManager* textureManager, float scale, const Texture* background, const std::string& verticesPath);
	
	virtual ~Level();

#pragma region RuleOf5
	Level(const Level&) = delete;
	
	Level& operator=(const Level&) = delete;
	
	Level(Level&&) = delete;
	
	Level& operator=(Level&&) = delete;
#pragma endregion
	virtual void Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth) = 0;

	virtual void DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const = 0;

	virtual void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	
	virtual bool IsOnGround(const Rectf& actorShape) const;
	
	bool IsPlayerFalling(const Rectf& actorShape) const;
	
	bool DoesProjectileHitCollision(const Point2f& spearBack, const Point2f& spearFront) const;

	bool ReachedEndLeft(const Rectf& actorShape) const;
	
	bool ReachedEndRight(const Rectf& actorShape) const;

	const Rectf& GetBoundaries() const;
	
	virtual Point2f GetSpawnLocation(bool rightSpawnLocation) const = 0;

	void HandlePlayerAttack(const Rectf& attackShape, int damage) const;
	
	void HandlePlayerAttack(const Point2f& pointA, const Point2f& pointB, int damage) const;

	void AddGameObjectToObjectManager(GameObject* gameObject) const;

protected:
	std::vector<std::vector<Point2f>> m_Vertices;
	
	Rectf m_Boundaries;
	
	float m_Scale;

	ObjectManager* m_pObjectManager;
	
	TextureManager* m_pTextureManager;

	Point2f CalculateCenterActor(const Rectf& actorShape) const;
	
	Point2f CalculateBottemCenterActor(const Rectf& actorShape) const;

private:
	Point2f CalculateTopCenterActor(const Rectf& actorShape) const;
	
	Point2f CalculateLeftCenterActor(const Rectf& actorShape) const;
	
	Point2f CalculateRightCenterActor(const Rectf& actorShape) const;
	
	Point2f CalculateTopLeft(const Rectf& actorShape) const;
	
	Point2f CalculateTopRight(const Rectf& actorShape) const;
};

