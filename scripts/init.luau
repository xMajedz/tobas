local require = API["require"]
local ms = require("modstuff")

local showMenu = false
local showRules = false
local showMods = false
local showReplays = false
local showScripts = false
local showSettings = false

local showConsole = false

local screenWidth, screenHeight = API.GetWindowSize()

local mod, o_count, p_count, p_details, frame, reaction_count, rules, gravity, console_message

local messages = {}
local commands = {}

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
	local arg = arg:split(" ")
	Net.ClientWhisper(tonumber(arg[1]), arg[2])
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

		showConsole = showConsole == false
	end

	if not showConsole and RAYLIB.IsKeyPressed(RAYLIB["KEY_SLASH"]) then
		showConsole = showConsole == false
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
		if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
			Game.Step(1)
		else
			Game.Step(rules.turnframes)
		end
	end

	if Game.IsMode(Game["MODE_REPLAY"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_SPACE"]) then
		Game.EnterMode(Game["MODE_FREEPLAY"])
	end

	if Game.IsFreeze() and Game.IsMode(Game["MODE_REPLAY"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_E"]) then
		--kinda useless because all you need is switch to FREEPLAY at a given frame
		--Game.EnterMode(Game["MODE_REPLAY_EDIT"])
	end

	if Game.IsFreeze() and Game.IsMode(Game["MODE_FREEPLAY"]) and Game.IsSelectedJointValid() then
		if not RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_CONTROL"]) and RAYLIB.IsKeyPressed(RAYLIB["KEY_Z"]) then
			Game.Refreeze()
			if RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) then
				Game.ToggleSelectedJointActiveStateAlt()
			else
				Game.ToggleSelectedJointActiveState()
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
			--Replay.PlayFrame(Game.GetGameFrame())
		end

		if RAYLIB.IsKeyPressed(RAYLIB["KEY_C"]) then
			Game.Refreeze()
			Game.ToggleSelectedPlayerPassiveStatesAlt()
			Game.ToggleSelectedPlayerPassiveStates()
		end
	end
	end
end

function API.Draw.init()
	if showMenu then
		local offset = 10
		local button_size = 120
		local gape = (screenWidth - offset)/5 - button_size
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Rules") then
			showRules = showRules == false
		end
		offset = offset + button_size + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Mods") then
			showMods = showMods == false
		end
		offset = offset + button_size + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Replays") then
			showReplays = showReplays == false
		end
		offset = offset + button_size + gape
		if RAYGUI.GuiButton(offset, 10, button_size, 25, "Scripts") then
			showScripts = showScripts == false
		end
		offset = offset + button_size + gape
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
		RAYGUI.GuiGroupBox(screenWidth-220, 20, 200, 400, "Mods")
		local mods = RAYLIB.LoadDirectoryFiles("mods");
		for i = 1, #mods do
			local mod = mods[i]:gsub("mods/", "")
			if RAYGUI.GuiButton(screenWidth-220, 20 * (i + 1), 200, 20, mod) then
				if mod:match("^[%w%-_]+%.luau") then
					API.Reset();
					API.loadmod(mod:match("^[%w%-_]+"))
					Game.NewGame()
				end
				if mod:match("^[%w%-_]+%.tbm") then
					API.Reset();
					ms.loadmodTBM(mod:match("^[%w%-_]+"))
					Game.NewGame()
				end
			end
		end
	end

	if showReplays then
		RAYGUI.GuiGroupBox(screenWidth-220, 20, 200, 400, "Replays")
		local replays = RAYLIB.LoadDirectoryFiles("replays")
		for i = 1, #replays do
			if RAYGUI.GuiButton(screenWidth-220, 20 * (i + 1), 200, 20, replays[i]) then

			end
		end
	end

	if showScripts then
		RAYGUI.GuiGroupBox(screenWidth-220, 20, 200, 400, "Scripts")
		local scripts = RAYLIB.LoadDirectoryFiles("scripts")
		for i = 1, #scripts do
			if (RAYGUI.GuiButton(screenWidth-220, 20 * (i + 1), 200, 20, scripts[i])) then

			end
		end
	end

	if showSettings then
		RAYGUI.GuiWindowBox(screenWidth-220, 20, 200, 400, "Settings")
	end

	if showConsole then
		RAYGUI.GuiTextBox(10, screenHeight-30, screenWidth-20, 20, true)
		for i = 1, #messages do
			--RAYGUI.GuiLabel(10, screenHeight-(30+20*(#messages-i+1)), screenWidth-20, 20, messages[i])
			RAYLIB.DrawText(messages[i], 10, screenHeight-(30+20*(#messages-i+1)), 20, RAYLIB["BLACK"])
		end
	end
end

function API.Console.init(msg)
	if msg:match("^/.+") then
		local cmd = msg:gsub("^/", "")
		cmd = commands[cmd:match("^%w+")]
		cmd = cmd and cmd(msg:gsub("^/%w+%s*", ""))
	else
		print(msg)
		table.insert(messages, msg)
	end
end
