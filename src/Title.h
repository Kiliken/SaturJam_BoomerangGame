# pragma once

# include <Siv3D.hpp>
# include "Button.h"

class Title : public SceneManager<String>::Scene
{
public:

	// Constructor (must be implemented)
	Title(const InitData& init);
	~Title();

	// Update function
	void update() override;

	// Draw function
	void draw() const override;

	void drawFadeIn(double t) const override;

	void drawFadeOut(double t) const override;

private:
	Button* playButton;
	Button* quitButton;

};