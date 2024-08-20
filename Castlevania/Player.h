#pragma once
#include <utils.h>

class Level;

class Texture;

class PlayerProjectileSpear;

class Hud;

class TextureManager;

class AudioManager;

class Player final
{
public:

	enum class ActionState
	{
		idle = 35,
		running = 70,
		staticJump = 107,
		movingJump = 141,
		startingToFall = 176,
		keepFalling = 212,
		landing = 248,
		meleeAttackMidAir = 283,
		meleeAttackStanding = 319,
		crouch = 345,
		meleeAttackWhileCrouched = 368,
		stayCrouched = 390,
		standUp = 417,
		readyProjectile = 452,
		throwProjectile = 487,
		takeDamageFromFront = 523,
		takeDamageFromBack = 558,
		dying = 594,
		backDashMoving = 630,
		backDashStoping = 666,
		swimming = 703
	};

	Player(TextureManager* textureManager, AudioManager* audioManager,const float viewportHeight, const float scale);
	
	~Player();

#pragma region RuleOf5
	Player(const Player&) = delete;
	
	Player& operator=(const Player&) = delete;
	
	Player(Player&&) = delete;
	
	Player& operator=(Player&&) = delete;
#pragma endregion

	void Update(float elapsedSec, const Level* level);
	
	void Draw() const;

	void DrawHud() const;

	const Rectf& GetShape() const;

	Rectf GetHitbox() const;

	float GetHealth() const;

	void SetLocation(const Point2f& spawnLocation);

	void TakeDamage(int damage, bool damagefFromTheLeft);

	void AddCoinValue(int valueGold);

	void UnlockBackDash();

	void SetVelocity(const Vector2f& velocity);

	void SetIsSwimming(bool isSwimming);

private:
	bool m_IsSwimming{ false };
	
	TextureManager* m_pTextureManager;
	
	AudioManager* m_pAudioManager;

	Hud* m_pHud;

	Rectf m_PlayerLocation;

	const float m_HorSpeed{ 85.0f },
				m_JumpSpeed{ 300.0f };
	
	const Vector2f m_Acceleration{ 0, -981.0f };
	
	Vector2f m_Velocity{ 0.0f, 0.0f };
	
	ActionState m_ActionState{ ActionState::idle },
				m_PreviusActionState{ ActionState::idle };
	

	int m_NrOfFrames{ 0 },
		m_AnimFrame{ 0 };

	float m_NrFramesPerSec{ 0 },
		m_AnimTime{ 0 },
		m_AnimationDestionation{ 0 },
		m_InverseAnimationDestionation{ 0 };

	Rectf m_AnimationSource;
	
	bool m_LookingLeft{ false },
		m_ChangeAnimation{ true },
		m_LoopAnimation{ true },
		m_MoveDuringAnimation{ true };

	float m_ParticlesAnimTime{ 0 };

	int m_ParticlesAnimFrame{ 0 };

	Rectf m_AttackArea;

	const int m_MeleeAttackDamage{10};
	
	PlayerProjectileSpear* m_pSpear;

	bool m_HasBackDash{ false };

	int m_Coins{};
	
	float m_MaxHealth{ 70 },
		m_MaxMana{ 80 },
		m_Health{ 70 },
		m_Mana{ 80 };

	bool m_HasPlayerBeenHit{ false };
	
	float m_InvincibilityTimer{ 0 },
		m_ManaRegenTimer{ 0 };

#pragma region HandleMovement
	void HandleKeyboardInput(const Uint8* pStates, const Level* level);
	
	void HandleMovement(float elapsedSec, const Level* level, bool ignoreGravity);
#pragma endregion

#pragma region HandleAnimation
	void ChangeActionState(const Level* level, const Uint8* pStates);

	void HandleActionStateCrouched( const Uint8* pStates );
	
	void UpdateAnimationFrame(float elapsedSec);
	
	void UpdateAnimationFrameBlood(float elapsedSec);

	void LoopAnimation();

	void RunAnimationOnce();

	void UpdateActionAnimationValues();

	void PerformDeathAnimation(float elapsedSec, const Level* level);
#pragma endregion

#pragma region HandleIncomingDamage
	void UpdateInvincibility(float elapsedSec);
#pragma endregion

#pragma region Attacks
	void PerformMeleeAttack(const Level* level);
	
	void FireProjectile(const Uint8* pStates);

	void UpdateProjectile(float elapsedSec, const Level* level);

	void PlayAttackAudio();
#pragma endregion

	
#pragma region DrawFunctions
	void DrawPlayer() const;

	void DrawBloodParticles() const;

	void DrawProjectile() const;
#pragma endregion
};

