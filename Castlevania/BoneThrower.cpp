#include "pch.h"
#include "BoneThrower.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Player.h"
#include "Level.h"
#include "EnemyProjectile.h"


BoneThrower::BoneThrower(TextureManager* textureManager, const Point2f& spawnLocation):
	Enemy(textureManager, ObjectType::boneThrower)
{
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_NrFramesPerSec = 0;
	m_NrOfFrames = 0;

	m_Shape.width = 30.f;
	m_Shape.height = 42.f;

	m_Health = 18;
	m_Damage = 9;

	m_Shape.bottom = spawnLocation.y;
	m_Shape.left = spawnLocation.x;

	m_AnimationSource.left = 0;
	m_AnimationSource.bottom = 41;
	m_AnimationSource.width = 30;
	m_AnimationSource.height = 42;

}

void BoneThrower::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	const Texture* boneProjectile{ m_pTextureManager->GetTexture( m_pTextureManager->boneProjectile ) };

	if(player->GetShape().left < m_Shape.left)
	{
		m_LookingLeft = true;
	}
	else
	{
		m_LookingLeft = false;
	}

	m_TimeUntilNextAttack += elapsedSec;
	
	if( ( m_AnimFrame == 4 ) && ( m_ActionState == ActionState::throwing ) && ( !m_ProjectileAlreadyThrown ) )
	{
		Rectf projectileShape{};

		projectileShape.left = m_Shape.left + ( m_Shape.width / 2.f );
		projectileShape.bottom = m_Shape.bottom + m_Shape.height;
		projectileShape.width = boneProjectile->GetWidth();
		projectileShape.height = boneProjectile->GetHeight();

		level->AddGameObjectToObjectManager(
			new EnemyProjectile{
				m_pTextureManager,
				projectileShape,
				m_LookingLeft
			}
		);

		m_ProjectileAlreadyThrown = true;

		m_TimeUntilNextAttack = 0;
	}

	const float timeTilNextAttack{ 2 };
	
	if( ( m_TimeUntilNextAttack > timeTilNextAttack ) && ( m_ActionState != ActionState::throwing ) )
	{
		m_ActionState = ActionState::throwing;
		
		m_ProjectileAlreadyThrown = false;
	}

	ChangeClipValues();
	
	ChangeAnimationFrame(elapsedSec);

	m_HasEnemyBeenHit = false;

	if( ( m_Health <= 0 ) && ( !m_HasEnemyBeenHit ) )
	{
		m_ObjectDestroyed = true;
	}
}

void BoneThrower::Draw() const
{
	const Texture* boneThrower{ m_pTextureManager->GetTexture( m_pTextureManager->boneThrower ) };

	glPushMatrix();
	{
		glTranslatef( m_Shape.left, m_Shape.bottom, 0 );
		
		if( m_LookingLeft )
		{
			glScalef( -1, 1, 1 );
			glTranslatef( -m_AnimationSource.width, 0, 0 );
		}

		boneThrower->Draw( Point2f{}, m_AnimationSource );		
	}
	glPopMatrix();
}

void BoneThrower::AttackObject(const Rectf& attackShape, int damage)
{
	if( ( utils::IsOverlapping( attackShape, m_Shape ) ) && ( !m_HasEnemyBeenHit ) )
	{
		m_Health -= damage;
		
		m_HasEnemyBeenHit = true;
	}
}

void BoneThrower::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
	if( ( utils::IsOverlapping( pointA, pointB, m_Shape ) ) && ( !m_HasEnemyBeenHit ) )
	{
		m_Health -= damage;
		
		m_HasEnemyBeenHit = true;
	}
}

void BoneThrower::ChangeAnimationFrame(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if( !m_HasEnemyBeenHit )
	{
		if( m_AnimTime >= 1.f / m_NrFramesPerSec )
		{
			m_AnimTime = 0;
			++m_AnimFrame;
			if( m_AnimFrame >= m_NrOfFrames )
			{
				m_AnimFrame = 0;
				if( m_ActionState == ActionState::throwing )
				{
					m_ActionState = ActionState::idle;
				}
			}
		}
	}
}

void BoneThrower::ChangeClipValues()
{
	float clipWidth{},
		clipHeight{};

	switch( m_ActionState )
	{
	case BoneThrower::ActionState::idle:
		clipWidth = 30.f;
		clipHeight = 42.f;
		m_NrOfFrames = 0;
		m_NrFramesPerSec = 1;
		m_AnimationSource.bottom = float( ActionState::throwing );
		break;
	case BoneThrower::ActionState::throwing:
		clipWidth = 30.f;
		clipHeight = 42.f;
		m_NrOfFrames = 7;
		m_NrFramesPerSec = 7;
		m_AnimationSource.bottom = float( m_ActionState );
		break;
	default:
		break;
	}

	m_AnimationSource.width = clipWidth;
	
	m_AnimationSource.height = clipHeight;
	
	m_AnimationSource.left = (clipWidth * m_AnimFrame);
}
