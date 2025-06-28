#include "api.h"
#include "api_raylib.h"
#include "api_raymath.h"
#include "api_callbacks.h"

#include <iostream>

#define PRINT(X) std::cout << "-- " << X << " --" << std::endl;
#define PRINT_A PRINT("A")
#define PRINT_B PRINT("B")
#define PRINT_C PRINT("C")

Window window;
Console console;
Game game;
API api(&game);

int main()
{
	window.width = 1600;
	window.height = 900;

	SetTraceLogLevel(LOG_ERROR);
	InitWindow(window.width, window.height, "TOBAS");

	lua_State* L = luaL_newstate();
	luaopen_api_main(L);
	luaopen_api_game(L);
	luaopen_api_raylib(L);
	luaopen_api_raymath(L);
	luaopen_api_callbacks(L);
	luaL_openlibs(L);
	luaL_sandbox(L);

	dInitODE();
	game.world = dWorldCreate();

	static auto nearCallback = [L](void* unsafe, dGeomID o1, dGeomID o2) {
		game.NearCallback(o1, o2);
		NearCallback(L, game.collision);
	};

	game.nearCallback = [](void* unsafe, dGeomID o1, dGeomID o2) {
		nearCallback(unsafe, o1, o2);
	};

	loadscript(L, "init");

	game.NewGame();
	Ray MouseRay = {0};
	RayCollision MouseCollision = {0};

	game.gamecam.camera = { 0 };
	game.gamecam.camera.up = (Vector3){ 0.00, 0.00, 1.00 };
	game.gamecam.camera.fovy = 45.00;
	game.gamecam.camera.projection = CAMERA_PERSPECTIVE;

	game.gamecam.CameraZoom = (Vector3){ 0.00, -5.00, 0.00 };
	game.gamecam.CameraOffset = (Vector3){ 0.00, 0.00, 0.00 };

	SetExitKey(0);
	while (!WindowShouldClose()) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));
		
		Player* selected_player;
		Joint* selected_joint;

		if (game.state.selected_player != "NONE") {
			selected_player = &(game.players[game.state.selected_player]);
			selected_joint = &(selected_player->joint[game.state.selected_joint]);
			//game.UpdatePlaycam(&game.gamecam);
			game.SelectJoint(game.gamecam.camera, MouseRay, MouseCollision);
		}
		
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
			game.SelectPlayer(game.gamecam.camera, MouseRay, MouseCollision);
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
		}
	
		if (IsFileDropped()) {
			FilePathList dropped_files = LoadDroppedFiles();
			FileDroppedCallback(L, *(dropped_files.paths));
			UnloadDroppedFiles(dropped_files);
	        }
	
		game.UpdateFrame();
	
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode3D(game.gamecam.camera);
				Draw3DCallback(L);
				game.DrawFrame();
			EndMode3D();
			Draw2DCallback(L);
		EndDrawing();
	}

	game.EndGame();
	CloseWindow();
	lua_close(L);
}
