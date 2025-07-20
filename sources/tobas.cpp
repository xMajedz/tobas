#include "api.h"
#include "game.h"
#include "camera.h"
#include "replay.h"
#include "common.h"

#include "netcode_client.h"
#include "netcode_server.h"

#include <format>

using namespace raylib;
#include "raygui.h"

bool showMenu = false;
bool showConsole = false;
bool showRules = false;
bool showMods = false;
bool showReplays = false;
bool showScripts = false;
bool showSettings = false;

int console_max_text = 1024;
char console_text[1024] = { 0 };
bool console_secret_active = false;

const char* console_message = "";

void ui(float screenWidth, float screenHeight)
{
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

	if (!showMenu && GuiLabel((Rectangle){10, 30, 40, 20}, label_2.c_str())) { }

	auto label_3 = std::format("{}", round(Game::GetReactionTime() - Game::GetReactionCount()));

	if (!showMenu && GuiLabel((Rectangle){10, 50, 40, 20}, label_3.c_str())) { }

	auto label_4 = std::format("{}", console_message);

	if (!showMenu && GuiLabel((Rectangle){10, 70, 900, 20}, label_4.c_str())) { }

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

	if (showConsole) {
		GuiTextBox((Rectangle){10, screenHeight-50, screenWidth-20, 40}, console_text, console_max_text, true);
	}
}

void NewGame(void*)
{
	Replay::RecordFrame();
}
	
void NetUpdate(void* arg)
{
	dReal dt = *(dReal*)arg;

	auto rules = Game::GetGamerules();
	auto selected_player_id = Game::GetSelectedPlayerID();
	auto selected_joint_id = Game::GetSelectedJointID();

	if (IsKeyPressed(KEY_ESCAPE)) {
		showMenu = showMenu == false;
	}
	
	if (IsKeyPressed(KEY_ENTER)) {
		if (showConsole) {
			LOG(console_text)

			for (int i = 0; i < console_max_text; i += 1) {
				console_text[i] = 0; 
			}
		}

		showConsole = showConsole == false;
	}

	if (IsKeyPressed(KEY_SPACE)) {
		Client::Ready();
	}

	Client::Update(Game::GetTime(), Game::GetFrameTime());
}

void Update(void* arg)
{
	dReal dt = *(dReal*)arg;

	auto rules = Game::GetGamerules();
	auto selected_player_id = Game::GetSelectedPlayerID();
	auto selected_joint_id = Game::GetSelectedJointID();

	if (IsKeyPressed(KEY_ESCAPE)) {
		showMenu = showMenu == false;
	}
	
	if (IsKeyPressed(KEY_ENTER)) {
		if (showConsole) {
			LOG(console_text)

			for (int i = 0; i < console_max_text; i += 1) {
				console_text[i] = 0; 
			}
		}

		showConsole = showConsole == false;
	}

	if (IsKeyPressed(KEY_R)) {
		Game::EnterMode(Gamemode::REPLAY_PLAY);
	}

	if (IsKeyPressed(KEY_P)) {
		Game::TogglePause();
	}

	if (IsKeyPressed(KEY_G)) {
		Game::ToggleGhosts();
	}

	if (Game::GetFreeze()) {
		switch(Game::GetGamemode()) {
		case SELF_PLAY:
		case FREE_PLAY: {
			if (selected_joint_id >= 0) {
				auto& selected_joint = Game::GetSelectedJoint();

				if (!IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
					Game::Refreeze();
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						selected_joint.ToggleActiveStateAlt();
					} else {
						selected_joint.ToggleActiveState();
					}
				}

				if (IsKeyPressed(KEY_X)) {
					Game::Refreeze();
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						selected_joint.TogglePassiveStateAlt();
					} else {
						selected_joint.TogglePassiveState();
					}
				}

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					Game::Refreeze();
					if (IsKeyDown(KEY_LEFT_SHIFT)) {
						selected_joint.CycleStateAlt();
					} else {
						selected_joint.CycleState();
					}
				}
			}

			if (selected_player_id >= 0) {
				auto& selected_player = Game::GetSelectedPlayer();

				if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
					//Replay::PlayFrame(Game::GetGameFrame() - 1);
				}
	
				if (IsKeyPressed(KEY_C)) {
					Game::Refreeze();
					selected_player.TogglePlayerPassiveStatesAlt();
					selected_player.TogglePlayerPassiveStates();
				}
	
			}

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
		case FREE_PLAY: {

		} break;
		case REPLAY_PLAY: {
			if (IsKeyPressed(KEY_SPACE)) {
				Game::EnterMode(Gamemode::FREE_PLAY);
			}
		
			if (!Game::GetFreeze() && IsKeyPressed(KEY_E)) {
				Game::EnterMode(Gamemode::REPLAY_EDIT);
			}
		} break;
		}
	}
}

void log(const char* message)
{
	console_message = message;
}

int main()
{

	Console::SetCallback(log);

	Window::Init();
	Window::SetDrawCallback(ui);

	Game::Init();

	Game::SetCallback(CallbackType::NEWGAME, NewGame);
	Game::NewGame();
	Game::SetCallback(CallbackType::UPDATE, NetUpdate);
	//Game::SetCallback(CallbackType::UPDATE, Update);

	Gamecam::Init();

	bool running = true;
	bool host_running = false;
	
	int status = Client::Connect();

	if (status != 0) {
		Server::HostGameThread();
		Server::WaitGameThread();
		host_running = true;

		int max_attempts = 2;

		for (int attempts = 0 ; status != 0 && attempts < max_attempts; attempts += 1) {
			status = Client::Connect();
		}
	}

	while (running) {
		Window::Update(Gamecam::Get());

		if (0 > Game::GetSelectedPlayerID()) {
			Gamecam::UpdateSpectatorcam(Game::GetFreeze(), Game::GetPlayers());
		} else {
			Gamecam::UpdatePlaycam(Game::GetFreeze(), Game::GetSelectedPlayer());
		}

		Gamecam::Update();

		Game::Update(Game::GetFrameTime());

		Window::Draw(Gamecam::Get());

		if (WindowShouldClose()) {
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
