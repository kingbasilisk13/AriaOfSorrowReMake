#include "pch.h"
#include "Player.h"
#include "Level.h"
#include "Texture.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "Hud.h"
#include "PlayerProjectileSpear.h"

Player::Player( TextureManager* textureManager, AudioManager* audioManager, const float viewportHeight, const float scale):
	m_pTextureManager{ textureManager },
	m_pAudioManager{ audioManager },
	m_pHud{ new Hud( textureManager, Point2f( 0, viewportHeight ), scale ) }
{
	const Texture* playerBaseShape{ m_pTextureManager->GetTexture( m_pTextureManager->playerBaseShape ) };
	
	if( playerBaseShape )
	{
		m_PlayerLocation.width = playerBaseShape->GetWidth();
		m_PlayerLocation.height = playerBaseShape->GetHeight();
	}
	m_pSpear = nullptr;
}

Player::~Player()
{
	if( m_pSpear != nullptr )
	{
		delete m_pSpear;
	}
	delete m_pHud;
}

void Player::Update(float elapsedSec, const Level *level)
{
	const Uint8* pStates = SDL_GetKeyboardState( nullptr );

	m_pHud->Update( m_Health, m_MaxHealth, m_Mana, m_MaxMana);

	UpdateProjectile( elapsedSec, level );

	if( m_Health > 0 )
	{
		if( m_Mana < m_MaxMana )
		{
			const float timeBeforeStartRegen{ 5.f };
			const float manaRegainedPerSecond{ 2.f };

			if( m_ManaRegenTimer < timeBeforeStartRegen )
			{
				m_ManaRegenTimer += elapsedSec;
			}
			else
			{
				m_Mana += manaRegainedPerSecond * elapsedSec;
			}
		}

		UpdateInvincibility( elapsedSec );

		m_Velocity.x = 0;
		
		HandleKeyboardInput( pStates, level );
		
		ChangeActionState( level, pStates );

		FireProjectile( pStates );

		PerformMeleeAttack( level );
		
		HandleMovement( elapsedSec, level, false );
	}
	else
	{
		PerformDeathAnimation(elapsedSec, level);
	}

	UpdateActionAnimationValues();

	UpdateAnimationFrame( elapsedSec );
}

void Player::Draw() const
{
	glPushMatrix();
	{
		glTranslatef
		( 
			m_PlayerLocation.left, 
			m_PlayerLocation.bottom, 
			0
		);

		DrawPlayer();
		
		DrawBloodParticles();
	}
	glPopMatrix();

	DrawProjectile();
}

void Player::DrawHud() const
{
	m_pHud->Draw();
}

#pragma region Getters

const Rectf& Player::GetShape() const
{
	return m_PlayerLocation;
}

Rectf Player::GetHitbox() const
{
	Rectf hitbox{};
	
	hitbox.width = m_PlayerLocation.width / 2.f;
	
	hitbox.height = m_PlayerLocation.height / 2.f;

	const float moveHitboxRight{ 3.f };

	hitbox.left = ( m_PlayerLocation.left + ( hitbox.width / 2.f ) ) + moveHitboxRight;
	
	hitbox.bottom = m_PlayerLocation.bottom + ( hitbox.height / 2.f );

	return hitbox;
}

float Player::GetHealth() const
{
	return m_Health;
}
#pragma endregion


#pragma region Setters
void Player::SetLocation(const Point2f& spawnLocation)
{
	m_PlayerLocation.left = spawnLocation.x;
	
	m_PlayerLocation.bottom = spawnLocation.y;
}

void Player::TakeDamage(int damage, bool damagefFromTheLeft)
{
	if ( ( !m_HasPlayerBeenHit ) && ( ( m_ActionState != ActionState::backDashMoving ) && ( m_ActionState != ActionState::backDashStoping ) ) )
	{
		m_Health -= damage;

		m_ChangeAnimation = false;
		
		m_HasPlayerBeenHit = true;

		if ( damagefFromTheLeft == m_LookingLeft )
		{
			m_ActionState = ActionState::takeDamageFromFront;
		}
		else
		{
			m_ActionState = ActionState::takeDamageFromBack;
		}

		if( m_Health > 0 )
		{
			int randomNumber{ rand() % 11 };

			if (randomNumber >= 5)
			{
				m_pAudioManager->PlaySoundEffect( m_pAudioManager->damageSound1 );
			}
			else
			{
				m_pAudioManager->PlaySoundEffect( m_pAudioManager->damageSound2 );
			}
		}
		else
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->deathScream );
		}
	}
}

