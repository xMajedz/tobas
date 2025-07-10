#include "api.h"
#include "game.h"
#include "camera.h"

#include "raygui.h"
#include <format>

bool running = true;
bool showMenu = false;
bool showRules = false;
bool showMods = false;
bool showReplays = false;
bool showScripts = false;
bool showSettings = false;

void ui(RenderTexture2D bg, RenderTexture2D fg, float screenWidth, float screenHeight)
{
	DrawTextureRec(bg.texture, {0, 0, screenWidth, -screenHeight}, {0, 0}, WHITE);
	DrawTextureRec(fg.texture, {0, 0, screenWidth, -screenHeight}, {0, 0}, WHITE);

	GuiSetStyle(0, 16, 20);
	
	auto p_count = Game::GetPlayerCount();
	std::string p_extras = "";
	for (int i = 0; i < p_count; i += 1) {
		auto b_count = Game::GetPlayerBodyCount (i);
		auto j_count = Game::GetPlayerJointCount(i);
		p_extras.append(std::format("P{}: B:{} J:{} ", i + 1,  b_count, j_count));
	}
	auto label_1 = std::format("{} O:{} P:{} {}",
		Game::GetMod(),
		Game::GetObjectCount(),
		p_count,
		p_extras
	);

	if (!showMenu && GuiLabel((Rectangle){10, 10, screenWidth-10, 10}, label_1.c_str())) {
		showRules = false;
		showMods = false;
		showReplays = false;
		showScripts = false;
		showSettings = false;
	}

	auto label_2 = std::format("{}", Game::GetGameFrame());

	if (!showMenu && GuiLabel((Rectangle){10, 30, 20, 20}, label_2.c_str())) { }

	auto label_3 = std::format("{}", Game::GetReactionTime() - Game::GetReactionCount());

	if (!showMenu && GuiLabel((Rectangle){10, 50, 20, 20}, label_3.c_str())) { }

	if (showMenu && GuiButton((Rectangle){20, 20, 200, 25}, "Rules")) {
		showRules = showRules == false;
	}

	if (showMenu && GuiButton((Rectangle){20, 45, 200, 25}, "Mods")) {
		showMods = showMods == false;
	}

	if (showMenu && GuiButton((Rectangle){20, 70, 200, 25}, "Replays")) {
		showReplays = showReplays == false;
	}

	if (showMenu && GuiButton((Rectangle){20, 95, 200, 25}, "Scripts")) {
		showScripts = showScripts == false;
	}

	if (showMenu && GuiButton((Rectangle){20, 120, 200, 25}, "Settings")) {
		showSettings = showSettings == false;
	}

	if (showRules) {
		GuiGroupBox((Rectangle){screenWidth-220, 20, 200, 400}, "Rules");
		auto rules = Game::GetGamerules();
		GuiLabel((Rectangle){screenWidth-200, 20, 200, 40}, rules.mod.data());

		auto gravity = std::format("{}, {}, {}", rules.gravity.x, rules.gravity.y, rules.gravity.z);
		GuiLabel((Rectangle){screenWidth-200, 40, 200, 40}, gravity.c_str());

		auto numplayers = std::format("{}", rules.numplayers);
		GuiLabel((Rectangle){screenWidth-200, 60, 200, 40}, numplayers.c_str());

		auto turnframes = std::format("{}", rules.turnframes);
		GuiLabel((Rectangle){screenWidth-200, 80, 200, 40}, turnframes.c_str());

		auto max_contacts = std::format("{}", rules.max_contacts);
		GuiLabel((Rectangle){screenWidth-200, 100, 200, 40}, max_contacts.c_str());

		auto reaction_time = std::format("{}", rules.reaction_time);
		GuiLabel((Rectangle){screenWidth-200, 120, 200, 40}, reaction_time.c_str());

		auto engagedistance = std::format("{}", rules.engagedistance);
		GuiLabel((Rectangle){screenWidth-200, 140, 200, 40}, engagedistance.c_str());

		auto engageheight = std::format("{}", rules.engageheight);
		GuiLabel((Rectangle){screenWidth-200, 160, 200, 40}, engageheight.c_str());

		auto friction = std::format("{}", rules.friction);
		GuiLabel((Rectangle){screenWidth-200, 180, 200, 40},  friction.c_str());
	}

	if (showMods) {
		GuiGroupBox((Rectangle){screenWidth-220, 20, 200, 400}, "Mods");
		auto mods = LoadDirectoryFiles("mods");
		for (int i = 0; i < mods.count; i += 1) {
			if (GuiButton((Rectangle){screenWidth-220, 20 * (i + 1), 200, 20}, mods.paths[i])) {

			}
		}
		UnloadDirectoryFiles(mods);
	}

	if (showReplays) {
		GuiGroupBox((Rectangle){screenWidth-220, 20, 200, 400}, "Replays");
		auto replays = LoadDirectoryFiles("replays");
		for (int i = 0; i < replays.count; i += 1) {
			if (GuiButton((Rectangle){screenWidth-220, 20 * (i + 1), 200, 20}, replays.paths[i])) {

			}
		}
		UnloadDirectoryFiles(replays);
	}

	if (showScripts) {
		GuiGroupBox((Rectangle){screenWidth-220, 20, 200, 400}, "Scripts");
		auto scripts = LoadDirectoryFiles("scripts");
		for (int i = 0; i < scripts.count; i += 1) {
			if (GuiButton((Rectangle){screenWidth-220, 20 * (i + 1), 200, 20}, scripts.paths[i])) {

			}
		}
		UnloadDirectoryFiles(scripts);
	}

	if (showSettings) {
		GuiWindowBox((Rectangle){screenWidth-220, 20, 200, 400}, "Settings");
	}
}

int main()
{
	auto screenWidth  = Window::GetWidth  ();
	auto screenHeight = Window::GetHeight ();
	SetTraceLogLevel(LOG_ERROR);
	InitWindow(screenWidth, screenHeight, "TOBAS");

	Game::Init();
	Game::NewGame();

	API::SetCallback("Update" , "sp", [](lua_State* L) {
		auto rules = Game::GetGamerules();

		if (IsKeyPressed(KEY_ESCAPE)) {
			showMenu = showMenu == false;
		}
		
		if (IsKeyPressed(KEY_ENTER)) {
			//showMenu = showMenu == false;
		}

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

	auto bg = LoadRenderTexture(screenWidth, screenHeight);
	auto fg = LoadRenderTexture(screenWidth, screenHeight);
	
	SetExitKey(0);
	while (running) {
		SetWindowTitle(TextFormat("TOBAS %dFPS", GetFPS()));

		if (WindowShouldClose()) {
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
			EndMode3D();
		EndTextureMode();
		BeginTextureMode(fg);
			BeginMode3D(camera);
			ClearBackground(Fade(WHITE, 0.00));
			Game::Draw();
			EndMode3D();
		EndTextureMode();
		BeginDrawing();

			ui(bg, fg, screenWidth, screenHeight);

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

	UnloadRenderTexture(bg);
	UnloadRenderTexture(fg);

	CloseWindow();
}
