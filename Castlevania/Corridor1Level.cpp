#include "pch.h"
#include "Corridor1Level.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "Player.h"
#include "WallChandelier.h"
#include "Zombie.h"

Corridor1Level::Corridor1Level(float scale, TextureManager* textureManager, const std::string& verticesPath):
	Level( textureManager, scale, textureManager->GetTexture( textureManager->corridor1 ), verticesPath )
{
}

void Corridor1Level::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
	const Rectf enemieSpawnArea{ 65.f, 16.f, 1150.f, 100.f };
	
	if( ( utils::IsOverlapping( player->GetShape(), enemieSpawnArea ) ) && ( m_pObjectManager->GetNumberOfObjects( ObjectType::zombie ) < 2 ) )
	{
		Level::AddGameObjectToObjectManager
		(
			new Zombie
			(
				enemieSpawnArea,
				m_pTextureManager,
				player->GetShape(),
				cameraBottemLeft,
				windowWidth / m_Scale
			)
		);
	}

	const Rectf camera{ cameraBottemLeft.x, cameraBottemLeft.y, windowWidth / m_Scale, 0.f };

	m_pObjectManager->Update( elapsedSec, this, player, camera );
}

void Corridor1Level::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	DrawStaticDrawBackground( camera, windowWidth, windowHeight );
	
	DrawParalexDrawBackground( camera );
	
	m_pTextureManager->GetTexture( m_pTextureManager->corridor1 )->Draw();
	
	m_pObjectManager->Draw();
}

Point2f Corridor1Level::GetSpawnLocation(bool rightSpawnLocation) const
{
	AddWallChandeliers();
	
	Point2f result{};
	
	if( rightSpawnLocation )
	{
		result.x = 1254.f;
		result.y = 48.f;
	}
	else
	{
		result.x = 9.f;
		result.y = 48.f;
	}

	return result;
}

void Corridor1Level::DrawParalexDrawBackground(const Point2f& camera) const
{
	const Texture* corridor1{ m_pTextureManager->GetTexture( m_pTextureManager->corridor1 ) };
	
	const Texture* wallBackground{ m_pTextureManager->GetTexture( m_pTextureManager->wallBackground ) };

	Point2f paralexBackgroundLocation{};
	
	paralexBackgroundLocation.x = camera.x / ( corridor1->GetWidth() / wallBackground->GetWidth() );
	
	paralexBackgroundLocation.y = camera.y;
	
	wallBackground->Draw( paralexBackgroundLocation );
}

void Corridor1Level::DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	const Texture* moonBackground2{ m_pTextureManager->GetTexture( m_pTextureManager->moonBackground2 ) };
	
	moonBackground2->Draw( Rectf( camera.x, camera.y, windowWidth / m_Scale, windowHeight / m_Scale ) );
}

void Corridor1Level::AddWallChandeliers() const
{
	m_pObjectManager->ClearAllObjects();
	
	const float height{ 81.5f };

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 256.f, height )
		)
	);

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 512.f, height )
		)
	);

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 768.f, height )
		)
	);

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 1024.f, height )
		)
	);
}