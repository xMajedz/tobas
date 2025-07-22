#include "api.h"
#include "game.h"

int main()
{
	Window::Init();

	Game::Init();
	Game::NewGame();

	bool running = true;
	while (running)
	{
		Window::Update();

		Game::Update(Game::GetFrameTime());
		
		Window::Draw();
	
		running = !raylib::WindowShouldClose();

		Console::Update();
	}

	Game::Quit();
}
