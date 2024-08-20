#include "pch.h"
#include <typeinfo>
#include "ObjectManager.h"
#include "Gold.h"
#include "TextureManager.h"
#include "Player.h"
#include "Enemy.h"
#include "WallChandelier.h"
#include "Zombie.h"


ObjectManager::ObjectManager(TextureManager* textureManager):
	m_pTextureManager{ textureManager }
{
}

ObjectManager::~ObjectManager()
{
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		delete m_pGameObjecs[index];
		m_pGameObjecs[index] = nullptr;
	}
}

void ObjectManager::Update( float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw )
{
	for ( int index{int(m_pGameObjecs.size())-1}; index >= 0; --index )
	{
		m_pGameObjecs[index]->Update(elapsedSec, level, player, cameraVieuw);

		if( m_pGameObjecs[index]->IsObjectDestroyed() )
		{
			if( m_pGameObjecs[index]->DoesObjectDropItem() )
			{
				m_pGameObjecs.push_back( m_pGameObjecs[index]->GetDroppedItem() );
			}
			delete m_pGameObjecs[index];
			m_pGameObjecs[index] = m_pGameObjecs.back();
			m_pGameObjecs.pop_back();
		}
	}
}

void ObjectManager::Draw() const
{
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		m_pGameObjecs[index]->Draw();
	}
}

void ObjectManager::AttackObject(const Rectf& attackShape, int damage)
{
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		m_pGameObjecs[index]->AttackObject( attackShape, damage );
	}
}

void ObjectManager::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		m_pGameObjecs[index]->AttackObject( pointA, pointB, damage );
	}
}

int ObjectManager::GetNumberOfObjects(ObjectType objectType) const
{
	int result{};
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		if( m_pGameObjecs[index]->GetObjectType() == objectType )
		{
			++result;
		}
	}
	return result;
}

void ObjectManager::ClearAllObjects()
{
	for( int index{ int(m_pGameObjecs.size()) - 1 }; index >= 0; --index )
	{
		if( m_pGameObjecs[index]->GetObjectType() != ObjectType::graveKeeperBackDash )
		{
			delete m_pGameObjecs[index];
			m_pGameObjecs[index] = m_pGameObjecs.back();
			m_pGameObjecs.pop_back();
		}
	}	
}

void ObjectManager::AddGameObject(GameObject* gameObject)
{
	m_pGameObjecs.push_back( gameObject );
}
