#include "pch.h"
#include "WallChandelier.h"
#include "Texture.h"
#include "TextureManager.h"
#include <utils.h>
#include "Gold.h"

WallChandelier::WallChandelier(TextureManager* textureManager, const Point2f& center):
	GameObject{ textureManager, ObjectType::wallChandelier }
{
	m_NrOfFrames = 3;
	m_NrFramesPerSec = 9;
	m_AnimTime = 0;
	m_AnimFrame = 0;

	m_Shape.width = 14.f;
	m_Shape.height = 17.f;
	m_Shape.left = center.x - (m_Shape.width / 2.f);
	m_Shape.bottom = center.y - (m_Shape.height / 2.f);

	m_AnimationSource.bottom = 16.f;
}

void WallChandelier::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	m_TimerFlickering += elapsedSec;
	
	const float flickeringLight{ 0.1f };

	if( m_TimerFlickering >= flickeringLight )
	{
		m_IsBacklightVisible = !m_IsBacklightVisible;
		m_TimerFlickering = 0;
	}

	m_AnimTime += elapsedSec;
	
	if( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
	{
		m_AnimTime = 0;
		m_AnimFrame += m_ChangeLoop;
		if( m_AnimFrame == ( m_NrOfFrames - 1 ) )
		{
			m_ChangeLoop = -1;
		}
		else if( m_AnimFrame <= 0 )
		{
			m_ChangeLoop = 1;
		}
	}

	m_AnimationSource.width = m_Shape.width;
	m_AnimationSource.height = m_Shape.height;
	m_AnimationSource.left = ( m_Shape.width * m_AnimFrame );
}

void WallChandelier::Draw() const
{
	const Texture* backLight{ m_pTextureManager->GetTexture( m_pTextureManager->backLight ) };
	
	const Texture* wallChandelier{ m_pTextureManager->GetTexture( m_pTextureManager->wallChandelier ) };

	const float moveBacklightLeft{ 1.f },
		moveBacklightUp{ 5.f };

	if( m_IsBacklightVisible )
	{
		backLight->Draw( Point2f( ( m_Shape.left - moveBacklightLeft ), ( m_Shape.bottom + moveBacklightUp ) ) );
	}

	wallChandelier->Draw( Point2f( m_Shape.left, m_Shape.bottom ), m_AnimationSource );
}

void WallChandelier::AttackObject(const Rectf& attackShape, int damage)
{
	if( utils::IsOverlapping( m_Shape, attackShape ) )
	{
		m_ObjectDestroyed = true;
	}
}

void WallChandelier::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
	if( utils::IsOverlapping( pointA, pointB, m_Shape ) )
	{
		m_ObjectDestroyed = true;
	}
}

bool WallChandelier::DoesObjectDropItem()
{
	return true;
}

GameObject* WallChandelier::GetDroppedItem() const
{
	return new Gold( m_pTextureManager, Point2f( m_Shape.left, m_Shape.bottom ) );
}

const Rectf& WallChandelier::GetShape() const
{
	return m_Shape;
}

