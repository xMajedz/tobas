#include "api.h"
#include "game.h"
#include "camera.h"
#include "replay.h"
#include "common.h"

#include <format>

using namespace raylib;
#include "raygui.h"

void Draw(float screenWidth, float screenHeight)
{
	API::DrawCallback();
}

void NewGame(void*)
{
	Replay::RecordFrame();
	API::NewGameCallback();
}
	
void Update(void* arg)
{
	dReal dt = *(dReal*)arg;

	auto rules = Game::GetGamerules();
	auto selected_player_id = Game::GetSelectedPlayerID();
	auto selected_joint_id = Game::GetSelectedJointID();

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

	API::UpdateCallback(dt);
}

void log(const char* message)
{
	API::ConsoleCallback(message);
}

void log_ode(int errnum, const char* msg, va_list ap)
{
	API::ConsoleCallback(TextFormat("%d: %s", errnum, msg));
}

int main()
{
	dSetErrorHandler(log_ode);
	dSetDebugHandler(log_ode);
	dSetMessageHandler(log_ode);

	Console::SetCallback(log);

	Window::Init();
	Window::SetDrawCallback(Draw);

	Game::Init();
	Game::SetCallback(CallbackType::NEWGAME, NewGame);
	Game::NewGame();
	Game::SetCallback(CallbackType::UPDATE, Update);

	Gamecam::Init();

	bool running = true;

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
	
		running = !WindowShouldClose();

		Console::Update();
	}

	Game::Quit();
}
