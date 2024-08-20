#include "pch.h"
#include "Hud.h"
#include "Texture.h"
#include "TextureManager.h"

Hud::Hud(TextureManager* textureManager, const Point2f& topLeft, float scale):
	m_pTextureManager{textureManager},
	m_Scale{scale},
	m_PlayerHealth{ 0 },
	m_PlayerMaxHealth{ 0 },
	m_PlayerMana{ 0 },
	m_PlayerMaxMana{ 0 }
{
	m_BottomLeft.x = topLeft.x;

	const Texture* counter{ textureManager->GetTexture(textureManager->counter) };
	
	if(counter)
	{
		m_BottomLeft.y = ( topLeft.y / scale ) - counter->GetHeight();
	}
}

void Hud::Update(float playerHealth, float playerMaxHealth, float playerMana, float playerMaxMana)
{
	m_PlayerHealth = playerHealth;
	
	m_PlayerMaxHealth = playerMaxHealth;

	m_PlayerMana = playerMana;
	
	m_PlayerMaxMana = playerMaxMana;
}

void Hud::Draw() const
{
	glPushMatrix();
	{
		glScalef(m_Scale, m_Scale, 1);

		m_pTextureManager->GetTexture( m_pTextureManager->counter )->Draw( m_BottomLeft );
		
		DrawHealthBar();
		
		DrawManaBar();
		
		DrawHealthInNumbers();
	}
	glPopMatrix();
}

void Hud::DrawHealthBar() const
{
	const float widthBarFilled{ 1 }, heightBarFilled{ 3 };
	
	const float totalWidthBar{ 64.f };
	
	const float totalHeightBar{ 3.f };

#pragma region Collord part
	const Texture* counter{ m_pTextureManager->GetTexture(m_pTextureManager->counter) };

	const float moveDown{ 9.f };
	const float moveRight{ 29.f };

	Rectf healthBarFullLocation{};
	healthBarFullLocation.bottom = m_BottomLeft.y + counter->GetHeight() - moveDown;
	healthBarFullLocation.left = m_BottomLeft.x + moveRight;
	healthBarFullLocation.width = totalWidthBar * ( float( m_PlayerHealth ) / m_PlayerMaxHealth );
	healthBarFullLocation.height = totalHeightBar;

	Rectf HealthBarFullSource{};
	HealthBarFullSource.width = widthBarFilled;
	HealthBarFullSource.height = heightBarFilled;
#pragma endregion

#pragma region Empty part
	Rectf healthBarEmptyLocation{};
	healthBarEmptyLocation.bottom = healthBarFullLocation.bottom;
	healthBarEmptyLocation.left = healthBarFullLocation.left;
	healthBarEmptyLocation.width = totalWidthBar;
	healthBarEmptyLocation.height = totalHeightBar;

	Rectf HealthBarEmptySource{};
	HealthBarEmptySource.left = 1;
	HealthBarEmptySource.width = widthBarFilled;
	HealthBarEmptySource.height = heightBarFilled;

#pragma endregion
	const Texture* healthBar{ m_pTextureManager->GetTexture(m_pTextureManager->healthBar) };
	
	healthBar->Draw(healthBarEmptyLocation, HealthBarEmptySource);

	if ( m_PlayerHealth > 0 )
	{
		healthBar->Draw(healthBarFullLocation, HealthBarFullSource);
	}
}

void Hud::DrawManaBar() const
{
	const float widthBarFilled{ 1 }, heightBarFilled{ 3 };
	const float totalWidthBar{ 64.f };
	const float totalHeightBar{ 3.f };

#pragma region Collord part
	const Texture* counter{ m_pTextureManager->GetTexture(m_pTextureManager->counter) };

	const float moveDown{ 14.f };
	const float moveRight{ 29.f };

	Rectf manaBarFullLocation{};
	manaBarFullLocation.bottom = m_BottomLeft.y + counter->GetHeight() - moveDown;
	manaBarFullLocation.left = m_BottomLeft.x + moveRight;
	manaBarFullLocation.width = totalWidthBar * (float(m_PlayerMana) / m_PlayerMaxMana);
	manaBarFullLocation.height = totalHeightBar;

	Rectf manaBarFullSource{};
	manaBarFullSource.width = widthBarFilled;
	manaBarFullSource.height = heightBarFilled;
#pragma endregion

#pragma region Empty part
	Rectf manaBarEmptyLocation{};
	manaBarEmptyLocation.bottom = manaBarFullLocation.bottom;
	manaBarEmptyLocation.left = manaBarFullLocation.left;
	manaBarEmptyLocation.width = totalWidthBar;
	manaBarEmptyLocation.height = totalHeightBar;

	Rectf manaBarEmptySource{};
	manaBarEmptySource.left = 1;
	manaBarEmptySource.width = widthBarFilled;
	manaBarEmptySource.height = heightBarFilled;
#pragma endregion

	const Texture* manaBar{ m_pTextureManager->GetTexture(m_pTextureManager->manaBar) };

	manaBar->Draw(manaBarEmptyLocation, manaBarEmptySource);

	if(m_PlayerMana > 0)
	{
		manaBar->Draw(manaBarFullLocation, manaBarFullSource);
	}
}

void Hud::DrawHealthInNumbers() const
{
	const float widthOfNumber{ 8 }, heightOfNumber{ 8 };
	const float distanceBetweenNumbers{ 1.f };

	int health = int(m_PlayerHealth);

	int devider{ 100 };

	int distance{ 0 };
	while (distance <= 2)
	{
#pragma region Collord part
		
		const float moveUp{ 15.f };

		Rectf numberLocation{};
		numberLocation.bottom = m_BottomLeft.y + moveUp;
		numberLocation.left = m_BottomLeft.x + ( widthOfNumber * distance );
		numberLocation.width = widthOfNumber;
		numberLocation.height = heightOfNumber;

		Rectf numberSource{};
		numberSource.left = widthOfNumber * float( health / devider );
		numberSource.width = widthOfNumber;
		numberSource.height = heightOfNumber;
#pragma endregion

		m_pTextureManager->GetTexture( m_pTextureManager->blueNumbers )->Draw( numberLocation, numberSource );
		health = health - ( health / devider ) * devider;
		distance += 1;
		devider = devider / 10;
	}
}
