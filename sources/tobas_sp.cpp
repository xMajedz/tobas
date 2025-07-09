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

	API::SetCallback("Update" , "sp", [](lua_State* L) {
		auto rules = Game::GetGamerules();

		if (IsKeyPressed(KEY_F)) {
			//SaveReplay();
		}
	
		if (IsKeyPressed(KEY_R)) {
			//StartReplay();
		}
		if (IsKeyPressed(KEY_P)) {
			//TogglePause();
		}
	
		if (IsKeyPressed(KEY_G)) {
			//ToggleGhosts();
		}
	
		if (IsKeyPressed(KEY_ESCAPE)) {
			//ResetGame();
		}

		if (Game::GetFreeze()) {
			switch(Game::GetGamemode()) {
			case FREEPLAY: {
				if (IsKeyPressed(KEY_SPACE)) {
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						Game::Step();
					} else {
						Game::Step(rules.turnframes);
					}
				}
				} break;
				}
		} else {
			switch(Game::GetGamemode()) {
			case FREEPLAY: {
				if (IsKeyPressed(KEY_Z)) {
					if (IsKeyDown(KEY_LEFT_CONTROL)) {
					//Replay::PlayFrame(state.game_frame - 1);
					} else {
						if (Game::GetSelectedJointID() >= 0) {
							Game::Refreeze();
							if (IsKeyDown(KEY_LEFT_SHIFT)) {
								//selected_joint->ToggleActiveStateAlt();
							} else {
								//selected_joint->ToggleActiveState();
							}
						}
					}
				}

				if (IsKeyPressed(KEY_X)) {
					if (Game::GetSelectedJointID() >= 0) {
						Game::Refreeze();
						if (IsKeyDown(KEY_LEFT_SHIFT)) {
							//selected_joint->TogglePassiveStateAlt();
						} else {
							//selected_joint->TogglePassiveState();
						}
					}
				}

				if (IsKeyPressed(KEY_C)) {
					//selected_player->TogglePlayerPassiveStatesAlt();
					//selected_player->TogglePlayerPassiveStates();
				}

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					//SelectPlayer(gamecam.camera, MouseRay, MouseCollision);
					if (Game::GetSelectedJointID() >= 0) {
						Game::Refreeze();
						if (IsKeyDown(KEY_LEFT_SHIFT)) {
							//selected_joint->CycleStateAlt();
						} else {
							//selected_joint->CycleState();
						}
					}
				}
			} break;
			case REPLAY: {
				if (IsKeyPressed(KEY_SPACE)) {
					//StartFreeplay();
				}
			
				if (!Game::GetFreeze() && IsKeyPressed(KEY_E)) {
					//EditReplay();
				}
			} break;
			}
		}
		return 1;
	});

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
