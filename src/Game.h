# pragma once
# include <Siv3D.hpp>
# include "Animator.h"


class Game : public SceneManager<String>::Scene
{
public:

	Game(const InitData& init)
		: IScene{ init }
		, m_emoji{ U"🐥"_emoji }
	{
		
	}

	~Game()
	{
		
	}


	void update() override;

	// Draw function
	void draw() const override;

	void drawFadeIn(double t) const override;

	void drawFadeOut(double t) const override;

private:

	Texture m_emoji;

	Stopwatch m_stopwatch;
};