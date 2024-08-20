#include "pch.h"
#include "BossRoomLevel.h"
#include "Texture.h"
#include "TextureManager.h"
#include <utils.h>
#include "SVGParser.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"

BossRoomLevel::BossRoomLevel(float scale, TextureManager* textureManager, const std::string& verticesPath):
	Level( textureManager, scale, textureManager->GetTexture( textureManager->bossRoom ), verticesPath )
{
}

void BossRoomLevel::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
	const Rectf camera{ cameraBottemLeft.x, cameraBottemLeft.y, windowWidth / m_Scale, 0 };
	
	m_pObjectManager->Update( elapsedSec, this, player, camera );
}

void BossRoomLevel::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	DrawStaticDrawBackground( camera, windowWidth, windowHeight );
	
	DrawParalexDrawBackground( camera );

	m_pTextureManager->GetTexture( m_pTextureManager->bossRoom )->Draw();

	m_pObjectManager->Draw();
}

Point2f BossRoomLevel::GetSpawnLocation(bool rightSpawnLocation) const
{
	Point2f result{};
	if( rightSpawnLocation )
	{
		result.x = 478.f;
		result.y = 48.f;
	}
	else
	{
		result.x = 9.f;
		result.y = 48.f;
	}
	return result;
}

void BossRoomLevel::DrawParalexDrawBackground(const Point2f& camera) const
{
	const Texture* bossRoom{ m_pTextureManager->GetTexture( m_pTextureManager->bossRoom ) };
	
	const Texture* wallBackground{ m_pTextureManager->GetTexture( m_pTextureManager->wallBackground ) };

	Point2f paralexBackgroundLocation{};

	const float widthDiferenceScaler{ 1.5f };

	if( bossRoom->GetWidth() < wallBackground->GetWidth() )
	{
		float widthDiference{ wallBackground->GetWidth() - bossRoom->GetWidth() };
		
		paralexBackgroundLocation.x = camera.x / ( ( bossRoom->GetWidth() + ( widthDiference * widthDiferenceScaler) ) / wallBackground->GetWidth() );
		
		paralexBackgroundLocation.y = camera.y;
	}

	wallBackground->Draw( paralexBackgroundLocation );
}

void BossRoomLevel::DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	const Texture* moonBackground2{ m_pTextureManager->GetTexture( m_pTextureManager->moonBackground2 ) };
	
	moonBackground2->Draw( Rectf( camera.x, camera.y, windowWidth / m_Scale, windowHeight / m_Scale ) );
}