void Player::AddCoinValue(int valueGold)
{
	m_Coins += valueGold;
}

void Player::UnlockBackDash()
{
	m_HasBackDash = true;
}

void Player::SetVelocity(const Vector2f& velocity)
{
	if(m_Velocity.y < 0)
	{
		m_Velocity.y = velocity.y;
	}
}

void Player::SetIsSwimming(bool isSwimming)
{
	m_IsSwimming = isSwimming;
}

#pragma endregion

#pragma region HandleMovement

void Player::HandleKeyboardInput(const Uint8* pStates, const Level* level)
{
	if( m_MoveDuringAnimation )
	{
		if( m_ActionState == ActionState::backDashMoving )
		{
			const float backDashDistance{ 200 };
			if( m_LookingLeft )
			{
				m_Velocity.x = backDashDistance;
			}
			else
			{
				m_Velocity.x = -backDashDistance;
			}
		}
		else if( pStates[SDL_SCANCODE_RIGHT] )
		{
			m_Velocity.x = m_HorSpeed;
			m_LookingLeft = false;
		}
		else if( pStates[SDL_SCANCODE_LEFT] )
		{
			m_Velocity.x = -m_HorSpeed;
			m_LookingLeft = true;
		}

		if( ( pStates[SDL_SCANCODE_Z] ) && ( level->IsOnGround(m_PlayerLocation) || m_IsSwimming ) )
		{
			m_Velocity.y = m_JumpSpeed;
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->jumpUp );
		}
	}
}

void Player::HandleMovement(float elapsedSec, const Level* level, bool ignoreGravity)
{
	if( !ignoreGravity )
	{
		if( !m_IsSwimming ) 
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
		}
	}

	level->HandleCollision( m_PlayerLocation, m_Velocity );
	
	m_PlayerLocation.left += m_Velocity.x * elapsedSec;
	
	m_PlayerLocation.bottom += m_Velocity.y * elapsedSec;
}

#pragma endregion

#pragma region HandleAnimation

void Player::ChangeActionState(const Level* level, const Uint8* pStates)
{
	if( m_ChangeAnimation )
	{
		if( m_IsSwimming )
		{
			m_ActionState = ActionState::swimming;
			return;
		}

		if( m_ActionState == ActionState::backDashMoving )
		{
			m_ActionState = ActionState::backDashStoping;
			m_ChangeAnimation = false;
			return;
		}

		if( pStates[SDL_SCANCODE_UP] )
		{
			m_ActionState = ActionState::readyProjectile;
			m_ChangeAnimation = false;
			return;
		}
		else if( pStates[SDL_SCANCODE_DOWN] )
		{
			HandleActionStateCrouched( pStates );
			
			return;
		}
		else if( ( pStates[SDL_SCANCODE_X] ) && ( m_ActionState != ActionState::meleeAttackStanding ) )
		{
			if( !level->IsOnGround( m_PlayerLocation ) )
			{
				m_ActionState = ActionState::meleeAttackMidAir;
				m_ChangeAnimation = false;
				PlayAttackAudio();
				return;
			}

			if( m_ActionState != ActionState::meleeAttackStanding )
			{
				m_ActionState = ActionState::meleeAttackStanding;
				m_ChangeAnimation = false;
				PlayAttackAudio();
				return;
			}
		}
		else if( m_ActionState == ActionState::crouch )
		{
			m_ActionState = ActionState::standUp;
			m_ChangeAnimation = false;
			return;
		}
		else if( ( pStates[SDL_SCANCODE_Q] ) && ( m_HasBackDash ) )
		{
			m_ActionState = ActionState::backDashMoving;
			m_ChangeAnimation = false;
			return;
		}
		else
		{
			m_ChangeAnimation = true;
		}


		if( m_Velocity.y > 0 )
		{
			m_ActionState = ActionState::staticJump;

			if( m_Velocity.x != 0 )
			{
				m_ActionState = ActionState::movingJump;
			}
		}

		if( ( m_Velocity.y < 0 ) && ( ( m_ActionState == ActionState::movingJump ) || ( m_ActionState == ActionState::staticJump ) ) )
		{
			m_ActionState = ActionState::startingToFall;
			m_ChangeAnimation = false;
			return;
		}

		if( ( m_Velocity.y < 0 ) && ( !level->IsOnGround( m_PlayerLocation ) ) )
		{
			m_ActionState = ActionState::keepFalling;
		}

		if( ( m_ActionState == ActionState::startingToFall ) || ( m_ActionState == ActionState::keepFalling ) )
		{
			if( ( level->IsOnGround( m_PlayerLocation ) ) && ( m_Velocity.x == 0 ) )
			{
				m_ActionState = ActionState::landing;

				m_ChangeAnimation = false;

				m_pAudioManager->PlaySoundEffect( m_pAudioManager->fallDown );

				return;
			}
		}

		if( ( m_Velocity.x != 0 ) && ( m_Velocity.y == 0 ) )
		{
			m_ActionState = ActionState::running;
		}

		if ( ( m_Velocity.x == 0 ) && ( m_Velocity.y == 0 ) )
		{
			m_ActionState = ActionState::idle;
		}
	}
}

