#include "pch.h"
#include "Zombie.h"

#include "Texture.h"
#include "TextureManager.h"
#include "Player.h"

Zombie::Zombie(const Rectf& spawnArea, TextureManager* textureManager, const Rectf& playerLocation, const Point2f& cameraBottemLeft, float windowWidth) :
	Enemy{ textureManager, ObjectType::zombie },
	m_SpawnArea{ spawnArea }
{
	m_AnimTime = 0;
	m_AnimFrame = 0;
	m_NrFramesPerSec = 0;
	m_NrOfFrames = 0;

	m_Shape.width = 25.f;
	m_Shape.height = 30.f;
	m_Health = 18 ;
	m_Damage = 9 ;

	const float max{ (spawnArea.left + spawnArea.width) - m_Shape.width };
	
	const float min{ spawnArea.left };

	m_Shape.bottom = spawnArea.bottom;
	do
	{
		m_Shape.left = rand() % int(max - min + 1) + min;

		if( m_Shape.left < playerLocation.left )
		{
			m_WalkingLeft = false;
		}
		else
		{
			m_WalkingLeft = true;
		}
	} while( ( m_Shape.left < cameraBottemLeft.x ) || ( m_Shape.left > ( cameraBottemLeft.x + windowWidth ) ) );
}

void Zombie::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	if( ( ( m_Shape.left + m_Shape.width ) < cameraVieuw.left ) || ( m_Shape.left > ( cameraVieuw.left + cameraVieuw.width ) ) )
	{
		GameObject::m_ObjectDestroyed = true;
		return;
	}

	ChangeActionState();
	ChangeClipValues();
	ChangeAnimationFrame(elapsedSec);

	if( ( m_ActionState != ActionState::spawning ) && ( m_ActionState != ActionState::dying ) && ( !m_HasEnemyBeenHit ) )
	{
		HandleMovement(elapsedSec);
		AttackPlayer(player);
	}

	if( ( m_Health <= 0 ) && ( !m_HasEnemyBeenHit ) )
	{
		m_ObjectDestroyed = true;
	}
}

void Zombie::Draw() const
{
	const Texture* zombie{ m_pTextureManager->GetTexture( m_pTextureManager->zombie ) };
	
	const Texture* blood{ m_pTextureManager->GetTexture( m_pTextureManager->blood ) };
	
	glPushMatrix();
	{
		glTranslatef( m_Shape.left, m_Shape.bottom, 0 );
		if( !m_WalkingLeft )
		{
			glScalef( -1, 1, 1 );
			glTranslatef( -m_AnimationSource.width, 0, 0 );
		}
		
		zombie->Draw( Point2f{}, m_AnimationSource );

		if( m_HasEnemyBeenHit )
		{
			Rectf sourceRectParticles{};
			sourceRectParticles.width = 32.f;
			sourceRectParticles.height = 33.f;
			sourceRectParticles.left = (sourceRectParticles.width * m_ParticlesAnimFrame);
			sourceRectParticles.bottom = 142.f;

			blood->Draw(Point2f{ 0, 10 }, sourceRectParticles);
		}
	}
	glPopMatrix();
}

void Zombie::AttackObject(const Rectf& attackShape, int damage)
{
	if( ( utils::IsOverlapping( attackShape, m_Shape ) ) && ( !m_HasEnemyBeenHit ) && ( m_ActionState != ActionState::spawning ) )
	{
		m_Health -= damage;
		m_HasEnemyBeenHit = true;
	}
}

void Zombie::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
	if( ( utils::IsOverlapping(pointA, pointB, m_Shape) ) && ( !m_HasEnemyBeenHit ) && ( m_ActionState != ActionState::spawning ) )
	{
		m_Health -= damage;
		m_HasEnemyBeenHit = true;
	}
}

void Zombie::AttackPlayer(Player* player)
{
	if( ( m_ActionState == ActionState::walkingHeadDown ) || ( m_ActionState == ActionState::walkingHeadUp ) )
	{
		Rectf leftSide{ player->GetHitbox() };
		leftSide.width /= 2.f;

		if( utils::IsOverlapping( player->GetHitbox(), m_Shape ) )
		{
			player->TakeDamage( m_Damage, utils::IsOverlapping( leftSide, m_Shape ) );
		}
	}
}

