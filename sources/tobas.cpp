#include "api.h"
#include "game.h"
#include "camera.h"
#include "netcode_client.h"
#include "netcode_server.h"

Window window;

bool running = true;
bool host_running = false;

int main()
{
	window = {.width = 800, .height = 450};
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(window.width, window.height, "TOBAS");

	Game::Init();
	Game::NewGame();
	
	if (client_connect() > 0) {
		host_game_thread();
		host_wait_thread();
		host_running = true;
		client_connect();
	}

	Gamecam::Init();
	const auto& camera = Gamecam::Get();

	SetExitKey(0);
	while (running) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		if (WindowShouldClose()) {
			client_disconnect();
			running = false;
		}

		client_update();

		if (0 > Game::GetSelectedPlayerID()) {
			Gamecam::UpdateSpectatorcam(Game::GetFreeze(), Game::GetPlayers());
		} else {
			Gamecam::UpdatePlaycam(Game::GetFreeze(), Game::GetSelectedPlayer());
		}

		Game::Update(GetFrameTime());
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				Game::Draw();
				API::Draw3DCallback();
			EndMode3D();
			API::Draw2DCallback();
		EndDrawing();
	}

	if (host_running) {
		host_close_thread();
	}

	Game::Quit();

	CloseWindow();
}
