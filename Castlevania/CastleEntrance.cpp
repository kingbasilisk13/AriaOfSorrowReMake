#include "pch.h"
#include "CastleEntrance.h"
#include "Texture.h"
#include "TextureManager.h"

CastleEntrance::CastleEntrance(float scale, TextureManager* textureManager , const std::string& verticesPath) :
	Level{ textureManager, scale,textureManager->GetTexture(textureManager->castleEntrance) ,verticesPath}
{
}

void CastleEntrance::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
}

void CastleEntrance::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	DrawStaticDrawBackground( camera, windowWidth, windowHeight );
	
	m_pTextureManager->GetTexture( m_pTextureManager->castleEntrance )->Draw();
}

Point2f CastleEntrance::GetSpawnLocation(bool rightSpawnLocation) const
{
	Point2f result{};

	if( rightSpawnLocation )
	{
		result.x = 484.f;
		result.y = 48.f;
	}
	else
	{
		result.x = 85.f;
		result.y = 16.f;
	}

	return result;
}

void CastleEntrance::DrawStaticDrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	m_pTextureManager->GetTexture( m_pTextureManager->moonBackground1 )->Draw( Rectf( camera.x, camera.y, windowWidth / m_Scale, windowHeight / m_Scale ) );
}