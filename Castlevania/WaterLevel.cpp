#include "pch.h"
#include "WaterLevel.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "GraveKeeperBackDash.h"
#include "Texture.h"
#include "utils.h"
#include "WallChandelier.h"
#include "Player.h"

WaterLevel::WaterLevel(float scale, TextureManager* textureManager, const std::string& verticesPath) :
	Level{ textureManager, scale,textureManager->GetTexture(textureManager->waterCrossing) ,verticesPath}
{
	m_pObjectManager->AddGameObject
	(
		new GraveKeeperBackDash
		(
			m_pTextureManager,
			Point2f( 252.f, 79.f )
		)
	);

	m_pPlatforms.push_back( Rectf{ 192.f, 65.f, 129.f, 15.f } );
	
	m_pPlatforms.push_back( Rectf{ 448.f, 118.f, 33.f, 10.f } );
}

void WaterLevel::Update(float elapsedSec, Player* player, const Point2f& cameraBottemLeft, float windowWidth)
{
	const Rectf camera{ cameraBottemLeft.x, cameraBottemLeft.y, windowWidth / m_Scale, 0.f };

	m_pObjectManager->Update( elapsedSec, this, player, camera );

	Rectf playerShape{ player->GetShape() };

	Point2f playerCenter
	{ 
		playerShape.left + ( playerShape.width / 2.f ),
		playerShape.bottom + ( playerShape.height * 0.8f )
	};

	if( utils::IsPointInRect( playerCenter, m_PoolOfWater ) )
	{
		player->SetVelocity( Vector2f{} );
		player->SetIsSwimming( true );
	}
	else
	{
		player->SetIsSwimming( false );
	}
}

void WaterLevel::DrawBackground(const Point2f& camera, float windowWidth, float windowHeight) const
{
	m_pTextureManager->GetTexture( m_pTextureManager->waterCrossing )->Draw();
	
	m_pObjectManager->Draw();

	utils::SetColor( Color4f{ 0.f, 0.f, 1.f, 0.5f });
	
	utils::FillRect(m_PoolOfWater);
}

Point2f WaterLevel::GetSpawnLocation(bool rightSpawnLocation) const
{
	AddWallChandeliers();

	Point2f result{};
	
	if( rightSpawnLocation )
	{
		result.x = 479.f;
		result.y = 303.f;
	}
	else
	{
		result.x = 15.f;
		result.y = 303.f;
	}
	return result;
}

void WaterLevel::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Level::HandleCollision( actorShape, actorVelocity );

	Point2f centerActor{ CalculateCenterActor( actorShape ) };
	
	Point2f bottemCenterActor{ CalculateBottemCenterActor( actorShape ) };

	const float movePointDown{ 5.f };

	bottemCenterActor.y -= movePointDown;

	utils::HitInfo hitInfo{};

	for( int i{ 0 }; i < m_pPlatforms.size(); i++ )
	{
		if( utils::Raycast( CalculateVertices( m_pPlatforms[i] ), centerActor, bottemCenterActor, hitInfo ) )
		{
			float dotProduct = actorVelocity.DotProduct(hitInfo.normal);

			if( dotProduct < 0 )
			{
				if( actorVelocity.y < 0 )
				{
					actorShape.bottom = hitInfo.intersectPoint.y;
					actorVelocity.y = 0.f;
				}
			}
		}
	}
}

bool WaterLevel::IsOnGround(const Rectf& actorShape) const
{
	bool result = Level::IsOnGround( actorShape );

	Point2f centerActor{ CalculateCenterActor( actorShape ) };
	
	Point2f bottemCenterActor{ CalculateBottemCenterActor( actorShape ) };

	const float movePointDown{ 5.f };

	bottemCenterActor.y -= movePointDown;

	utils::HitInfo hitInfo{};
	
	for( int i{ 0 }; i < m_pPlatforms.size(); i++ )
	{
		if( utils::Raycast( CalculateVertices( m_pPlatforms[i] ), centerActor, bottemCenterActor, hitInfo ) )
		{
			result = true;
		}
	}
	return result;
}

const std::vector<Point2f> WaterLevel::CalculateVertices(const Rectf& platform) const
{
	std::vector<Point2f> vertices;
	
	Point2f corner{};

	corner.x = platform.left;
	corner.y = platform.bottom + platform.height;
	vertices.push_back( corner );

	corner.x = platform.left + platform.width;
	vertices.push_back( corner );

	return vertices;
}

void WaterLevel::AddWallChandeliers() const
{
	m_pObjectManager->ClearAllObjects();

	const float height{ 200.f };

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 55.f, height )
		)
	);

	Level::AddGameObjectToObjectManager
	(
		new WallChandelier
		(
			m_pTextureManager,
			Point2f( 443.f, height )
		)
	);
}
