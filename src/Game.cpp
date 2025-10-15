# include "Game.h"

void Game::drawFadeIn(double t) const
{
	draw();
	Circle{ Scene::Size().x / 2, Scene::Size().y / 2, Scene::Size().x }
	.drawFrame(((1 - t) * Scene::Size().x), 0, ColorF{ 0.2, 0.3, 0.4 });



}

void Game::drawFadeOut(double t) const
{
	draw();
	Circle{ Scene::Size().x / 2, Scene::Size().y / 2, Scene::Size().x }
	.drawFrame((t * Scene::Size().x), 0, ColorF{ 0.2, 0.3, 0.4 });
}

void Game::update()
{
	if (not m_stopwatch.isStarted())
	{
		m_stopwatch.start();
	}

	// On left click
	if (MouseL.down())
	{
		m_stopwatch.pause();

		// Transition to title scene
		changeScene(U"Title", 1.5s);
	}
}

void Game::draw() const
{
	Scene::SetBackground(ColorF(0.0, 0.6, 0.4));

	const double t = m_stopwatch.sF();

	const Vec2 pos{ (400 + Periodic::Sine1_1(3s, t) * 300), 300 };

	m_emoji.drawAt(pos);
}