void Player::HandleActionStateCrouched(const Uint8* pStates)
{
	if( ( pStates[SDL_SCANCODE_X] ) && ( m_ActionState != ActionState::meleeAttackWhileCrouched ) )
	{
		m_ActionState = ActionState::meleeAttackWhileCrouched;
		m_ChangeAnimation = false;
		PlayAttackAudio();
		return;
	}

	if( m_ActionState == ActionState::meleeAttackWhileCrouched )
	{
		m_ActionState = ActionState::stayCrouched;
		m_ChangeAnimation = false;
		return;
	}

	if( m_ActionState == ActionState::stayCrouched )
	{
		m_PreviusActionState = ActionState::crouch;

		m_ActionState = ActionState::crouch;

		UpdateActionAnimationValues();

		m_AnimFrame = m_NrOfFrames - 1;

		m_ChangeAnimation = false;

		return;
	}

	m_ActionState = ActionState::crouch;

	m_ChangeAnimation = false;
}

void Player::UpdateAnimationFrame(float elapsedSec)
{
	UpdateAnimationFrameBlood( elapsedSec );

	m_AnimTime += elapsedSec;

	if( m_LoopAnimation )
	{
		LoopAnimation();
	}
	else
	{
		RunAnimationOnce();
	}	
}

void Player::UpdateAnimationFrameBlood(float elapsedSec)
{
	const float framesPerSecond{ 1.f / 5.f };

	const int maxNumberOfFrames{ 5 };

	if( m_Health <= 0 )
	{
		m_ParticlesAnimTime += elapsedSec;

		if( m_ParticlesAnimTime >= framesPerSecond )
		{
			m_ParticlesAnimTime = 0;
			
			++m_ParticlesAnimFrame;
			
			if( m_ParticlesAnimFrame >= maxNumberOfFrames )
			{
				m_ParticlesAnimFrame = 0;
			}
		}
	}
}

