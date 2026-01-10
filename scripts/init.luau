local require = API["require"]
local ms = require("modstuff")

local showMenu = false
local showRules = false
local showMods = false

local showReplays = false
local showReplaysSaving = false

local showScripts = false
local showSettings = false

local showConsole = false
local showConsoleMessages = false

local screenWidth, screenHeight = API.GetWindowSize()

local mod, o_count, p_count, p_details, frame, reaction_count, rules, gravity, console_message

local messages = {}
local commands = {}

local loaded_scripts = {}

for k,v in require("intro") do
	table.insert(messages, k .. ": " .. v)
end

showConsoleMessages = true

RAYGUI.GuiSetStyle(0, 16, 20)

function commands.loadscript(script_path) 
	API.loadscript(script_path)
end

function commands.loadmod(mod_path) 
	API.loadmod(mod_path)
end

function commands.restart() 
	Game.Restart()
end

function commands.clear() 
	messages = {}
end

function commands.whisper(arg) 
if Net then
	local arg = arg:split(" ")
	Net.ClientWhisper(tonumber(arg[1]), arg[2])
end
end

function commands.mods(mod_path) 
	local mods = RAYLIB.LoadDirectoryFiles("mods");
	table.insert(messages, #mods .. " mod")

	for i = 1, #mods do
		local mod = mods[i]:gsub("mods[/\\]", "")
		table.insert(messages, mod)
		showConsoleMessages = true
	end
end

commands["ls"] = commands["loadscript"]
commands["lm"] = commands["loadmod"]
commands["rt"] = commands["restart"]

function API.NewGame.init()
	mod = Game.GetMod()
	o_count = Game.GetObjectCount()
	p_count = Game.GetPlayerCount()
	p_details = ""
	for i = 0, p_count - 1 do
		local b_count = Game.GetPlayerBodyCount(i)
		local j_count = Game.GetPlayerJointCount(i)
		p_details = p_details .. "p" .. (i + 1) .. ": "
		p_details = p_details .. "b:" .. b_count .. " "
		p_details = p_details .. "j:" .. j_count .. " "
	end
	rules = Game.GetGamerules()
	gravity = `{rules.gravity.x}, {rules.gravity.y}, {rules.gravity.z}`
end

function API.Update.init(dt)
	frame = Game.GetGameFrame()
	reaction_count = math.round(Game.GetReactionTime() - Game.GetReactionCount())

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_ENTER"]) then
		if showConsole and RAYGUI["_VAR"]["GuiTextBox"] ~= "" then
			if Net then
				Net.ClientEcho(RAYGUI["_VAR"]["GuiTextBox"])
			else
				API.log(RAYGUI["_VAR"]["GuiTextBox"])
			end

			RAYGUI["_VAR"]["GuiTextBox"] = ""
		end

		if showConsole then
			showConsoleMessages = false
			showConsole = false
		else
			showConsoleMessages = true
			showConsole = true
		end
	end

	if not showConsole and RAYLIB.IsKeyPressed(RAYLIB["KEY_SLASH"]) then
		showConsoleMessages = true 
		showConsole = true
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_TAB"]) then
		API.log("TAB")
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_F"]) then
		showReplaysSaving = true
		Replay.Save("tempreplayfile.bin")
	end
	
if Net then
else

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_R"]) then
		Game.EnterMode(Game["MODE_REPLAY"])
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_P"]) then
		Game.TogglePause()
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_G"]) then
		Game.ToggleGhosts()
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_ESCAPE"]) then
		showMenu = showMenu == false
	end

	if Game.IsFreeze() and not Game.IsMode(Game["MODE_REPLAY"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_SPACE"]) then
		Game.Step((RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) and 1) or Game.Step(rules.turnframes))
	end

	if Game.IsMode(Game["MODE_REPLAY"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_SPACE"]) then
		Game.EnterMode(Game["MODE_FREEPLAY"])
	end

	if Game.IsMode(Game["MODE_REPLAY"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_E"]) then
		Game.EnterMode(Game["MODE_REPLAY_EDIT"])
	end

	if Game.IsFreeze() and Game.IsMode(Game["MODE_FREEPLAY"]) and Game.IsSelectedJointValid() then
		if not RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_CONTROL"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_Z"]) then
			Game.Refreeze()

			if loaded_scripts["variable_joint_velocity"] == nil then
				if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
					Game.ToggleSelectedJointActiveStateAlt()
				else
					Game.ToggleSelectedJointActiveState()
				end
			end
		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_X"]) then
			Game.Refreeze()

			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				Game.ToggleSelectedJointPassiveStateAlt()
			else
				Game.ToggleSelectedJointPassiveState()
			end
		end

		if RAYLIB.IsMouseButtonPressed(RAYLIB["MOUSE_BUTTON_LEFT"]) then
			Game.Refreeze()
			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				Game.CycleSelectedJointStateAlt()
			else
				Game.CycleSelectedJointState()
			end
		end	

	end

	if Game.IsFreeze() and Game.IsMode(Game["MODE_FREEPLAY"]) and Game.IsSelectedPlayerValid() then
		if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_CONTROL"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_Z"]) then
			Game.Refreeze()
			Game.UndoSelectedPlayerMove()
		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_C"]) then
			Game.Refreeze()
			Game.ToggleSelectedPlayerPassiveStatesAlt()
			Game.ToggleSelectedPlayerPassiveStates()
		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_V"]) then
			Game.Refreeze()
			Game.ToggleSelectedPlayerBodyStates()
		end
	end
end
end

function API.Draw.init()
	if showMenu then
		local offset = 10
		local button_size = 100
		local gape = (screenWidth - button_size - 2 * offset)/4
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Rules") then
			showRules = showRules == false
		end
		offset = offset + gape 
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Mods") then
			showMods = showMods == false
		end
		offset = offset + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Replays") then
			showReplays = showReplays == false
		end
		offset = offset + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Scripts") then
			showScripts = showScripts == false
		end
		offset = offset + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Settings") then
			showSettings = showSettings == false
		end
	else
		RAYGUI.GuiLabel(10, 10, screenWidth-10, 10, `{mod}: o:{o_count} p:{p_count} {p_details}`)
		RAYGUI.GuiLabel(10, 30, screenWidth-10, 10, `{frame}`)
		RAYGUI.GuiLabel(10, 50, screenWidth-10, 10, `{reaction_count}`)
	end

	if showRules then
		RAYGUI.GuiGroupBox(15, 50, 200, 380, "Rules")
		RAYGUI.GuiLabel(20,  50, 200, 40, rules.mod)
		RAYGUI.GuiLabel(20,  70, 200, 40, gravity)
		RAYGUI.GuiLabel(20,  90, 200, 40, `{rules.numplayers}`)
		RAYGUI.GuiLabel(20, 110, 200, 40, `{rules.turnframes}`)
		RAYGUI.GuiLabel(20, 130, 200, 40, `{rules.max_contacts}`)
		RAYGUI.GuiLabel(20, 150, 200, 40, `{rules.reaction_time}`)
		RAYGUI.GuiLabel(20, 170, 200, 40, `{rules.engagedistance}`)
		RAYGUI.GuiLabel(20, 190, 200, 40, `{rules.engageheight}`)
		RAYGUI.GuiLabel(20, 210, 200, 40, `{rules.friction}`)
	end

	if showMods then 
		RAYGUI.GuiGroupBox(15, 50, 200, 400, "Mods")
		local mods = RAYLIB.LoadDirectoryFiles("mods");
		local i = 1
		local j = 1
		while i <= #mods do
			local mod = mods[i]:gsub("mods[/\\]", "")
			if not mod:match(".+%.txt") then
				if RAYGUI.GuiButton(15, 30 + 20 * (j + 1), 200, 20, mod) then
					if mod:match("^[%.%w%-_]+%.luau") then
						API.Reset()
						API.loadmod(mod:match("^[%w%-_]+"))
						Game.NewGame()
					end
					if mod:match("^[%.%w%-_]+%.tbm") then
						API.Reset()
						ms.loadmodTBM2(mod:match("^[%w%-_]+"))
						Game.NewGame()
					end
	
					showMods = false
	
				end

				j = j + 1
			end

			i = i + 1
		end
	end

	if showReplays then
		RAYGUI.GuiGroupBox(15, 50, 200, 400, "Replays")
		local replays = RAYLIB.LoadDirectoryFiles("replays")
		for i = 1, #replays do
			local replay = replays[i]:gsub("replays[/\\]", "")
			if RAYGUI.GuiButton(15, 30 + 20 * (i + 1), 200, 20, replay) then
				if replay:match("^[%.%w%-_]+%.rpl") then
					Replay.Load(replay)
				end

				showReplays = false
			end
		end
	end

	if showScripts then
		RAYGUI.GuiGroupBox(15, 50, 200, 400, "Scripts")
		local scripts = RAYLIB.LoadDirectoryFiles("scripts")
		for i = 1, #scripts do
			local script = scripts[i]:gsub("scripts[/\\]", "")
			if (RAYGUI.GuiButton(15, 30 + 20 * (i + 1), 200, 20, script)) then
				script = script:gsub("%..+", "")
				API.loadscript(script)
				loaded_scripts[script] = 1

				showScripts = false
			end
		end
	end

	if showSettings then
		RAYGUI.GuiGroupBox(15, 50, 200, 400, "Settings")
		--RAYGUI.GuiWindowBox(screenWidth-220, 20, 200, 400, "Settings")
	end

	if showConsole then
		RAYGUI.GuiTextBox(10, screenHeight-30, screenWidth-20, 20, true)
	end
	
	if showConsoleMessages then
		for i in messages do
			RAYLIB.DrawText(messages[i], 10, screenHeight-(30+20*(#messages-i+1)), 20, RAYLIB["BLACK"])
		end
	end

	if showReplaysSaving then
		if (RAYGUI.GuiButton(10, 80 , 200, 20, "Save")) then
			showReplaysSaving = false

			if RAYGUI["_VAR"]["GuiTextBox"] ~= "" then
				Replay.Save(RAYGUI["_VAR"]["GuiTextBox"] .. ".bin")
			else
				Replay.Save("utitiledreplayfile.bin")
			end

			RAYGUI["_VAR"]["GuiTextBox"] = ""
		end

		RAYGUI.GuiTextBox(10, 40, 200, 20, true)
	end
end

function API.Console.init(msg)
	if msg:match("^/.*") then
		local cmd = msg:gsub("^/", "")
		cmd = commands[cmd:match("^%w+")]
		cmd = cmd and cmd(msg:gsub("^/%w+%s*", ""))
	else
		table.insert(messages, msg)
		showConsoleMessages = true
	end
end
