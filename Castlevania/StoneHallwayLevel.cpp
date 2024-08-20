#include "pch.h"
#include "StoneHallwayLevel.h"
#include "Texture.h"
#include "TextureManager.h"
#include <utils.h>
#include "SVGParser.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Enemy.h"
#include "WallChandelier.h"

StoneHallwayLevel::StoneHallwayLevel(float scale, TextureManager* textureManager, const std::string& verticesPath):
	Level{ textureManager, scale, textureManager->GetTexture( textureManager->stoneHallway ), verticesPath }
{
}

void StoneHallwayLevel::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
	const Rectf camera{ cameraBottemLeft.x, cameraBottemLeft.y, windowWidth / m_Scale, 0.f };

	m_pObjectManager->Update( elapsedSec, this, player, camera );
}

void StoneHallwayLevel::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	const Texture* moonBackground1{ m_pTextureManager->GetTexture( m_pTextureManager->moonBackground1 ) };
	
	moonBackground1->Draw( Rectf( camera.x, camera.y, windowWidth / m_Scale, windowHeight / m_Scale ) );

	m_pTextureManager->GetTexture( m_pTextureManager->stoneHallway )->Draw();

	m_pObjectManager->Draw();
}

Point2f StoneHallwayLevel::GetSpawnLocation(bool rightSpawnLocation) const
{
	Point2f result{};

	if( rightSpawnLocation )
	{
		result.x = 484.f;
		result.y = 48.f;
	}
	else
	{
		result.x = 9.f;
		result.y = 48.f;
	}
	return result;
}