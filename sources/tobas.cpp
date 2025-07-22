#include "api.h"
#include "game.h"

#include "netcode_client.h"
#include "netcode_server.h"

int main()
{

	Window::Init();

	Game::Init();
	Game::NewGame();

	bool running = true;
	bool host_running = false;
	
	int status = Client::Connect();

	if (status != 0) {
		LOG_A
		Server::HostGameThread();
		Server::WaitGameThread();
		host_running = true;

		int max_attempts = 2;

		for (int attempts = 0 ; status != 0 && attempts < max_attempts; attempts += 1) {
			status = Client::Connect();
		}
	}

	while (running)
	{
		Window::Update();

		Client::Update(Game::GetTime(), Game::GetFrameTime());

		Game::Update(Game::GetFrameTime());

		Window::Draw();

		if (raylib::WindowShouldClose()) {
			Client::Disconnect();
			running = false;
		}

		Console::Update();
	}

	if (host_running) {
		Server::CloseThread();
	} else {
		Game::Quit();
	}
}
