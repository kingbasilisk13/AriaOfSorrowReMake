#include "pch.h"
#include "EnemyProjectile.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Player.h"
#include "Level.h"

EnemyProjectile::EnemyProjectile(TextureManager* textureManager, const Rectf& projectileShape, bool thrownToTheLeft):
	GameObject{ textureManager, ObjectType::enemyProjectile },
	m_ThrownToTheLeft{ thrownToTheLeft },
	m_ProjectileYOrigin{ projectileShape.bottom}
{
	m_Shape = projectileShape;	
}

void EnemyProjectile::Update(float elapsedSec, const Level* level, Player* player, const Rectf& cameraVieuw)
{
	m_TimeAliveProjectile += elapsedSec;

	const float verticalSpeed{ 300.f };
	
	const float horizontalSpeed{ 50.0f };
	
	const Vector2f acceleration{ 0, -981.0f };

	const Texture* wingedGuardSpear{ m_pTextureManager->GetTexture( m_pTextureManager->wingedGuardSpear ) };

	if( m_ThrownToTheLeft )
	{
		m_Shape.left -= horizontalSpeed * elapsedSec;
	}
	else
	{
		m_Shape.left += horizontalSpeed * elapsedSec;
	}

	m_Shape.bottom = m_ProjectileYOrigin + (verticalSpeed * m_TimeAliveProjectile) + (0.5f * acceleration.y * powf(m_TimeAliveProjectile, 2));

	bool doesProjectileHitGround = level->DoesProjectileHitCollision(
		Point2f{
			m_Shape.left,
			m_Shape.bottom
		},
		Point2f{
			m_Shape.left + m_Shape.width,
			m_Shape.bottom + m_Shape.height
		}
	);

	AttackPlayer( player );

	if( doesProjectileHitGround )
	{
		m_ObjectDestroyed = true;
	}
}

void EnemyProjectile::Draw() const
{
	const Texture* boneProjectile{ m_pTextureManager->GetTexture( m_pTextureManager->boneProjectile ) };
	
	float angle{ 360.f / 1.5f };

	glPushMatrix();
	{
		glTranslatef
		(
			m_Shape.left + m_Shape.width / 2.f,
			m_Shape.bottom + m_Shape.height / 2.f,
			0.0f
		);

		glRotatef( angle * m_TimeAliveProjectile, 0.0f, 0.0f, 1.0f );

		glTranslatef
		(
			-m_Shape.width / 2.f,
			-m_Shape.height / 2.f,
			0.0f
		);

		if( m_ThrownToTheLeft )
		{
			glScalef( -1, 1, 1 );
			glTranslatef( -m_Shape.width, 0, 0 );
		}

		boneProjectile->Draw();
	}
	glPopMatrix();	
}

void EnemyProjectile::AttackObject(const Rectf& attackShape, int damage)
{
}

void EnemyProjectile::AttackObject(const Point2f& pointA, const Point2f& pointB, int damage)
{
}

void EnemyProjectile::AttackPlayer(Player* player)
{
	const int damage{ 9 };

	Rectf leftSide{ player->GetHitbox() };
	leftSide.width /= 2.f;

	if( utils::IsOverlapping( player->GetHitbox(), m_Shape ) )
	{
		player->TakeDamage( damage, utils::IsOverlapping( leftSide, m_Shape ) );
		m_ObjectDestroyed = true;
	}
}