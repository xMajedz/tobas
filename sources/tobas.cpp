#include "api.h"
#include "game.h"
#include "camera.h"
#include "netcode_client.h"
#include "netcode_server.h"

Window window;

int main()
{
	window = { .width = 1600, .height = 900};
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(window.width, window.height, "TOBAS");

	Game::Init();
	//Game::NewGame();
	
	if (client_connect() > 0) {
		host_game_thread();
		host_wait_thread();
		client_connect();
	}

	Gamecam::Init();
	const auto& camera = Gamecam::Get();

	SetExitKey(0);
	while (!WindowShouldClose()) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		client_update();
		
		//Game::Update(GetFrameTime());
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
				//Game::Draw();
				//API::Draw3DCallback();
			EndMode3D();
			//API::Draw2DCallback();
		EndDrawing();
	}

	client_disconnect();
	host_close_thread();
	Game::Quit();

	CloseWindow();
}