void Player::LoopAnimation()
{
	if ( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
	{
		m_AnimTime = 0;
		
		++m_AnimFrame;
		
		if( m_AnimFrame >= m_NrOfFrames )
		{
			m_AnimFrame = 0;
		}
	}
}

void Player::RunAnimationOnce()
{
	if( m_AnimTime >= ( 1.f / m_NrFramesPerSec ) )
	{
		m_AnimTime = 0;

		if( m_AnimFrame != ( m_NrOfFrames - 1 ) )
		{
			++m_AnimFrame;
		}
		else
		{
			m_ChangeAnimation = true;
		}
	}
}

void Player::UpdateActionAnimationValues()
{
	float clipWidth{};
	
	float clipHeight{};

	switch (m_ActionState)
	{
	case Player::ActionState::idle:
		clipWidth = 21.f;
		clipHeight = 36.f;

		m_NrOfFrames = 8;
		m_NrFramesPerSec = 8;

		m_AnimationDestionation = 0;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = true;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::running:
		clipWidth = 28.f;
		clipHeight = 35.f;

		m_NrOfFrames = 17;
		m_NrFramesPerSec = 17;

		m_AnimationDestionation = 0;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = true;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::staticJump:
		clipWidth = 22.f;
		clipHeight = 37.f;

		m_NrOfFrames = 4;
		m_NrFramesPerSec = 4;

		m_AnimationDestionation = 0;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = true;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::movingJump:
		clipWidth = 31.f;
		clipHeight = 34.f;

		m_NrOfFrames = 3;
		m_NrFramesPerSec = 3;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::startingToFall:
		clipWidth = 30.f;
		clipHeight = 35.f;

		m_NrOfFrames = 3;
		m_NrFramesPerSec = 9;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 0;

		m_MoveDuringAnimation = true;
		m_LoopAnimation = false;

		break;
	case Player::ActionState::keepFalling:
		clipWidth = 29.f;
		clipHeight = 36.f;

		m_NrOfFrames = 2;
		m_NrFramesPerSec = 2;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 0;

		m_MoveDuringAnimation = true;
		m_LoopAnimation = true;

		break;
	case Player::ActionState::landing:
		clipWidth = 31.f;
		clipHeight = 36.f;

		m_NrOfFrames = 5;
		m_NrFramesPerSec = 20;

		m_AnimationDestionation = -4;
		m_InverseAnimationDestionation = 6;

		m_MoveDuringAnimation = true;
		m_LoopAnimation = false;

		break;
	case Player::ActionState::meleeAttackMidAir:
		clipWidth = 54.f;
		clipHeight = 35.f;

		m_NrOfFrames = 7;
		m_NrFramesPerSec = 21;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 17;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::meleeAttackStanding:
		clipWidth = 54.f;
		clipHeight = 36.f;

		m_NrOfFrames = 12;
		m_NrFramesPerSec = 18;

		m_AnimationDestionation = -15;
		m_InverseAnimationDestionation = 18;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::crouch:
		clipWidth = 34.f;
		clipHeight = 26.f;

		m_NrOfFrames = 4;
		m_NrFramesPerSec = 8;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::meleeAttackWhileCrouched:
		clipWidth = 51.f;
		clipHeight = 23.f;

		m_NrOfFrames = 7;
		m_NrFramesPerSec = 14;

		m_AnimationDestionation = -13;
		m_InverseAnimationDestionation = 14;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::stayCrouched:
		clipWidth = 30.f;
		clipHeight = 22.f;

		m_NrOfFrames = 3;
		m_NrFramesPerSec = 6;

		m_AnimationDestionation = -6;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::standUp:
		clipWidth = 28.f;
		clipHeight = 27.f;

		m_NrOfFrames = 2;
		m_NrFramesPerSec = 10;

		m_AnimationDestionation = -4;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::readyProjectile:
		clipWidth = 32.f;
		clipHeight = 35.f;

		m_NrOfFrames = 5;
		m_NrFramesPerSec = 10;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::throwProjectile:
		clipWidth = 38.f;
		clipHeight = 35.f;

		m_NrOfFrames = 3;
		m_NrFramesPerSec = 12;

		m_AnimationDestionation = -10;
		m_InverseAnimationDestionation = 6;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::takeDamageFromFront:
		clipWidth = 28.f;
		clipHeight = 36.f;

		m_NrOfFrames = 4;
		m_NrFramesPerSec = 4;

		m_AnimationDestionation = -1;
		m_InverseAnimationDestionation = 5;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::takeDamageFromBack:
		clipWidth = 28.f;
		clipHeight = 35.f;

		m_NrOfFrames = 2;
		m_NrFramesPerSec = 2;

		m_AnimationDestionation = 0;
		m_InverseAnimationDestionation = 6;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::dying:
		clipWidth = 39.f;
		clipHeight = 36.f;

		m_NrOfFrames = 5;
		m_NrFramesPerSec = 3;

		m_AnimationDestionation = -7;
		m_InverseAnimationDestionation = 6;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::backDashMoving:
		clipWidth = 32.f;
		clipHeight = 36.f;

		m_NrOfFrames = 3;
		m_NrFramesPerSec = 9;

		m_AnimationDestionation = -1;
		m_InverseAnimationDestionation = 10;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = true;

		break;
	case Player::ActionState::backDashStoping:
		clipWidth = 32.f;
		clipHeight = 36.f;

		m_NrOfFrames = 4;
		m_NrFramesPerSec = 12;

		m_AnimationDestionation = -1;
		m_InverseAnimationDestionation = 10;

		m_LoopAnimation = false;
		m_MoveDuringAnimation = false;

		break;
	case Player::ActionState::swimming:
		clipWidth = 30.f;
		clipHeight = 37.f;

		m_NrOfFrames = 4;
		m_NrFramesPerSec = 4;

		m_AnimationDestionation = 0;
		m_InverseAnimationDestionation = 0;

		m_LoopAnimation = true;
		m_MoveDuringAnimation = true;

		break;
	default:
		break;
	}


	if (m_ActionState != m_PreviusActionState)
	{
		m_PreviusActionState = m_ActionState;
		m_AnimFrame = 0;
		m_AnimTime = 0;
	}

	m_AnimationSource.width = clipWidth;
	m_AnimationSource.height = clipHeight;
	m_AnimationSource.left = ( clipWidth * m_AnimFrame );
	m_AnimationSource.bottom = float( m_ActionState );
}

void Player::PerformDeathAnimation(float elapsedSec, const Level* level)
{
	const float movementSpeed{ 10.f };

	m_ActionState = ActionState::dying;

	if (m_LookingLeft)
	{
		m_Velocity.x = movementSpeed;
	}
	else
	{
		m_Velocity.x = -movementSpeed;
	}

	m_Velocity.y = 0;
	
	HandleMovement( elapsedSec, level, true );
}

#pragma endregion


#pragma region HandleIncomingDamage
void Player::UpdateInvincibility(float elapsedSec)
{
	const float maxTimeInvincible{ 2.f };

	if( m_HasPlayerBeenHit )
	{
		m_InvincibilityTimer += elapsedSec;
	}

	if( m_InvincibilityTimer >= maxTimeInvincible )
	{
		m_HasPlayerBeenHit = false;
		m_InvincibilityTimer = 0;
	}
}
#pragma endregion


#pragma region Attacks
void Player::PerformMeleeAttack(const Level* level)
{
	m_AttackArea.width = 20;

	m_AttackArea.height = 5;

	if( ( m_ActionState == ActionState::meleeAttackStanding ) || ( m_ActionState == ActionState::meleeAttackMidAir ) )
	{
		if( ( m_AnimFrame == 2 ) || ( m_AnimFrame == 3 ) )
		{
			m_AttackArea.left = m_PlayerLocation.left + m_PlayerLocation.width;

			const float percentageHeight{ 2.f / 3.f };

			m_AttackArea.bottom = m_PlayerLocation.bottom + ( m_PlayerLocation.height * percentageHeight );

			if( m_ActionState == ActionState::meleeAttackMidAir )
			{
				m_AttackArea.left += 5.f;
			}

			if( m_LookingLeft )
			{
				m_AttackArea.left = m_PlayerLocation.left - m_AttackArea.width;
			}

			level->HandlePlayerAttack(m_AttackArea, m_MeleeAttackDamage);
		}
	}
	else if( ( m_ActionState == ActionState::meleeAttackWhileCrouched ) && ( ( m_AnimFrame == 2 ) || ( m_AnimFrame == 3 ) ) )
	{
		m_AttackArea.left = m_PlayerLocation.left + m_PlayerLocation.width;

		const float percentageHeight{ 1.f / 3.f };

		m_AttackArea.bottom = m_PlayerLocation.bottom + ( m_PlayerLocation.height * percentageHeight );

		if( m_LookingLeft )
		{
			m_AttackArea.left = m_PlayerLocation.left - m_AttackArea.width;
		}

		level->HandlePlayerAttack( m_AttackArea, m_MeleeAttackDamage );
	}
	else
	{
		m_AttackArea = Rectf{};
	}
}

void Player::FireProjectile(const Uint8* pStates)
{
	if( ( pStates[SDL_SCANCODE_UP] ) && ( pStates[SDL_SCANCODE_X] ) && ( m_pSpear == nullptr ) )
	{
		m_pSpear = new PlayerProjectileSpear
		(
			Point2f
			(
				m_PlayerLocation.left + m_PlayerLocation.width / 2,
				m_PlayerLocation.bottom + m_PlayerLocation.height / 2 + 8.f
			),
			m_pTextureManager,
			m_LookingLeft
		);

		if( m_Mana < m_pSpear->GetManaConsumption() )
		{
			delete m_pSpear;
			m_pSpear = nullptr;
		}
		else
		{
			m_Mana -= m_pSpear->GetManaConsumption();
			
			if( m_ManaRegenTimer > 0 )
			{
				m_ManaRegenTimer = 0;
			}

			m_ActionState = ActionState::throwProjectile;

			m_ChangeAnimation = false;
			
			m_pAudioManager->PlaySoundEffect(m_pAudioManager->playerThrow);
		}
	}
}

void Player::UpdateProjectile(float elapsedSec, const Level* level)
{
	if( m_pSpear != nullptr )
	{
		m_pSpear->Update( elapsedSec );

		level->HandlePlayerAttack
		(
			m_pSpear->getTransVertices()[0],
			m_pSpear->getTransVertices()[1],
			m_pSpear->GetDamage()
		);

		if( level->DoesProjectileHitCollision( m_pSpear->getTransVertices()[0], m_pSpear->getTransVertices()[1] ) )
		{
			m_pSpear->StopMovement();	
		}

		if( m_pSpear->IsObjectDead() )
		{
			delete m_pSpear;
			m_pSpear = nullptr;
		}
		else if ( ( !utils::IsPointInRect( m_pSpear->getTransVertices()[0], level->GetBoundaries() ) ) && ( !utils::IsPointInRect( m_pSpear->getTransVertices()[1], level->GetBoundaries() ) ) )
		{
			delete m_pSpear;
			m_pSpear = nullptr;
		}
	}
}

void Player::PlayAttackAudio()
{
	int randomNumber{ rand() % 201 };

	if( randomNumber > 100 )
	{
		if( randomNumber > 180 )
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->randomSound1 );
		}
		else if( randomNumber > 160 )
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->randomSound2 );
		}
		else if( randomNumber > 140 )
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->randomSound3 );
		}
		else if( randomNumber > 120 )
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->randomSound4 );
		}
		else
		{
			m_pAudioManager->PlaySoundEffect( m_pAudioManager->randomSound5 );
		}
	}

	m_pAudioManager->PlaySoundEffect( m_pAudioManager->playerSwing );
}

