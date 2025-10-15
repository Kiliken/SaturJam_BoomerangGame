#include "Button.h"

void Button::update() {
	if (rect.mouseOver())
	{
		// Change the mouse cursor to a hand
		Cursor::RequestStyle(CursorStyle::Hand);
		hover = true;
	}
	else hover = false;
}

void Button::draw() {

	RoundRect roundRect = rect.rounded(6);

	roundRect
		.drawShadow(Vec2{ 2, 2 }, 12, 0)
		.draw((hover ? ColorF{ 1, 0.75, 0.5 } : ColorF{ 0.9, 0.9, 0.9 }));

	// Draw border
	rect.stretched(-3).rounded(3)
		.drawFrame(2, ColorF{ 0.2, 0.2, 0.2 });

	// Draw text
	font(text).drawAt(40, rect.center(), ColorF{ 0.1, 0.1, 0.1 });
}

bool Button::clicked() {
	return rect.leftClicked();
}
