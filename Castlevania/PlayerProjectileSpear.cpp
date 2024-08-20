#include "pch.h"
#include "PlayerProjectileSpear.h"
#include "Texture.h"
#include "TextureManager.h"
#include <Matrix2x3.h>

PlayerProjectileSpear::PlayerProjectileSpear(Point2f center, TextureManager* textureManager, bool moveLeft):
	m_pTextureManager{ textureManager },
	m_SpearBack{ center },
	m_SpearBackOrigin{ center },
	m_IsMoving{ true },
	m_IsMovingLeft{ moveLeft },
	m_TimeAlive{ 0 },
	m_TimeBetweenDrawings{ 0 },
	m_Angle{ 0 },
	m_IsObjectDead{ false },
	m_DrawObject{ true }
{
	CalculateBothEndsSpear();
}

void PlayerProjectileSpear::Update(float elapsedSec)
{
	if( m_IsMoving )
	{
		m_TimeAlive += elapsedSec;
		HandleMovement( elapsedSec );
		UpdateShape();
	}
	else
	{
		m_TimeAlive += elapsedSec;
		m_TimeBetweenDrawings += elapsedSec;
		UpdateFadingObject();
	}

	const float timeUntilDeletion{ 10.f };

	if( m_TimeAlive > timeUntilDeletion )
	{
		m_IsObjectDead = true;
	}
}

void PlayerProjectileSpear::Draw() const
{
	const Texture* wingedGuardSpear{ m_pTextureManager->GetTexture( m_pTextureManager->wingedGuardSpear ) };

	glPushMatrix();
	{
		glTranslatef
		(
			m_SpearBack.x,
			m_SpearBack.y,
			0.0f
		);
		
		glRotatef
		(
			m_Angle, 
			0.0f, 
			0.0f, 
			1.0f
		);

		if ( m_IsMovingLeft )
		{
			glScalef
			(
				-1, 
				1, 
				1
			);
		}
		
		glTranslatef
		(
			-wingedGuardSpear->GetWidth() / 2,
			-wingedGuardSpear->GetHeight() / 2,
			0.0f
		);

		if( m_DrawObject )
		{
			wingedGuardSpear->Draw();
		}
	}
	glPopMatrix();
}

const std::vector<Point2f>& PlayerProjectileSpear::getTransVertices() const
{
	return m_TransVertices;
}

int PlayerProjectileSpear::GetDamage() const
{
	const int damage{ 19 };
	return damage;
}

int PlayerProjectileSpear::GetManaConsumption() const
{
	const int manaCost{ 10 };
	return manaCost;
}

bool PlayerProjectileSpear::IsObjectDead() const
{
	return m_IsObjectDead;
}

void PlayerProjectileSpear::StopMovement()
{
	if( m_IsMoving )
	{
		m_TimeAlive = 0;
		m_IsMoving = false;
	}
}


void PlayerProjectileSpear::HandleMovement(float elapsedSec)
{
	const float verticalSpeed{ 100.f };

	const float horizontalSpeed{ 400.0f };
	
	const Vector2f acceleration{ 0, -981.0f };

	const Texture* wingedGuardSpear{ m_pTextureManager->GetTexture(m_pTextureManager->wingedGuardSpear) };

	if ( m_IsMovingLeft )
	{
		m_SpearBack.x += -horizontalSpeed * elapsedSec;
	}
	else
	{
		m_SpearBack.x += horizontalSpeed * elapsedSec;
	}

	m_SpearBack.y = m_SpearBackOrigin.y + (verticalSpeed * m_TimeAlive) + (0.5f * acceleration.y * powf(m_TimeAlive,2));

	
	Point2f spearFront{};
	
	const float timeFrontEnd{ m_TimeAlive + ( wingedGuardSpear->GetWidth() / horizontalSpeed ) };
	
	spearFront.y = m_SpearBackOrigin.y + ( verticalSpeed * timeFrontEnd ) + ( 0.5f * acceleration.y * powf( timeFrontEnd, 2 ) );
	
	spearFront.x = sqrtf( powf( wingedGuardSpear->GetWidth(), 2) - powf( ( spearFront.y - m_SpearBack.y ), 2 ) ) + m_SpearBack.x;

	m_Angle = ( atan2f( ( spearFront.y - m_SpearBack.y ), ( spearFront.x - m_SpearBack.x ) ) * ( 180.f / float( M_PI ) ) );

	if( m_IsMovingLeft )
	{
		m_Angle = -m_Angle;
	}
}

void PlayerProjectileSpear::UpdateShape()
{
	const Texture* wingedGuardSpear{ m_pTextureManager->GetTexture( m_pTextureManager->wingedGuardSpear ) };

	Matrix2x3 matTranslate{}, matRotate{}, matScale{}, matCenter{};

	matTranslate.SetAsTranslate( m_SpearBack.x, m_SpearBack.y );

	matRotate.SetAsRotate( m_Angle );
	
	if( m_IsMovingLeft )
	{
		matScale.SetAsScale( -1 );
	}
	else
	{
		matScale.SetAsScale( 1 );
	}
	
	matCenter.SetAsTranslate
	(
		-wingedGuardSpear->GetWidth() / 2.f,
		-wingedGuardSpear->GetHeight() / 2.f
	);

	Matrix2x3 matTransform = matTranslate * matRotate * matScale * matCenter;

	m_TransVertices = matTransform.Transform( m_VerticesOrigin );
}

void PlayerProjectileSpear::CalculateBothEndsSpear()
{
	const Texture* wingedGuardSpear{ m_pTextureManager->GetTexture( m_pTextureManager->wingedGuardSpear ) };

	Point2f point{};
	
	point.x = 0;
	point.y = wingedGuardSpear->GetHeight() / 2.f;
	m_VerticesOrigin.push_back( point );

	point.x = wingedGuardSpear->GetWidth();
	m_VerticesOrigin.push_back( point );

	m_TransVertices = m_VerticesOrigin;
}

void PlayerProjectileSpear::UpdateFadingObject()
{
	const float		timeBetweenSwitch{ 0.1f },
					maxTimeToLive{ 1 };

	if(m_TimeAlive < maxTimeToLive)
	{
		if (m_TimeBetweenDrawings > timeBetweenSwitch)
		{
			m_TimeBetweenDrawings = 0;
			m_DrawObject = !m_DrawObject;
		}
	}
	else
	{
		m_DrawObject = false;
		m_IsObjectDead = true;
	}
}
