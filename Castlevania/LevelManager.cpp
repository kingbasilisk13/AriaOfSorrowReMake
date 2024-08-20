#include "pch.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Camera.h"
#include "Texture.h"

#include "CastleEntrance.h"
#include "Corridor1Level.h"
#include "WaterLevel.h"
#include "Corridor2Level.h"
#include "StoneHallwayLevel.h"
#include "BossRoomLevel.h"

LevelManager::LevelManager(const float scale, TextureManager* textureManager, AudioManager* audioManager):
	m_pTextureManager{ textureManager },
	m_pAudioManager{ audioManager }
{
	m_pLevels.push_back( new CastleEntrance( scale, m_pTextureManager, "Level/CastleEntrance.svg" ) );

	m_pLevels.push_back( new Corridor1Level( scale, m_pTextureManager, "Level/Corridor1.svg" ) );

	m_pLevels.push_back( new WaterLevel( scale, m_pTextureManager, "Level/WaterCrossing.svg" ) );
						 
	m_pLevels.push_back( new Corridor2Level( scale, m_pTextureManager, "Level/Corridor2.svg" ) );
						 
	m_pLevels.push_back( new StoneHallwayLevel( scale, m_pTextureManager, "Level/StoneHallway.svg" ) );
						 
	m_pLevels.push_back( new BossRoomLevel( scale, m_pTextureManager, "Level/BossRoom.svg" ) );	
}

LevelManager::~LevelManager()
{
	for ( int index{ 0 }; index < m_pLevels.size(); index++ )
	{
		delete m_pLevels[index];
		m_pLevels[index] = nullptr;
	}

	m_pLevels.clear();
}

void LevelManager::Update(float elapsedSec, Player* player, Camera* camera, const float viewportWidth)
{
	const float timeLevelTransition{ 1.f };
	const float deathTransitionTime{ 5.f };

	if( player->GetHealth() <= 0 )
	{
		m_TransitionTime += elapsedSec;
	}

	if( ( !m_DeathAnimationDone ) && ( m_TransitionTime >= deathTransitionTime ) )
	{
		m_DeathAnimationDone = true;
		m_TransitionTime = 0;
	}

	if( m_ChanginLevel )
	{
		m_TransitionTime += elapsedSec;
		if( m_TransitionTime >= timeLevelTransition )
		{
			if( m_CurrentLevel > m_PreviusLevel )
			{
				ChangeLevel( false, player, camera );
			}
			else
			{
				ChangeLevel( true, player, camera );
			}

			m_PreviusLevel = m_CurrentLevel;
			
			m_ChanginLevel = false;
			
			m_TransitionTime = 0;
		}
	}
	else if( !m_DeathAnimationDone )
	{
		m_pLevels[m_CurrentLevel]->Update( elapsedSec, player, camera->GetCameraBottemLeft(), viewportWidth );

		HasPlayerReachedEndLevel( player );
	}
}

void LevelManager::Draw(const Camera* camera, const float viewportWidth, const float viewportHeight, float playerHealth) const
{
	const float deathTransitionTime{ 5.f };

	const Texture* gameOverScreen{ m_pTextureManager->GetTexture( m_pTextureManager->gameOverScreen ) };

	if( m_DeathAnimationDone )
	{
		gameOverScreen->Draw( Rectf( 0, 0, viewportWidth, viewportHeight ) );
		
		m_pAudioManager->PlaySoundStream( m_pAudioManager->gameOver );
		
		float alphaChanel = 1 - ( 1.f / deathTransitionTime ) * m_TransitionTime;
		
		utils::SetColor( Color4f( 0, 0, 0, alphaChanel ) );
		
		utils::FillRect( Rectf( 0, 0, viewportWidth, viewportHeight ) );
	}
	else 
	{
		m_pLevels[m_CurrentLevel]->DrawBackground( camera->GetCameraBottemLeft(), viewportWidth, viewportHeight );
		if (playerHealth <= 0)
		{
			m_pAudioManager->PauseSoundStream();

			float alphaChanel = ( 1.f / deathTransitionTime ) * m_TransitionTime;
			
			utils::SetColor( Color4f( 0, 0, 0, alphaChanel ) );
			
			utils::FillRect( Rectf( 0, 0, viewportWidth, viewportHeight ) );
		}
	}
}

const Level* LevelManager::GetCurrentLevel() const
{
	return m_pLevels[m_CurrentLevel];
}

bool LevelManager::IsChanginLevel() const
{
	return m_ChanginLevel;
}

bool LevelManager::IsDeathAnimationDone() const
{
	return m_DeathAnimationDone;
}

float LevelManager::GetTransitionTimer() const
{
	return m_TransitionTime;
}

void LevelManager::HasPlayerReachedEndLevel(Player* player)
{
	if( m_pLevels[m_CurrentLevel]->ReachedEndRight( player->GetShape() ) )
	{
		if(int( m_pLevels.size()) > ( m_CurrentLevel + 1 ) )
		{
			++m_CurrentLevel;
			m_ChanginLevel = true;
		}
	}
	else if( m_pLevels[m_CurrentLevel]->ReachedEndLeft( player->GetShape() ) )
	{
		if ( ( m_CurrentLevel - 1 ) > -1 )
		{
			--m_CurrentLevel;
			m_ChanginLevel = true;
		}
	}
}

void LevelManager::ChangeLevel(bool rightSpawnLocation, Player* player, Camera* camera)
{
	player->SetLocation( m_pLevels[m_CurrentLevel]->GetSpawnLocation( rightSpawnLocation ) );
	camera->SetLevelBoundaries( m_pLevels[m_CurrentLevel]->GetBoundaries() );
}