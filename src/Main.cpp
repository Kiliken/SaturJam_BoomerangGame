# include <Siv3D.hpp>
# include "Title.h"
# include "Game.h"

#if false 




void Main()
{
	

	while (System::Update())
	{

	}
}


#else

using App = SceneManager<String>;



void Main()
{
	Window::SetTitle(U"BoomerangGame");
	//Window::Resize(1280, 720);

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
