#include "api.h"
#include "game.h"
#include "camera.h"

Window window;

int SelectJoint(Camera3D camera, Ray* MouseRay, RayCollision* MouseCollision, Player* player)
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

int SelectPlayer(Camera3D* camera, Ray* MousseRay, RayCollision* MouseCollision)
{
	return -1;
}

int main()
{
	window = { .width = 1600, .height = 900};
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

		/*
		 game.state.selected_joint = SelectJoint(camera, &MouseRay, &MouseCollision, selected_player);

		
		if (IsKeyPressed(KEY_Z)) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) {
				if (game.state.freeze && game.state.gamemode == FREEPLAY) {
					game.PlayFrame(game.state.game_frame - 1);
				}
			} else {
				if (game.state.selected_joint != "NONE") {
					game.ReFreeze();
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						selected_joint->ToggleActiveStateAlt();
					} else {
						selected_joint->ToggleActiveState();
					}
				}
			}
		}
	
		if (IsKeyPressed(KEY_X)) {
			if (game.state.selected_joint != "NONE") {
				game.ReFreeze();
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					selected_joint->TogglePassiveStateAlt();
				} else {
					selected_joint->TogglePassiveState();
				}
			}
		}
	
		if (IsKeyPressed(KEY_C)) {
			selected_player->TogglePlayerPassiveStatesAlt();
			selected_player->TogglePlayerPassiveStates();
		}
	
		if (IsMouseButtonPressed(0)) {
			//game.SelectPlayer(game.gamecam.camera, MouseRay, MouseCollision);
			if (game.state.selected_joint != "NONE") {
				game.ReFreeze();
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					selected_joint->CycleStateAlt();
				} else {
					selected_joint->CycleState();
				}
			}
		}
	
		if (IsKeyPressed(KEY_SPACE)) {
			if (game.state.freeze && game.state.gamemode == FREEPLAY) {
				if (IsKeyDown(KEY_LEFT_SHIFT)) {
					game.StepGame(1);
				} else {
					game.StepGame(game.rules.turnframes);
				}
			} else if (!game.state.freeze && game.state.gamemode == REPLAY) {
				game.StartFreeplay();
			}
		}
	
		if (IsKeyPressed(KEY_E)) {
			if (!game.state.freeze && game.state.gamemode == REPLAY) {
				game.EditReplay();
			}
		}
	
		if (IsKeyPressed(KEY_F)) {
			game.SaveReplay();
		}
	
		if (IsKeyPressed(KEY_R)) {
			game.StartReplay();
		}
	
		if (IsKeyPressed(KEY_P)) {
			game.TogglePause();
		}
	
		if (IsKeyPressed(KEY_G)) {
			game.ToggleGhosts();
		}
	
		if (IsKeyPressed(KEY_ESCAPE)) {
			game.ResetGame();
		}
	
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			MouseButtonPressedCallback(L);
		}
	
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			MouseButtonDownCallback(L);
		}
	
		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
			MouseButtonUpCallback(L);
		}*/
	
		if (IsFileDropped()) {
			FilePathList dropped_files = LoadDroppedFiles();
			API::FileDroppedCallback(*(dropped_files.paths));
			UnloadDroppedFiles(dropped_files);
	        }

		if (Console::GetHasEvent()) {
			const char* message = Console::GetMessage();
			API::ConsoleCallback(message);
			Console::ResetHasEvent();
		}
	}

	CloseWindow();

	Game::Quit();
}
