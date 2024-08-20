#include "pch.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "Level.h"
#include "Player.h"

GameObject::GameObject(TextureManager* textureManager, ObjectType objectType):
	m_pTextureManager{textureManager},
	m_ObjectType{ objectType }
{
}

bool GameObject::DoesObjectDropItem()
{
	return false;
}

GameObject* GameObject::GetDroppedItem() const
{
	return nullptr;
}

bool GameObject::IsObjectDestroyed() const
{
	return m_ObjectDestroyed;
}

ObjectType GameObject::GetObjectType() const
{
	return m_ObjectType;
}
