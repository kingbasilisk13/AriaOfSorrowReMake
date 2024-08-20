#pragma once
#include <vector>
#include "GameObject.h"

class Player;
class Gold;
class Level;
class Enemie;
class GameObject;
class WallChandelier;
class TextureManager;

class ObjectManager final
{
public:
	ObjectManager(TextureManager* textureManager);

	~ObjectManager();

#pragma region RuleOf5
	ObjectManager(const ObjectManager&) = delete;
	
	ObjectManager& operator=(const ObjectManager&) = delete;
	
	ObjectManager(ObjectManager&&) = delete;
	
	ObjectManager& operator=(ObjectManager&&) = delete;
#pragma endregion

	void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw);
	
	void Draw() const;

	void AttackObject(const Rectf& attackShape, int damage);
	
	void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage);

	int GetNumberOfObjects(ObjectType objectType) const;

	void ClearAllObjects();

	void AddGameObject(GameObject* gameObject);

private:
	TextureManager* m_pTextureManager;

	std::vector<GameObject*> m_pGameObjecs;
};

