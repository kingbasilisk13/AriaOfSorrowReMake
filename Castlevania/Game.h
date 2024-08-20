#pragma once
#include "BaseGame.h"
#include <vector>

class Player;
class Camera;
class LevelManager;
class TextureManager;
class AudioManager;
class UIManager;


class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
#pragma region RuleOf5
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
#pragma endregion
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	
	void Draw( ) const override;

	
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Player* m_pPlayer;
	Camera* m_pCamera;
	LevelManager* m_pLevelManager;
	TextureManager* m_pTextureManager;
	AudioManager* m_pAudioManager;

	const float m_Scale;

	UIManager* m_pUIManager;

#pragma region HelperFunction
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	void PrintControles() const;
#pragma endregion

	
};