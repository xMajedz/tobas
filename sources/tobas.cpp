#include "api.h"
#include "camera.h"
#include "netcode_client.h"
#include "netcode_server.h"

#include "raygui.h"

bool running = true;
bool host_running = false;

int main()
{
	SetTraceLogLevel(LOG_ERROR);
	auto screenWidth  = Window::GetWidth  ();
	auto screenHeight = Window::GetHeight ();
	InitWindow(screenWidth, screenHeight, "TOBAS");

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

	auto bg = LoadRenderTexture(screenWidth, screenHeight);
	auto fg = LoadRenderTexture(screenWidth, screenHeight);

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
		
		BeginTextureMode(bg);
			ClearBackground(RAYWHITE);
			BeginMode3D(camera);
			Game::Draw();
			EndMode3D();
		EndTextureMode();
		BeginTextureMode(fg);
			BeginMode3D(camera);
			ClearBackground(Fade(WHITE, 0.0f));
			EndMode3D();
		EndTextureMode();
		BeginDrawing();
			DrawTextureRec(bg.texture, {0, 0, screenWidth, -screenHeight}, {0, 0}, WHITE);
			DrawTextureRec(fg.texture, {0, 0, screenWidth, -screenHeight}, {0, 0}, WHITE);
			GuiLabel((Rectangle){20, 140, 90, 20}, "69");
		EndDrawing();
	}

	if (host_running) {
		Server::CloseThread();
	} else {
		Game::Quit();
	}

	UnloadRenderTexture(bg);
	UnloadRenderTexture(fg);

	CloseWindow();
}
