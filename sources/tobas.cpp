#include "api.h"
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

	if (Client::Connect() > 0) {
		Server::HostGameThread();
		Server::WaitGameThread();

		if (Client::Connect() > 0) {
			Client::Connect();
		}

		Client::SkipLocalSim();
	} else {
		Game::Init();
		Game::NewGame();
	}

	API::SetCallback("Update" , "Net", [](lua_State* L) {
		if (IsKeyPressed(KEY_SPACE)) {
			Client::Ready();
		}

		Client::Update(GetTime(), GetFrameTime());
		return 1;
	});

	Gamecam::Init();
	const auto& camera = Gamecam::Get();

	SetExitKey(0);
	while (running) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		if (WindowShouldClose()) {
			Client::Disconnect();
			running = false;
		}

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
		Server::CloseThread();
	} else {
		Game::Quit();
	}

	CloseWindow();
}
