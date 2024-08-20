#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include <utils.h>
#include "SVGParser.h"
#include "Player.h"
#include "ObjectManager.h"
#include "TextureManager.h"

Level::Level(TextureManager* textureManager, float scale, const Texture* background, const std::string& verticesPath) :
	m_Scale{scale},
	m_pObjectManager{ new ObjectManager(textureManager) },
	m_pTextureManager{ textureManager }
{
	SVGParser().GetVerticesFromSvgFile(verticesPath, m_Vertices);

	if( background )
	{
		m_Boundaries = Rectf
		(	
			0,
			0,
			background->GetWidth(),
			background->GetHeight()
		);
	}
}

Level::~Level()
{
	delete m_pObjectManager;
}

#pragma region collision handeling
void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	utils::HitInfo hitInfo{};

	for( size_t i{ 0 }; i < m_Vertices.size(); ++i )
	{
		if( utils::Raycast( m_Vertices[i],  CalculateTopCenterActor( actorShape ), CalculateBottemCenterActor( actorShape ), hitInfo ) )
		{
			float dotProduct = actorVelocity.DotProduct( hitInfo.normal );
			if( dotProduct < 0 )
			{
				if( actorVelocity.y < 0 )
				{
					actorShape.bottom = hitInfo.intersectPoint.y;
					actorVelocity.y = 0;
				}
				else if( actorVelocity.y > 0 )
				{
					actorShape.bottom = hitInfo.intersectPoint.y - actorShape.height;
					actorVelocity.y = 0;
				}
			}
		}
		
		if( utils::Raycast( m_Vertices[i],  CalculateLeftCenterActor( actorShape ), CalculateRightCenterActor( actorShape ), hitInfo ) )
		{
			float dotProduct = actorVelocity.DotProduct( hitInfo.normal );

			if( dotProduct < 0 )
			{
				if( actorVelocity.x < 0 )
				{
					actorShape.left = hitInfo.intersectPoint.x;
					actorVelocity.x = 0;
				}
				else if( actorVelocity.x > 0 )
				{
					actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
					actorVelocity.x = 0;
				}	
			}
		}

		if( utils::Raycast( m_Vertices[i], CalculateTopLeft( actorShape ), CalculateTopRight( actorShape ), hitInfo ) )
		{
			float dotProduct = actorVelocity.DotProduct( hitInfo.normal );

			if( dotProduct < 0 )
			{
				if( actorVelocity.x < 0 )
				{
					actorShape.left = hitInfo.intersectPoint.x;
					actorVelocity.x = 0;
				}
				else if( actorVelocity.x > 0 )
				{
					actorShape.left = hitInfo.intersectPoint.x - actorShape.width;
					actorVelocity.x = 0;
				}
			}
		}
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	Point2f centerActor{ CalculateCenterActor( actorShape ) };

	Point2f bottemCenterActor{ CalculateBottemCenterActor( actorShape ) };

	const float movePointDown{ 5.f };

	bottemCenterActor.y -= movePointDown;

	utils::HitInfo hitInfo{};

	for( size_t i{ 0 }; i < m_Vertices.size(); ++i )
	{
		if( utils::Raycast( m_Vertices[i], centerActor, bottemCenterActor, hitInfo ) )
		{
			return true;
		}
	}
	return false;
}

bool Level::IsPlayerFalling(const Rectf& actorShape) const
{
	Point2f centerActor{ CalculateCenterActor( actorShape ) };

	Point2f bottemCenterActor{ CalculateBottemCenterActor( actorShape ) };

	bottemCenterActor.y -= actorShape.height / 2.f;

	utils::HitInfo hitInfo{};

	for( size_t i{ 0 }; i < m_Vertices.size(); ++i )
	{
		if( utils::Raycast( m_Vertices[i], centerActor, bottemCenterActor, hitInfo ) )
		{
			return false;
		}
	}
	return true;
}

