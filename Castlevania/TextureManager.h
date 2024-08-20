#pragma once
#include <iostream>
#include <string>
#include <map>

class Texture;

class TextureManager final
{
public:
	TextureManager();
	~TextureManager();

#pragma region RuleOf5
	TextureManager(const TextureManager&) = delete;

	TextureManager& operator=(const TextureManager&) = delete;
	
	TextureManager(TextureManager&&) = delete;
	
	TextureManager& operator=(TextureManager&&) = delete;
#pragma endregion

#pragma region TexturePaths
	const std::string somaCruz				{ "Player/SomaCruz.png" };
	const std::string playerBaseShape		{ "Player/PlayerPlaceHolder.png" };
	const std::string wingedGuardSpear		{ "Player/WingedGuardSpear.png" };
	const std::string blood					{ "blood.png" };
	const std::string zombie				{ "Enemies/Zombie.png" };
	const std::string boneThrower			{ "Enemies/BoneThrower.png" };
	const std::string boneProjectile		{ "Enemies/BoneProjectile.png" };
	const std::string goldCoins				{ "Objects/GoldCoins.png" };
	const std::string backLight				{ "Objects/BackLight.png" };
	const std::string wallChandelier		{ "Objects/WallChandelier.png" };
	const std::string castleEntrance		{ "Level/CastleEntrance.png" };
	const std::string moonBackground1		{ "Level/MoonBackground1.png" };
	const std::string corridor1				{ "Level/Corridor1.png" };
	const std::string corridor2				{ "Level/Corridor2.png" };
	const std::string wallBackground		{ "Level/WallBackground.png" };
	const std::string moonBackground2		{ "Level/MoonBackground2.png" };
	const std::string waterCrossing			{ "Level/WaterCrossing.png" };
	const std::string stoneHallway			{ "Level/StoneHallway.png" };
	const std::string bossRoom				{ "Level/BossRoom.png" };
	const std::string loadinScreen			{ "LoadinScreen.png" };
	const std::string gameOverScreen		{ "GameOverScreen.png" };
	const std::string counter				{ "Hud/Counter.png" };
	const std::string healthBar				{ "Hud/HealthBar.png" };
	const std::string manaBar				{ "Hud/ManaBar.png" };
	const std::string blueNumbers			{ "Hud/BlueNumbers.png" };
	const std::string graveKeeperBackDash	{ "Objects/GraveKeeperBackDash.png" };
	const std::string introScreenBackground	{ "UI/IntroScreenBackground.png" };
	const std::string titleScreenLogo		{ "UI/TitleScreenLogo.png" };
	const std::string instructions			{ "UI/instructions.png" };
	const std::string music					{ "UI/Music.png" };
	const std::string sound					{ "UI/Sound.png" };
	const std::string pointer				{ "UI/Pointer.png" };
#pragma endregion

	const Texture* GetTexture(const std::string& filename);

private:
	std::map<std::string, Texture*> m_TextureMap{};
};