void Zombie::HandleMovement(float elapsedSec)
{
	if( m_WalkingLeft )
	{
		m_Shape.left -= m_Horizontal * elapsedSec;
	}
	else
	{
		m_Shape.left += m_Horizontal * elapsedSec;
	}

	if( m_Shape.left <= m_SpawnArea.left )
	{
		m_Shape.left = m_SpawnArea.left;
	}
	else if( ( m_Shape.left + m_Shape.width ) >= ( m_SpawnArea.left + m_SpawnArea.width ) )
	{
		m_Shape.left = m_SpawnArea.left + m_SpawnArea.width - m_Shape.width;
	}
}

void Zombie::ChangeActionState()
{
	if( ( m_ActionState == ActionState::spawning ) && ( m_AnimFrame == 0 ) && ( m_StartWalking ) )
	{
		int randomValue{ rand() % 10 + 1 };
		if( randomValue <= 5 )
		{
			m_ActionState = ActionState::walkingHeadDown;
		}
		else
		{
			m_ActionState = ActionState::walkingHeadUp;
		}
		m_AnimFrame = 0;
		m_AnimTime = 0;
	}

	if( m_ActionState != ActionState::deSpawning )
	{
		if( ( m_Shape.left <= m_SpawnArea.left ) || ( ( m_Shape.left + m_Shape.width ) >= ( m_SpawnArea.left + m_SpawnArea.width ) ) )
		{
			m_ActionState = ActionState::deSpawning;
			m_AnimFrame = 5;
			m_AnimTime = 0;
		}
	}

	if( m_Health <= 0 )
	{
		m_ActionState = ActionState::dying;
		m_AnimFrame = 0;
		m_AnimTime = 0;
	}
}

void Zombie::ChangeAnimationFrame(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if( m_ActionState == ActionState::deSpawning )
	{
		if( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
		{
			m_AnimTime = 0;
			--m_AnimFrame;
			if( m_AnimFrame <= -1 )
			{
				m_Health = 0;
			}
		}
	}
	else
	{
		if( !m_HasEnemyBeenHit )
		{
			if (m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
			{
				m_AnimTime = 0;
				++m_AnimFrame;
				m_StartWalking = true;
				if( m_AnimFrame >= m_NrOfFrames )
				{
					m_AnimFrame = 0;
				}
			}
		}
	}

	if( m_HasEnemyBeenHit )
	{
		m_ParticlesAnimTime += elapsedSec;

		if( m_ParticlesAnimTime >= ( 1.f / 5 ) )
		{
			m_ParticlesAnimTime = 0;
			++m_ParticlesAnimFrame;
			if( m_ParticlesAnimFrame >= 5 )
			{
				m_ParticlesAnimFrame = 0;
				m_HasEnemyBeenHit = false;
			}
		}
	}
}

void Zombie::ChangeClipValues()
{
	float clipWidth{};
	float clipHeight{};

	switch( m_ActionState )
	{
	case Zombie::ActionState::spawning:
		clipWidth = 25.f;
		clipHeight = 28.f;
		m_NrOfFrames = 6;
		m_NrFramesPerSec = 6;
		break;
	case Zombie::ActionState::walkingHeadDown:
		clipWidth = 23.f;
		clipHeight = 35.f;
		m_NrOfFrames = 2;
		m_NrFramesPerSec = 2;
		break;
	case Zombie::ActionState::walkingHeadUp:
		clipWidth = 25.f;
		clipHeight = 38.f;
		m_NrOfFrames = 4;
		m_NrFramesPerSec = 4;
		break;
	case Zombie::ActionState::dying:
		clipWidth = 33.f;
		clipHeight = 43.f;
		m_NrOfFrames = 1;
		m_NrFramesPerSec = 1;
		break;
	case Zombie::ActionState::deSpawning:
		clipWidth = 25.f;
		clipHeight = 28.f;
		m_NrOfFrames = 6;
		m_NrFramesPerSec = 6;
		break;
	default:
		break;
	}

	m_AnimationSource.width = clipWidth;
	m_AnimationSource.height = clipHeight;
	m_AnimationSource.left = ( clipWidth * m_AnimFrame );
	m_AnimationSource.bottom = float( m_ActionState );

	if( m_ActionState == ActionState::deSpawning )
	{
		m_AnimationSource.bottom = float( ActionState::spawning );
	}
}