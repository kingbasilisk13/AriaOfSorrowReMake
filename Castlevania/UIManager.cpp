#include "pch.h"
#include "UIManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "Texture.h"
#include "utils.h"

UIManager::UIManager(TextureManager* textureManager, AudioManager* audioManager):
	m_pTextureManager{ textureManager },
	m_pAudioManager{ audioManager }
{
	audioManager->PlaySoundStream( audioManager->titleTheme, true );

	const Texture* music{ textureManager->GetTexture( textureManager->music ) };
	const Texture* sound{ textureManager->GetTexture( textureManager->sound ) };

	const Point2f locationMusic
	{
		100.f,
		100.f
	};

	const Point2f locationSound
	{
		100.f,
		50.f
	};

	m_MusicSlider = Rectf{};
	m_MusicSlider.width = 200.f;
	m_MusicSlider.height = 5.f;
	m_MusicSlider.left = locationMusic.x + music->GetWidth() / 2.f - m_MusicSlider.width / 2.f;
	m_MusicSlider.bottom = ( locationMusic.y - m_MusicSlider.height * 2.f );


	m_SoundSlider = Rectf{};
	m_SoundSlider.width = 200.f;
	m_SoundSlider.height = 5.f;
	m_SoundSlider.left = locationSound.x + sound->GetWidth() / 2.f - m_SoundSlider.width / 2.f;
	m_SoundSlider.bottom = locationSound.y - m_SoundSlider.height * 2.f;
}

void UIManager::Update(float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );

	if( !m_ShowTitleScreen )
	{
		ChangeVolume( pStates );
	}

	m_AnimTime += elapsedSec;
	UpdateAnimation();
	m_AnimationSource.width = 240.f;
	m_AnimationSource.height = 160.f;
	m_AnimationSource.bottom = 0.f;
	m_AnimationSource.left = m_AnimationSource.width * m_AnimFrame;

}

void UIManager::Draw(float width, float height) const
{
	const Texture* introScreenBackground{ m_pTextureManager->GetTexture( m_pTextureManager->introScreenBackground ) };
	
	utils::SetColor( Color4f{ 8.f/255, 8.f/255, 176.f/255, 1.f } );
	
	utils::FillRect( Point2f{}, width, height );
	
	introScreenBackground->Draw( Point2f{}, m_AnimationSource );
	
	if( m_ShowTitleScreen )
	{
		DrawTitleScreen( width, height );
	}
	else
	{
		DrawVolumeOptions( width, height );
	}
}

bool UIManager::GetStartGame() const
{
	return m_StartGame;
}

void UIManager::SetStartGame()
{
	m_ShowTitleScreen = false;
	m_StartGame = !m_StartGame;

	if( m_StartGame )
	{
		m_pAudioManager->PlaySoundStream( m_pAudioManager->ruinedCastleCorridor, true );
	}
	else
	{
		m_pAudioManager->PlaySoundStream( m_pAudioManager->nameEntry, true );
	}
}

void UIManager::UpdateAnimation()
{
	if( m_AnimTime >= 1.f / m_NrFramesPerSec )   
	{
		m_AnimTime = 0;
		++m_AnimFrame;
		if( m_AnimFrame >= m_NrOfFrames )
		{
			m_AnimFrame = 0;
		}
	}
}

void UIManager::DrawTitleScreen(float width, float height) const
{
	const Texture* titleScreenLogo{ m_pTextureManager->GetTexture( m_pTextureManager->titleScreenLogo ) };

	const Texture* instructions{ m_pTextureManager->GetTexture( m_pTextureManager->instructions ) };
	
	glPushMatrix();
	{
		const float scaleDown{ 0.2f };
		glScalef( scaleDown, scaleDown, 1);

		const Point2f titleLocation
		{
			( width / 2.f ) - ( titleScreenLogo->GetWidth() / 2.f ),
			( height / 2.f ) - ( titleScreenLogo->GetHeight() / 2.f )
		};

		titleScreenLogo->Draw( titleLocation );

		const Point2f instructionsLocation
		{
			500,200
		};

		instructions->Draw( instructionsLocation );
	}
	glPopMatrix();
}

void UIManager::DrawVolumeOptions(float width, float height) const
{
	const Texture* music{ m_pTextureManager->GetTexture( m_pTextureManager->music ) };
	const Texture* sound{ m_pTextureManager->GetTexture( m_pTextureManager->sound ) };
	const Texture* pointer{ m_pTextureManager->GetTexture( m_pTextureManager->pointer ) };

	const Point2f locationMusic
	{
		100,
		100
	};

	const Point2f locationSound
	{
		100,
		50
	};

	music->Draw( locationMusic );
	sound->Draw( locationSound );

	if( m_MusicSelected )
	{
		pointer->Draw(
			Point2f
			{
				locationMusic.x - pointer->GetWidth(),
				locationMusic.y
			}
		);
	}
	else
	{
		pointer->Draw(
			Point2f
			{
				locationSound.x - pointer->GetWidth(),
				locationSound.y
			}
		);
	}


	utils::SetColor( Color4f{ 0,0,0, 1.f} );
	utils::FillRect( m_MusicSlider );
	utils::FillRect( m_SoundSlider );


	utils::SetColor( Color4f{ 1, 1, 1, 1.f } );

	Rectf volumeMarker{};
	volumeMarker.width = m_MusicSlider.height;
	volumeMarker.height = m_MusicSlider.height * 2.f;

	const float markerLeft{ volumeMarker.width / 2.f };
	const float markerBottem{ volumeMarker.height / 4.f };

	const float musicPercentage{ m_MusicSlider.width * ( m_pAudioManager->GetSoundStreamVolume() / float( m_pAudioManager->GetMaxMusicVolume() ) ) };
	volumeMarker.left = m_MusicSlider.left + musicPercentage - markerLeft;
	volumeMarker.bottom = m_MusicSlider.bottom - markerBottem;
	
	utils::FillRect(volumeMarker);

	const float soundPercentage{ m_SoundSlider.width * ( m_pAudioManager->GetSoundEffectVolume() / float( m_pAudioManager->GetMaxSoundeffectVolume() ) ) };
	volumeMarker.left = m_SoundSlider.left + soundPercentage - markerLeft;
	volumeMarker.bottom = m_SoundSlider.bottom - markerBottem;

	utils::FillRect(volumeMarker);
}

void UIManager::ChangeVolume(const Uint8* pStates)
{
	if( pStates[SDL_SCANCODE_UP] )
	{
		m_MusicSelected = true;
	}
	else if( pStates[SDL_SCANCODE_DOWN] )
	{
		m_MusicSelected = false;
	}

	if( pStates[SDL_SCANCODE_RIGHT] )
	{
		if( m_MusicSelected )
		{
			m_pAudioManager->IncreaseSoundStreamVolume();
		}
		else
		{
			m_pAudioManager->IncreaseSoundEffectVolume();
		}
	}
	else if( pStates[SDL_SCANCODE_LEFT] )
	{
		if( m_MusicSelected )
		{
			m_pAudioManager->DecreaseSoundStreamVolume();
		}
		else
		{
			m_pAudioManager->DecreaseSoundEffectVolume();
		}
	}
}
