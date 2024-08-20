#pragma once
#include <utils.h>
#include "GameObject.h"

class Texture;
class Player;
class TextureManager;

class Enemy : public GameObject
{
public:
	Enemy(TextureManager* textureManager, ObjectType objectType);
	
	virtual ~Enemy() = default;

#pragma region rule of 5
	Enemy(const Enemy&) = delete;
	
	Enemy& operator=(const Enemy&) = delete;
	
	Enemy(Enemy&&) = delete;
	
	Enemy& operator=(Enemy&&) = delete;
#pragma endregion

protected:
	bool m_HasEnemyBeenHit{ false };

	int m_Health{ 0 },
		m_Damage{ 0 };

private:

};

