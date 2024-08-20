#include "pch.h"
#include "GraveKeeperBackDash.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Player.h"


GraveKeeperBackDash::GraveKeeperBackDash(TextureManager* textureManager, const Point2f& bottemLeft):
	GameObject{ textureManager,ObjectType::graveKeeperBackDash }
{
	m_Shape.bottom = bottemLeft.y;
	m_Shape.left = bottemLeft.x;
	m_Shape.width = 23;
	m_Shape.height = 39;

	m_NrOfFrames = 6;
	m_NrFramesPerSec = 12;
	m_AnimTime = 0;
	m_AnimFrame = 0;
}

void GraveKeeperBackDash::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	ChangeAnimationFrame( elapsedSec );

	if( m_HasBeenHit )
	{
		player->UnlockBackDash();
		m_ObjectDestroyed = true;
	}
	
	m_AnimationSource.width = m_Shape.width;
	m_AnimationSource.height = m_Shape.height;
	m_AnimationSource.bottom = m_AnimationSource.height;
	m_AnimationSource.left = m_AnimationSource.width * m_AnimFrame;
}

void GraveKeeperBackDash::Draw() const
{
	const Texture* graveKeeperBackDash{ m_pTextureManager->GetTexture( m_pTextureManager->graveKeeperBackDash ) };

	graveKeeperBackDash->Draw( m_Shape, m_AnimationSource );
}

void GraveKeeperBackDash::AttackObject(const Rectf& attackShape, int damage)
{
	if( utils::IsOverlapping( attackShape, m_Shape ) )
	{
		m_HasBeenHit = true;
	}
}

void GraveKeeperBackDash::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
	if( utils::IsOverlapping( pointA, pointB, m_Shape ) )
	{
		m_HasBeenHit = true;
	}
}

void GraveKeeperBackDash::ChangeAnimationFrame(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	
	if( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
	{
		m_AnimTime = 0;
		++m_AnimFrame;
		if( m_AnimFrame >= m_NrOfFrames )
		{
			m_AnimFrame = 0;
		}
	}
}
