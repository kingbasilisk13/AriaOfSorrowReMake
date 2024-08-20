#pragma once
class Texture;

class TextureManager;

class Hud final
{
public:
	Hud(TextureManager* textureManager, const Point2f& topLeft, float scale);
	
	~Hud() = default;

#pragma region RuleOf5
	Hud(const Hud&) = delete;
	
	Hud& operator=(const Hud&) = delete;
	
	Hud(Hud&&) = delete;
	
	Hud& operator=(Hud&&) = delete;
#pragma endregion
	
	void Update(float playerHealth, float playerMaxHealth, float playerMana, float playerMaxMana);
	
	void Draw() const;

private:
	TextureManager* m_pTextureManager;

	Point2f m_BottomLeft;
	
	float m_PlayerHealth{ 0 },
		m_PlayerMaxHealth{ 0 },
		m_PlayerMana{ 0 },
		m_PlayerMaxMana{ 0 },
		m_Scale;

#pragma region HelperFunctions
	void DrawHealthBar() const;
	
	void DrawManaBar() const;
	
	void DrawHealthInNumbers() const;
#pragma endregion
};

