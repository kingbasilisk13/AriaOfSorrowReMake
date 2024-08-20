#pragma once
class TextureManager;
class Level;
class Player;

enum class ObjectType
{
	wallChandelier,
	graveKeeperBackDash,
	gold,
	enemyProjectile,
	zombie,
	boneThrower
};

class GameObject
{
public:
	GameObject(TextureManager* textureManager, ObjectType objectType);
	
	virtual ~GameObject() = default;

#pragma region RuleOf5
	GameObject(const GameObject&) = delete;
	
	GameObject& operator=(const GameObject&) = delete;
	
	GameObject(GameObject&&) = delete;
	
	GameObject& operator=(GameObject&&) = delete;
#pragma endregion

	virtual void Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw) = 0;

	virtual void Draw() const = 0;

	virtual void AttackObject(const Rectf& attackShape, int damage) = 0;
	
	virtual void AttackObject(const Point2f& pointA, const Point2f& pointB, int damage) = 0;

	virtual bool DoesObjectDropItem();
	
	virtual GameObject* GetDroppedItem() const;

	bool IsObjectDestroyed() const;

	ObjectType GetObjectType() const;

protected:
	TextureManager* m_pTextureManager;
	
	bool m_ObjectDestroyed{ false };

	Rectf m_Shape,
		m_AnimationSource;

	int m_NrOfFrames{ 0 },
		m_NrFramesPerSec{ 0 },
		m_AnimFrame{ 0 };

	float m_AnimTime{ 0 };

	ObjectType m_ObjectType;

private:

};

