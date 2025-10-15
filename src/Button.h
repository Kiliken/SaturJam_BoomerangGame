#pragma once

#include <Siv3D.hpp>

class Button
{
	public:

	Button(Rect r, FontAsset f,String s)
		: rect(r)
		, font(f)
		, text(s)
	{};
	~Button();


	void update();

	void draw();

	bool clicked();
	
	private:

	bool hover = false;

	Rect rect;
	FontAsset font;
	String text;
};