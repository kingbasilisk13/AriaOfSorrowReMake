#include "pch.h"
#include "Corridor2Level.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "WallChandelier.h"
#include "BoneThrower.h"

Corridor2Level::Corridor2Level(float scale, TextureManager* textureManager, const std::string& verticesPath):
	Level( textureManager, scale, textureManager->GetTexture( textureManager->corridor2 ), verticesPath )
{
}

void Corridor2Level::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
	const Rectf camera{ cameraBottemLeft.x, cameraBottemLeft.y, windowWidth / m_Scale, 0.f };

	m_pObjectManager->Update( elapsedSec, this, player, camera );
}

void Corridor2Level::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	DrawStaticDrawBackground( camera, windowWidth, windowHeight );
	
	DrawParalexDrawBackground( camera );

	m_pTextureManager->GetTexture( m_pTextureManager->corridor2 )->Draw();

	m_pObjectManager->Draw();
}

Point2f Corridor2Level::GetSpawnLocation(bool rightSpawnLocation) const
{
	Point2f result{};
	if( rightSpawnLocation )
	{
		result.x = 736.f;
		result.y = 48.f;
	}
	else
	{
		result.x = 9.f;
		result.y = 48.f;
	}

	SpawnBoneThrowers();

	return result;
}

void Corridor2Level::DrawParalexDrawBackground(const Point2f& camera) const
{
	const Texture* corridor2{ m_pTextureManager->GetTexture( m_pTextureManager->corridor2 ) };

	const Texture* wallBackground{ m_pTextureManager->GetTexture( m_pTextureManager->wallBackground ) };

	Point2f paralexBackgroundLocation{};

	paralexBackgroundLocation.x = camera.x / ( corridor2->GetWidth() / wallBackground->GetWidth() );

	paralexBackgroundLocation.y = camera.y;

	wallBackground->Draw( paralexBackgroundLocation );
}

void Corridor2Level::DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	const Texture* moonBackground2{ m_pTextureManager->GetTexture( m_pTextureManager->moonBackground2 ) };
	
	moonBackground2->Draw( Rectf( camera.x, camera.y, windowWidth / m_Scale, windowHeight / m_Scale ) );
}

void Corridor2Level::SpawnBoneThrowers() const
{
	m_pObjectManager->ClearAllObjects();

	const Rectf enemieSpawnArea{ 65.f, 16.f, 600.f, 100.f };

	float intervall{ 600.f / 5.f };

	for( int i{ 0 }; i < 5; i++)
	{
		Point2f spawnLocation
		{
			enemieSpawnArea.left + ( intervall * ( i + 1 ) ),
			enemieSpawnArea.bottom
		};
		m_pObjectManager->AddGameObject( new BoneThrower( m_pTextureManager, spawnLocation ) );
	}
}