#pragma endregion


#pragma region DrawFunctions
void Player::DrawPlayer() const
{
	const Texture* somaCruz{ m_pTextureManager->GetTexture( m_pTextureManager->somaCruz ) };
	if( m_LookingLeft )
	{
		glScalef( -1, 1, 1 );
		
		glTranslatef( -m_AnimationSource.width, 0, 0 );

		somaCruz->Draw( Point2f{ m_InverseAnimationDestionation, 0 }, m_AnimationSource );
	}
	else
	{
		somaCruz->Draw( Point2f{ m_AnimationDestionation, 0 }, m_AnimationSource );
	}
}

void Player::DrawBloodParticles() const
{
	const Texture* blood{ m_pTextureManager->GetTexture( m_pTextureManager->blood ) };

	if( m_Health <= 0 )
	{
		Rectf sourceParticles{};
		
		sourceParticles.width = 32.f;
		
		sourceParticles.height = 33.f;
		
		sourceParticles.bottom = sourceParticles.height;

		const int numberOfDrawings{ 4 };

		for(int i{ 0 }; i < numberOfDrawings; i++)
		{
			sourceParticles.left = ( sourceParticles.width * ( m_ParticlesAnimFrame + i ) );
			
			blood->Draw( Point2f{}, sourceParticles );
		}
	}
}

void Player::DrawProjectile() const
{
	if( m_pSpear != nullptr )
	{
		m_pSpear->Draw();
	}
}
#pragma endregion
