#include "pch.h"
#include "Enemy.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Player.h"

Enemy::Enemy(TextureManager* textureManager, ObjectType objectType):
	GameObject{ textureManager, objectType }
{

}