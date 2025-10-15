# include <Siv3D.hpp>
# include "Title.h"
# include "Game.h"

#if true 

Vec2 GenerateEnemy()
{
	return RandomVec2({ 50, 750 }, -20);
}

void Main()
{
	Scene::SetBackground(ColorF{ 0.424, 0.62, 0.063 });

	const Font font{ FontMethod::MSDF, 48 };

	// Player texture
	const Texture playerTexture{ U"👩‍🌾"_emoji };
	// Enemy texture
	const Texture enemyTexture{ U"🦘"_emoji };

	const Texture boomerangTexture{ U"🪃"_emoji };

	// Player
	Vec2 playerPos{ 400, 500 };
	// Enemy
	Array<Vec2> enemies = { GenerateEnemy() };

	// Player shots
	Array<Vec2> playerBullets;
	// Enemy shots
	Array<Vec2> enemyBullets;

	// Player speed
	constexpr double PlayerSpeed = 550.0;
	// Player shot speed
	constexpr double PlayerBulletSpeed = 500.0;
	// Enemy speed
	constexpr double EnemySpeed = 100;
	// Enemy shot speed
	constexpr double EnemyBulletSpeed = 300.0;

	// Initial enemy spawn interval (seconds)
	constexpr double InitialEnemySpawnInterval = 2.0;
	// Enemy spawn interval (seconds)
	double enemySpawnTime = InitialEnemySpawnInterval;
	// Enemy spawn accumulated time (seconds)
	double enemyAccumulatedTime = 0.0;

	// Player shot cooltime (seconds)
	constexpr double PlayerShotCoolTime = 0.1;
	// Player shot cooltime timer (seconds)
	double playerShotTimer = 0.0;

	// Enemy shot cooltime (seconds)
	constexpr double EnemyShotCoolTime = 0.9;
	// Enemy shot cooltime timer (seconds)
	double enemyShotTimer = 0.0;

	//Boomerang suff
	bool throwBoomerang = false;
	bool boomBack = false;
	Vec2 boomerangPos = { 0.0,0.0 };
	Vec2 boomThrowPos = { 0.0,0.0 };
	Vec2 boomCurrentPos = { Scene::Width(),Scene::Height() };
	Circle boomCircle;

	Effect effect;

	// High score
	int32 highScore = 0;
	// Current score
	int32 score = 0;

	while (System::Update())
	{
		const uint64 t = Time::GetMillisec();
		const int32 x = (t / 100 % 36);

		// Game over check
		bool gameover = false;

		const double deltaTime = Scene::DeltaTime();
		enemyAccumulatedTime += deltaTime;
		playerShotTimer = Min((playerShotTimer + deltaTime), PlayerShotCoolTime);
		enemyShotTimer += deltaTime;

		// Generate enemies
		while (enemySpawnTime <= enemyAccumulatedTime)
		{
			enemyAccumulatedTime -= enemySpawnTime;
			enemySpawnTime = Max(enemySpawnTime * 0.95, 0.3);
			enemies << GenerateEnemy();
		}

		// Player movement
		const Vec2 move = Vec2{ (KeyD.pressed() - KeyA.pressed()), (KeyS.pressed() - KeyW.pressed()) }
		.setLength(deltaTime * PlayerSpeed * (KeyShift.pressed() ? 1.0 : 0.5));
		playerPos.moveBy(move).clamp(Scene::Rect());

		if (KeySpace.pressed() && !throwBoomerang) {
			throwBoomerang = true;
			boomThrowPos = playerPos + Vec2{ 0.0,playerPos.y - 800.0 };
			boomCurrentPos = playerPos;

		}



		if (throwBoomerang) {
			if (!boomBack) {
				boomCurrentPos = Math::Lerp(boomCurrentPos, boomThrowPos, Scene::DeltaTime());
				if (boomCurrentPos.distanceFrom(boomThrowPos) < 20.0)
					boomBack = true;
			}

			else {
				boomCurrentPos = Math::Lerp(boomCurrentPos, playerPos, Scene::DeltaTime());
				if (boomCurrentPos.distanceFrom(playerPos) < 20.0)
				{
					throwBoomerang = false;
					boomBack = false;
				}
			}
			boomCircle = Circle{ boomCurrentPos, 40 };
			//boomCurrentPos.x = Math::Lerp(boomCurrentPos.y,boomThrowPos.x,Scene::DeltaTime());
			//boomCurrentPos.y = Math::Lerp(boomCurrentPos.y,boomThrowPos.x,Scene::DeltaTime());
		}

		/*
		// Player shot firing
		if (PlayerShotCoolTime <= playerShotTimer)
		{
			playerShotTimer -= PlayerShotCoolTime;
			playerBullets << playerPos.movedBy(0, -50);
		}

		// Move player shots
		for (auto& playerBullet : playerBullets)
		{
			playerBullet.y += (deltaTime * -PlayerBulletSpeed);
		}
		// Remove player shots that went off screen
		playerBullets.remove_if([](const Vec2& b) { return (b.y < -40); });
		*/

		// Move enemies
		for (auto& enemy : enemies)
		{
			float dx = playerPos.x - enemy.x;
			float dy = playerPos.y - enemy.y;

			// Calculate distance
			float distance = enemy.distanceFrom(playerPos);

			// Avoid division by zero
			if (distance > 0.0f)
			{
				// Normalize direction
				float dirX = dx / distance;
				float dirY = dy / distance;

				// Move enemy at constant speed
				enemy.x += dirX * EnemySpeed * deltaTime;
				enemy.y += dirY * EnemySpeed * deltaTime;
			}
		}


		// Remove enemies that went off screen
		enemies.remove_if([&](const Vec2& e)
		{
			if (700 < e.y)
			{
				// Game over if enemy goes off screen
				gameover = true;
				return true;
			}
			else
			{
				return false;
			}
		});



		////////////////////////////////
		//
		//	Hit detection
		//
		////////////////////////////////

		// Enemy vs player shot
		for (auto itEnemy = enemies.begin(); itEnemy != enemies.end();)
		{
			const Circle enemyCircle{ *itEnemy, 40 };
			bool skip = false;


			if (enemyCircle.intersects(boomCircle) && throwBoomerang)
			{
				// Add explosion effect
				effect.add([pos = *itEnemy](double t)
				{
				const double t2 = ((0.5 - t) * 2.0);
				Circle{ pos, (10 + t * 280) }.drawFrame((20 * t2), ColorF{ 1.0, (t2 * 0.5) });
				return (t < 0.5);
				});

				itEnemy = enemies.erase(itEnemy);
				++score;
				skip = true;
				break;
			}


			if (skip)
			{
				continue;
			}

			++itEnemy;
		}

		// Enemy vs player
		for (auto& enemy : enemies)
		{
			if (enemy.distanceFrom(playerPos) <= 20)
			{
				// Game over
				gameover = true;
				break;
			}
		}

		// Reset if game over
		if (gameover)
		{
			playerPos = Vec2{ 400, 500 };
			enemies.clear();
			playerBullets.clear();
			enemyBullets.clear();
			enemySpawnTime = InitialEnemySpawnInterval;
			throwBoomerang = false;
			highScore = Max(highScore, score);
			score = 0;
		}

		////////////////////////////////
		//
		//	Drawing
		//
		////////////////////////////////

		// Draw background animation
		for (int32 i = 0; i < 12; ++i)
		{
			const double a = Periodic::Sine0_1(2s, Scene::Time() - (2.0 / 12 * i));
			Rect{ 0, (i * 50), 800, 50 }.draw(ColorF(1.0, a * 0.2));
		}

		// Draw player
		playerTexture.resized(80).drawAt(playerPos);



		// Draw enemies
		for (const auto& enemy : enemies)
		{
			enemyTexture.resized(60).drawAt(enemy);
		}

		if (throwBoomerang) {
			boomerangTexture.resized(80).rotated(x).drawAt(boomCurrentPos);
			//boomCircle.draw();
		}

		// Draw explosion effects
		effect.update();

		// Draw score
		font(U"{} [{}]"_fmt(score, highScore)).draw(30, Arg::bottomRight(780, 580));
	}
}


#else

using App = SceneManager<String>;

void Main()
{
	Window::SetTitle(U"BoomerangGame");
	Window::Resize(1280, 720);

	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, Typeface::Bold);

	// Create scene manager
	App manager;

	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");

	manager.setFadeColor(ColorF{ 0.8, 0.9, 1.0 });

	manager.init(U"Title", 0.75s);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}

#endif
