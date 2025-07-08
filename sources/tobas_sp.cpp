#include "api.h"
#include "game.h"
#include "camera.h"

Window window;

JointID SelectJoint(
		Camera3D* camera,
		Ray* MouseRay,
		RayCollision* MouseCollision,
		Player* player
)
{
	/*int hits = 0;
	RayCollision collision = { 0 };
	*MouseRay = GetMouseRay(GetMousePosition(), camera);
	std::string joint_name = "NONE";
	for (auto& [joint_name, j] : player->joint) {
		collision = j.collide_mouse_ray(*MouseRay, collision);
		if (collision.hit) {
			hits += 1;
			j.select = true;
			return joint_name;
		} else {
			j.select = false;
		}
	}
	return joint_name;*/
	return -1;
}

PlayerID SelectPlayer(
		Camera3D* camera,
		Ray* MousseRay,
		RayCollision* MouseCollision,
		std::vector<Player>* players
)
{
	return -1;
}

int main()
{
	window = {.width = 800, .height = 450};
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(window.width, window.height, "TOBAS");

	Game::Init();
	Game::NewGame();

	Gamecam::Init();
	const auto& camera = Gamecam::Get();

	Ray MouseRay = { 0 };
	RayCollision MouseCollision = { 0 };

	SetExitKey(0);
	while (!WindowShouldClose()) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

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
	
		if (IsFileDropped()) {
			FilePathList dropped_files = LoadDroppedFiles();
			API::FileDroppedCallback(*(dropped_files.paths));
			UnloadDroppedFiles(dropped_files);
	        }

		if (Console::GetHasMessage()) {
			const char* message = Console::GetMessage();
			API::ConsoleCallback(message);
			Console::ResetHasMessage();
		}
	}

	Game::Quit();

	CloseWindow();
}
