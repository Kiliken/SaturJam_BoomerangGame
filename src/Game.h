# pragma once
# include <Siv3D.hpp>
# include "Animator.h"




class Game : public SceneManager<String>::Scene
{
public:

	Game(const InitData& init)
		: IScene{ init }
	{
		Scene::SetBackground(ColorF{ 0.424, 0.62, 0.063 });
	}

	~Game()
	{
		
	}


	void update() override;

	// Draw function
	void draw() const override;

	void drawFadeIn(double t) const override;

	void drawFadeOut(double t) const override;

	Vec2 GenerateEnemy()
	{
		return RandomVec2({ 50, 750 }, -20);
	}

private:

	const Font font{ FontMethod::MSDF, 48 };

	Texture* playerTextureRef;
	Texture* enemyTextureRef;
	Texture* boomerangTextureRef;

	Vec2 playerPos{ 400, 500 };
	Array<Vec2> enemies = { GenerateEnemy() };

	Array<Vec2> playerBullets;
	Array<Vec2> enemyBullets;

	double PlayerSpeed = 550.0;
	double PlayerBulletSpeed = 500.0;
	double EnemySpeed = 100;
	double EnemyBulletSpeed = 300.0;

	double InitialEnemySpawnInterval = 2.0;
	double enemySpawnTime = InitialEnemySpawnInterval;
	double enemyAccumulatedTime = 0.0;


	double PlayerShotCoolTime = 0.1;
	double playerShotTimer = 0.0;

	double EnemyShotCoolTime = 0.9;
	double enemyShotTimer = 0.0;

	//Boomerang suff
	bool throwBoomerang = false;
	bool boomBack = false;
	Vec2 boomerangPos = { 0.0,0.0 };
	Vec2 boomThrowPos = { 0.0,0.0 };
	Vec2 boomCurrentPos = { Scene::Width(),Scene::Height() };
	Circle boomCircle;

	Effect effect;

	int32 highScore = 0;
	int32 score = 0;
};