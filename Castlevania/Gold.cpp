#include "pch.h"
#include "Gold.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Level.h"
#include "Player.h"

Gold::Gold(TextureManager* textureManager, const Point2f& bottemLeft):
	GameObject{ textureManager, ObjectType::gold }
{
	m_NrOfFrames = 3;
	m_NrFramesPerSec = 12;
	m_AnimTime = 0;
	m_AnimFrame = 0;

	m_Shape.left = bottemLeft.x;
	m_Shape.bottom = bottemLeft.y;
	m_Shape.width = 9;
	m_Shape.height = 9;

	const int maximum{ 101 };

	int randomNumber{ rand() % maximum };

	if( randomNumber >= 88 )
	{
		m_GoldCoinType = GoldCoinType::yellow;
	}
	else if( randomNumber >= 76 )
	{
		m_GoldCoinType = GoldCoinType::green;
	}
	else if( randomNumber >= 51 )
	{
		m_GoldCoinType = GoldCoinType::orange;
	}
	else
	{
		m_GoldCoinType = GoldCoinType::blue;
	}

}

void Gold::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	IsOverlapping(player);

	ChangeAnimationFrame(elapsedSec);
	
	ChangeObjectLifeTime(elapsedSec);
	
	HandleMovement(elapsedSec, level);

	const Texture* goldCoins{ m_pTextureManager->GetTexture(m_pTextureManager->goldCoins) };
	
	m_AnimationSource.width = 9;
	m_AnimationSource.height = 9;
	m_AnimationSource.bottom = m_AnimationSource.height * int( m_GoldCoinType );
	m_AnimationSource.left = m_AnimationSource.width * m_AnimFrame;
}

void Gold::Draw() const
{
	const Texture* goldCoins{ m_pTextureManager->GetTexture( m_pTextureManager->goldCoins ) };

	if( m_DrawObject )
	{
		goldCoins->Draw( m_Shape, m_AnimationSource );
	}
}

void Gold::AttackObject(const Rectf& attackShape, int damage)
{
}

void Gold::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
}

void Gold::IsOverlapping(Player* player)
{
	if( utils::IsOverlapping( player->GetShape(), m_Shape ) )
	{
		player->AddCoinValue( GetCoinValue() );
		m_ObjectDestroyed = true;
	}
}

void Gold::ChangeAnimationFrame(float elapsedSec)
{
	if( m_AnimFrame == 0 )
	{
		m_ChangeFrameValue = 1;
	}
	else if( ( m_AnimFrame + m_ChangeFrameValue ) == ( m_NrOfFrames ) )
	{
		m_ChangeFrameValue = -1;
	}

	m_AnimTime += elapsedSec;

	if( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
	{
		m_AnimTime = 0;
		m_AnimFrame += m_ChangeFrameValue;
	}
}

void Gold::ChangeObjectLifeTime(float elapsedSec)
{
	m_ObjectTimeLived += elapsedSec;
	
	if( ( m_ObjectTimeLived > m_ObjectLifeTime ) && ( m_ObjectTimeLived <= ( 2.f * m_ObjectLifeTime ) ) )
	{
		m_ObjectTimeDespawning += elapsedSec;

		if( m_ObjectTimeDespawning >= m_ObjectDespawnTime )
		{
			m_DrawObject = !m_DrawObject;
			
			m_ObjectTimeDespawning = 0;

			if( m_DrawObject )
			{
				m_ObjectDespawnTime = ( m_ObjectLifeTime - ( m_ObjectTimeLived - m_ObjectLifeTime ) ) * 0.1f;
			}
		}
	}
	else if( m_ObjectTimeLived > ( 2.f * m_ObjectLifeTime ) )
	{
		m_DrawObject = false;
		m_ObjectDestroyed = true;
	}
}

void Gold::HandleMovement(float elapsedSec, const Level* level)
{
	const float gravity{ -981.f };

	m_Velocity.y += gravity * elapsedSec;

	level->HandleCollision( m_Shape, m_Velocity );
	
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

int Gold::GetCoinValue() const
{
	int result{};

	switch( m_GoldCoinType )
	{
	case Gold::GoldCoinType::blue:
		result = 1;
		break;
	case Gold::GoldCoinType::orange:
		result = 10;
		break;
	case Gold::GoldCoinType::green:
		result = 50;
		break;
	case Gold::GoldCoinType::yellow:
		result = 100;
		break;
	default:
		break;
	}

	return result;
}
