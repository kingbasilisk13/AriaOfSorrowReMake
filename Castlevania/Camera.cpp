#include "pch.h"
#include "Camera.h"
#include <utils.h>

Camera::Camera(float scale, float width, float height) :
	m_Width{ width},
	m_Height{ height},
	m_Scale{ scale },
	m_MaxScale{ scale },
	m_LevelBoundaries{ Rectf(0,0,width,height) },
	m_CameraBottemLeft{Point2f(0,0)}
{
	const float freeAreaSize{ 50 };

	m_FreeArea.width = freeAreaSize;
	
	m_FreeArea.height = freeAreaSize;
	
	m_FreeArea.left = ( width / scale ) / 2 - ( m_FreeArea.width / 2 );
	
	m_FreeArea.bottom = ( height / scale) / 2 - ( m_FreeArea.height / 2 );
	
}

void Camera::Update()
{
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );

	if( pStates[SDL_SCANCODE_P] )
	{
		ZoomIn( 1 );
	}
	else if( pStates[SDL_SCANCODE_O] )
	{
		ZoomOut( 1 );
	}
}

void Camera::SetLevelBoundaries( const Rectf& levelBoundaries )
{
	m_LevelBoundaries = levelBoundaries;
}


void Camera::Transform(const Rectf& target)
{
	UpdareFreeArea();
	m_CameraBottemLeft = Track( target );
	Clamp( m_CameraBottemLeft );
	glTranslatef( -m_CameraBottemLeft.x, -m_CameraBottemLeft.y, 0 );
}

const Point2f& Camera::GetCameraBottemLeft() const
{
	return m_CameraBottemLeft;
}

float Camera::GetScale() const
{
	return m_Scale;
}

const Point2f& Camera::Track(const Rectf& target)
{
	if( target.left <= m_FreeArea.left )
	{
		m_CameraBottemLeft.x -= m_FreeArea.left - target.left;
	}

	if( ( target.left + target.width ) >= ( m_FreeArea.left + m_FreeArea.width ) )
	{
		m_CameraBottemLeft.x += ( target.left + target.width ) - ( m_FreeArea.left + m_FreeArea.width );
	}

	if( target.bottom <= m_FreeArea.bottom )
	{
		m_CameraBottemLeft.y -=  m_FreeArea.bottom - target.bottom;
	}

	if( ( target.bottom + target.height ) >= ( m_FreeArea.bottom + m_FreeArea.height ) )
	{
		m_CameraBottemLeft.y += ( target.bottom + target.height ) - ( m_FreeArea.bottom + m_FreeArea.height );
	}

	return m_CameraBottemLeft;
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	if( bottomLeftPos.x < m_LevelBoundaries.left )
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	if( bottomLeftPos.x + ( m_Width/m_Scale ) > m_LevelBoundaries.left + m_LevelBoundaries.width )
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - ( m_Width / m_Scale );
	}

	if(bottomLeftPos.y < m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	if( ( bottomLeftPos.y + (m_Height/m_Scale) ) > ( m_LevelBoundaries.bottom + m_LevelBoundaries.height ) )
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - ( m_Height / m_Scale );
	}
}

void Camera::UpdareFreeArea()
{
	m_FreeArea.left = m_CameraBottemLeft.x + ( m_Width / m_Scale ) / 2.f - ( m_FreeArea.width / 2.f );
	m_FreeArea.bottom = m_CameraBottemLeft.y + ( m_Height / m_Scale ) / 2.f - ( m_FreeArea.height / 2.f );
}

void Camera::ZoomIn(float value)
{
	const float zoomSpeed{ 90.f };
	m_Scale += m_Scale / zoomSpeed;

	m_CameraBottemLeft.x = m_FreeArea.left + m_FreeArea.width / 2.f - ( m_Width / m_Scale ) / 2.f;
	m_CameraBottemLeft.y = m_FreeArea.bottom + m_FreeArea.width / 2.f - ( m_Width / m_Scale ) / 2.f;
}

void Camera::ZoomOut(float value)
{
	const float zoomSpeed{ 90.f };
	float scaleValue = m_Scale / zoomSpeed;

	if( ( m_Scale - scaleValue ) < m_MaxScale )
	{
		m_Scale = m_MaxScale;
		return;
	}

	m_Scale -= scaleValue;

	m_CameraBottemLeft.x = m_FreeArea.left + m_FreeArea.width / 2.f - ( m_Width / m_Scale ) / 2.f;
	
	m_CameraBottemLeft.y = m_FreeArea.bottom + m_FreeArea.width / 2.f - ( m_Width / m_Scale ) / 2.f;
}