bool Level::DoesProjectileHitCollision(const Point2f& spearBack, const Point2f& spearFront) const
{
	utils::HitInfo hitInfo{};

	for( size_t i{ 0 }; i < m_Vertices.size(); ++i )
	{
		if( utils::Raycast( m_Vertices[i], spearBack, spearFront, hitInfo ) )
		{
			return true;
		}
	}
	return false;
}

const Rectf& Level::GetBoundaries() const
{
	return m_Boundaries;
}
#pragma endregion

#pragma region Level transition

bool Level::ReachedEndLeft(const Rectf& actorShape) const
{
	const Rectf leftExit
	{ 
		m_Boundaries.left,
		m_Boundaries.bottom, 
		2.f,
		m_Boundaries.height
	};

	if( utils::IsOverlapping( actorShape, leftExit ) )
	{
		return true;
	}
	return false;
}

bool Level::ReachedEndRight(const Rectf& actorShape) const
{
	const Rectf rightExit
	{ 
		m_Boundaries.left + m_Boundaries.width - 2.f,
		m_Boundaries.bottom, 
		2.f,
		m_Boundaries.height 
	};

	if( utils::IsOverlapping( actorShape, rightExit ) )
	{
		return true;
	}
	return false;
}

void Level::HandlePlayerAttack(const Rectf& attackShape, int damage) const
{
	m_pObjectManager->AttackObject( attackShape, damage );
}

void Level::HandlePlayerAttack(const Point2f& pointA, const Point2f& pointB, int damage) const
{
	m_pObjectManager->AttackObject( pointA, pointB, damage );
}
#pragma endregion

#pragma region ActorPointCalculations

Point2f Level::CalculateCenterActor(const Rectf& actorShape) const
{
	Point2f centerActor{};

	centerActor.x = actorShape.left + ( actorShape.width / 2.f );
	
	centerActor.y = actorShape.bottom + ( actorShape.height / 2.f );
	
	return centerActor;
}

Point2f Level::CalculateBottemCenterActor(const Rectf& actorShape) const
{
	Point2f bottemCenterActor{};
	
	bottemCenterActor.x = actorShape.left + ( actorShape.width / 2.f );
	
	bottemCenterActor.y = actorShape.bottom;
	
	return bottemCenterActor;
}

void Level::AddGameObjectToObjectManager(GameObject* gameObject) const
{
	m_pObjectManager->AddGameObject( gameObject );
}

Point2f Level::CalculateTopCenterActor(const Rectf& actorShape) const
{
	Point2f topCenterActor{};
	
	topCenterActor.x = actorShape.left + ( actorShape.width / 2.f );
	
	topCenterActor.y = actorShape.bottom + actorShape.height;
	
	return topCenterActor;
}

Point2f Level::CalculateLeftCenterActor(const Rectf& actorShape) const
{
	Point2f leftCenterActor{};

	leftCenterActor.x = actorShape.left;
	
	leftCenterActor.y = actorShape.bottom + ( actorShape.height / 2.f );
	
	return leftCenterActor;
}

Point2f Level::CalculateRightCenterActor(const Rectf& actorShape) const
{
	Point2f rightCenterActor{};

	rightCenterActor.x = actorShape.left + actorShape.width;
	
	rightCenterActor.y = actorShape.bottom + ( actorShape.height / 2.f );
	
	return rightCenterActor;
}

Point2f Level::CalculateTopLeft(const Rectf& actorShape) const
{
	Point2f TopLeft{};

	TopLeft.x = actorShape.left;
	
	TopLeft.y = actorShape.bottom + actorShape.height;
	
	return TopLeft;
}

Point2f Level::CalculateTopRight(const Rectf& actorShape) const
{
	Point2f TopRight{};
	
	TopRight.x = actorShape.left + actorShape.width;
	
	TopRight.y = actorShape.bottom + actorShape.height;
	
	return TopRight;
}

#pragma endregion