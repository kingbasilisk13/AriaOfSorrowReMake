#include "pch.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "Texture.h"
#include "Enemy.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include <iostream>
#include "UIManager.h"

Game::Game( const Window& window ) 
	:BaseGame{ window },
	m_Scale{ GetViewPort().height / Texture{ "Level/Corridor1.png" }.GetHeight()}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pTextureManager = new TextureManager();
	
	m_pAudioManager = new AudioManager();

	m_pLevelManager = new LevelManager(m_Scale, m_pTextureManager, m_pAudioManager);

	m_pPlayer = new Player(m_pTextureManager, m_pAudioManager, GetViewPort().height, m_Scale);

	m_pPlayer->SetLocation(m_pLevelManager->GetCurrentLevel()->GetSpawnLocation(false));

	m_pCamera = new Camera(m_Scale, GetViewPort().width, GetViewPort().height);

	m_pCamera->SetLevelBoundaries(m_pLevelManager->GetCurrentLevel()->GetBoundaries());
	
	m_pUIManager = new UIManager(m_pTextureManager, m_pAudioManager);
}

void Game::Cleanup( )
{
	delete m_pPlayer;
	delete m_pCamera;
	delete m_pLevelManager;
	delete m_pTextureManager;
	delete m_pAudioManager;
	delete m_pUIManager;
}

void Game::Update( float elapsedSec )
{
	if( m_pUIManager->GetStartGame() )
	{
		m_pCamera->Update();
		
		m_pLevelManager->Update( elapsedSec, m_pPlayer, m_pCamera, GetViewPort().width );

		if( !m_pLevelManager->IsChanginLevel() )
		{
			m_pPlayer->Update( elapsedSec, m_pLevelManager->GetCurrentLevel() );
		}
	}
	else
	{
		m_pUIManager->Update( elapsedSec );
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	if( m_pUIManager->GetStartGame() )
	{
		if( m_pLevelManager->IsChanginLevel() )
		{
			m_pTextureManager->GetTexture( m_pTextureManager->loadinScreen )->Draw();
		}
		else if( !m_pLevelManager->IsDeathAnimationDone() )
		{
			glPushMatrix();
			{
				glScalef( m_pCamera->GetScale(), m_pCamera->GetScale(), 1 );

				m_pCamera->Transform( m_pPlayer->GetShape() );

				m_pLevelManager->Draw( m_pCamera, GetViewPort().width, GetViewPort().height, m_pPlayer->GetHealth() );

				m_pPlayer->Draw();
			}
			glPopMatrix();
			
			m_pPlayer->DrawHud();
		}
	}
	else
	{
		glPushMatrix();
		{
			glScalef( m_pCamera->GetScale(), m_pCamera->GetScale(), 1 );

			m_pUIManager->Draw( GetViewPort().width, GetViewPort().height );
		}
		glPopMatrix();
	}

	if( m_pLevelManager->IsDeathAnimationDone() )
	{
		m_pLevelManager->Draw( m_pCamera, GetViewPort().width, GetViewPort().height, m_pPlayer->GetHealth() );
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	if( !m_pLevelManager->IsChanginLevel() )
	{
		switch (e.keysym.sym)
		{
		case SDLK_1:
			m_pAudioManager->PauseSoundStream();
			break;
		case SDLK_2:
			m_pAudioManager->ResumeSoundStream();
			break;
		case SDLK_3:
			m_pAudioManager->DecreaseSoundStreamVolume();
			break;
		case SDLK_4:
			m_pAudioManager->IncreaseSoundStreamVolume();
			break;
		case SDLK_5:
			m_pAudioManager->DecreaseSoundEffectVolume();
			break;
		case SDLK_6:
			m_pAudioManager->IncreaseSoundEffectVolume();
			break;
		}

		if ( m_pPlayer->GetHealth() > 0 )
		{
			switch (e.keysym.sym)
			{
			case SDLK_RETURN:
				m_pUIManager->SetStartGame();
				break;
			}
		}
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_i:
		PrintControles();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintControles() const
{
	std::cout << "Enter: start game\n\n";

	std::cout << "In Game\n";
	std::cout << "left arrow: move left \nright arrow: move right\nZ: jump\nX: melee attack\n";
	std::cout << "Up arrow: ready projectile -> up arrow + X: fire projectile\n";
	std::cout << "Down arrow: crouch\nDown arrow + X: crouch attack\n";
	std::cout << "X while in the air: jump attack\n";
	std::cout << "Q: Back dash (needs to be unlocked)\n";
	std::cout << "P: zoom in\nO: zoom out\n";
	std::cout << "1: pause music\n2: resume music\n3: music volume down\n4: music volume up\n5: soundeffects volume down\n6: soundeffects volume up\n";
	std::cout << "Enter: pause game and open sound settings\n\n";

	std::cout << "In sound settings\n";
	std::cout << "Up arrow: select music volume \nDown arrow: sellect sound effects volume \nLeft arrow: decrease volume \nRight arrow: increase volume\nEnter: close sound settings";
}
