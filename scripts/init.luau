local require = API["require"]
local ms = require("modstuff")

local showGui = true

local showMenu = false
local showRules = false
local showMods = false

local showReplays = false
local showReplaysSaving = false

local showScripts = false
local showSettings = false

local showConsole = false
local showConsoleMessages = false

local screenWidth, screenHeight = RAYLIB.GetScreenWidth(), RAYLIB.GetScreenHeight()

local Data = {}
Data["GravityX_GuiTextBox"] = API.CreateData(1024)
Data["GravityY_GuiTextBox"] = API.CreateData(1024)
Data["GravityZ_GuiTextBox"] = API.CreateData(1024)

Data["Players_GuiTextBox"] = API.CreateData(1024)
Data["EngageHeight_GuiTextBox"] = API.CreateData(1024)
Data["EngageDistance_GuiTextBox"] = API.CreateData(1024)

Data["TurnFrames_GuiTextBox"] = API.CreateData(1024)
Data["ReactionTime_GuiTextBox"] = API.CreateData(1024)
Data["MaxContacts_GuiTextBox"] =  API.CreateData(1024)
Data["Friction_GuiTextBox"] = API.CreateData(1024)
Data["Bounce_GuiTextBox"] = API.CreateData(1024)

Data["Console_GuiTextBox"] = API.CreateData(1024)
Data["ReplayName_GuiTextBox"] = API.CreateData(1024)

local editValue = {}
editValue["GravityX_GuiTextBox"] = false
editValue["GravityY_GuiTextBox"] = false
editValue["GravityZ_GuiTextBox"] = false

editValue["Players_GuiTextBox"] = false
editValue["EngageHeight_GuiTextBox"] = false
editValue["EngageDistance_GuiTextBox"] = false
editValue["TurnFrames_GuiTextBox"] = false
editValue["ReactionTime_GuiTextBox"] = false
editValue["MaxContacts_GuiTextBox"] =  false
editValue["Friction_GuiTextBox"] = false
editValue["Bounce_GuiTextBox"] = false

editValue["Console_GuiTextBox"] = false
editValue["ReplayName_GuiTextBox"] = false

local replay_slider_value = API.CreateData(4)

local mod, o_count, p_count, p_details, frame, contact_count, reaction_count, rules, gravity, console_message

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

	API.StringToData(`{rules.gravity.x}`, Data["GravityX_GuiTextBox"])
	API.StringToData(`{rules.gravity.y}`, Data["GravityY_GuiTextBox"])
	API.StringToData(`{rules.gravity.z}`, Data["GravityZ_GuiTextBox"])
	API.StringToData(`{rules.numplayers}`, Data["Players_GuiTextBox"])
	API.StringToData(`{rules.engageheight}`, Data["EngageHeight_GuiTextBox"])
	API.StringToData(`{rules.engagedistance}`, Data["EngageDistance_GuiTextBox"])
	API.StringToData(`{rules.turnframes}`, Data["TurnFrames_GuiTextBox"])
	API.StringToData(`{rules.reaction_time}`, Data["ReactionTime_GuiTextBox"])
	API.StringToData(`{rules.max_contacts}`, Data["MaxContacts_GuiTextBox"])
	API.StringToData(`{rules.friction}`, Data["Friction_GuiTextBox"])
	API.StringToData(`{rules.bounce}`, Data["Bounce_GuiTextBox"])
end

function API.Update.init(dt)
	frame = Game.GetGameFrame()
	contact_count = Game.GetContactCount()
	if rules then
		reaction_count = math.round(rules.reaction_time - Game.GetReactionCount())
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_ENTER"]) then
		local s = API.StringFromData(Data["Console_GuiTextBox"])

		if showConsole and s ~= "" then
			if Net then
				Net.ClientEcho(s)
			else
				API.log(s)
			end

			API.StringToData("", Data["Console_GuiTextBox"])
		end

		showConsoleMessages = (showConsole and showConsoleMessages) == false
		showConsole = showConsole == false
	end

	if not showConsole and RAYLIB.IsKeyPressed(RAYLIB["KEY_SLASH"]) then
		showConsoleMessages = true 
		showConsole = true
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_TAB"]) then
		showGui = showGui == false
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_F"]) then
		showReplaysSaving = true
		Replay.Export("tempreplayfile.bin")
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
		local step_size = (RAYLIB.IsKeyDown(RAYLIB["KEY_LEFT_SHIFT"]) and 1) or rules.turnframes
		Game.Step(step_size)
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
if showGui then
	if showMenu then
		local offset = 10
		local button_size = 100
		local gape = (screenWidth - 2 * button_size + 10)/3
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

		if RAYGUI.GuiButton(screenWidth - 70, 10, 25, 25, "#142#") then
			showSettings = showSettings == false
		end
		if RAYGUI.GuiButton(screenWidth - 35, 10, 25, 25, "#113#") then
			Game.Quit()
		end
	else
		if rules then
			local offset = 10
			RAYGUI.GuiLabel(10, offset, screenWidth-10, 10, `{mod}: o:{o_count} p:{p_count} {p_details}`)
			offset = offset + 20
			RAYGUI.GuiLabel(10, offset, screenWidth-10, 10, `{frame}`)
			offset = offset + 20
			if rules.reaction_time > 0 then
				RAYGUI.GuiLabel(10, offset, screenWidth-10, 10, `{reaction_count}`)
				offset = offset + 20
			end
			RAYGUI.GuiLabel(10, offset, screenWidth-10, 10, `{contact_count}`)
		end
	end

	if showRules then
		if RAYGUI.GuiWindowBox(10, 40, 400, 400, "Rules") then
			showRules = false
		end

		--RAYGUI.GuiLabel(20,  50, 200, 40, rules.mod)

		RAYGUI.GuiLabel(20, 75, 200, 40, "Gravity")

		if RAYGUI.GuiTextBox(20, 110, 60, 20, Data["GravityX_GuiTextBox"], 1024, editValue["GravityX_GuiTextBox"]) then
			editValue["GravityX_GuiTextBox"] = editValue["GravityX_GuiTextBox"] == false
		end

		if RAYGUI.GuiTextBox(85, 110, 60, 20, Data["GravityY_GuiTextBox"], 1024, editValue["GravityY_GuiTextBox"]) then
			editValue["GravityY_GuiTextBox"] = editValue["GravityY_GuiTextBox"] == false
		end

		if RAYGUI.GuiTextBox(150, 110, 60, 20, Data["GravityZ_GuiTextBox"], 1024, editValue["GravityZ_GuiTextBox"]) then
			editValue["GravityZ_GuiTextBox"] = editValue["GravityZ_GuiTextBox"] == false
		end

		RAYGUI.GuiLabel(20, 125, 170, 40, "Max Contacts")

		if RAYGUI.GuiTextBox(20, 160, 60, 20, Data["MaxContacts_GuiTextBox"], 1024, editValue["MaxContacts_GuiTextBox"]) then
			editValue["MaxContacts_GuiTextBox"] = editValue["MaxContacts_GuiTextBox"] == false
		end
	
		RAYGUI.GuiLabel(20, 175, 200, 40, "Friction")

		if RAYGUI.GuiTextBox(20, 210, 60, 20, Data["Friction_GuiTextBox"], 1024, editValue["Friction_GuiTextBox"]) then
			editValue["Friction_GuiTextBox"] = editValue["Friction_GuiTextBox"] == false
		end

		RAYGUI.GuiLabel(20, 225, 200, 40, "Bounce")

		if RAYGUI.GuiTextBox(20, 260, 60, 20, Data["Bounce_GuiTextBox"], 1024, editValue["Bounce_GuiTextBox"]) then
			editValue["Bounce_GuiTextBox"] = editValue["Bounce_GuiTextBox"] == false
		end

		RAYGUI.GuiLabel(20, 275, 200, 40, "Players")

		if RAYGUI.GuiTextBox(20, 310, 60, 20, Data["Players_GuiTextBox"], 1024, editValue["Players_GuiTextBox"]) then
			editValue["Players_GuiTextBox"] = editValue["Players_GuiTextBox"] == false
		end

		RAYGUI.GuiLabel(20, 325, 200, 40, "Engage Height")

		if RAYGUI.GuiTextBox(20, 360, 60, 20, Data["EngageHeight_GuiTextBox"], 1024, editValue["EngageHeight_GuiTextBox"]) then
			editValue["EngageHeight_GuiTextBox"] = editValue["EngageHeight_GuiTextBox"] == false
		end
		
		RAYGUI.GuiLabel(20, 375, 200, 40, "Engage Distance")

		if RAYGUI.GuiTextBox(20, 410, 60, 20, Data["EngageDistance_GuiTextBox"], 1024, editValue["EngageDistance_GuiTextBox"]) then
			editValue["EngageDistance_GuiTextBox"] = editValue["EngageDistance_GuiTextBox"] == false
		end
		
		RAYGUI.GuiLabel(220, 75, 200, 40, "Turn Frames")

		if RAYGUI.GuiTextBox(220, 110, 60, 20, Data["TurnFrames_GuiTextBox"], 1024, editValue["TurnFrames_GuiTextBox"]) then
			editValue["TurnFrames_GuiTextBox"] = editValue["TurnFrames_GuiTextBox"] == false
		end

		RAYGUI.GuiLabel(220, 125, 200, 40, "Reaction Time")

		if RAYGUI.GuiTextBox(220, 160, 60, 20, Data["ReactionTime_GuiTextBox"], 1024, editValue["ReactionTime_GuiTextBox"]) then
			editValue["ReactionTime_GuiTextBox"] = editValue["ReactionTime_GuiTextBox"] == false
		end

		if RAYGUI.GuiButton(220, screenHeight-80, 100, 20, "Apply") then
			local x = tonumber(API.StringFromData(Data["GravityX_GuiTextBox"]))
			local y = tonumber(API.StringFromData(Data["GravityY_GuiTextBox"]))
			local z = tonumber(API.StringFromData(Data["GravityZ_GuiTextBox"]))
			Game.SetGravity(x, y, z)

			local mc = tonumber(API.StringFromData(Data["MaxContacts_GuiTextBox"]))
			Game.SetMaxContacts(mc)

			local friction = tonumber(API.StringFromData(Data["Friction_GuiTextBox"]))
			Game.SetFriction(friction)

			local bounce = tonumber(API.StringFromData(Data["Bounce_GuiTextBox"]))
			Game.SetBounce(bounce)

			local tf = tonumber(API.StringFromData(Data["TurnFrames_GuiTextBox"]))
			rules.turnframes = tf
			Game.SetTurnFrames(tf)

			local rt = tonumber(API.StringFromData(Data["ReactionTime_GuiTextBox"]))
			rules.reaction_time = rt
			Game.SetReactionTime(rt)

			if true then	
				Game.NewGame()
			end
		end
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
						Game.Reset()
						Game.ImportMod()
						Game.NewGame()
					end
					if mod:match("^[%.%w%-_]+%.tbm") then
						API.Reset()
						ms.loadmodTBM2(mod:match("^[%w%-_]+"))
						Game.Reset()
						Game.ImportMod()
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
					Replay.Import(replay)
					--[[
					local mod = Replay.GetMod()
					if mod:match("^CONVERTED_.+") then
						API.Reset()
						local mod_name = mod:gsub("CONVERTED_", "")
						ms.loadmodTBM2(mod_name)
						Game.ImportMod()
						Game.NewGame()
					end
					Replay.Import(replay)
					]]
					Game.EnterMode(Game["MODE_REPLAY"])
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

		local offset = 50

		for i, res in {"800x450", "1600x900"} do
			local width = res:gsub("x.+", "")
			local height = res:gsub(".+x", "")

			offset = offset + 20

			if (RAYGUI.GuiButton(15, offset, 200, 20, res)) then
				RAYLIB.SetWindowSize(width, height)
				screenWidth, screenHeight = width, height
			end

		end

		if (RAYGUI.GuiButton(15, offset + 20, 200, 20, "Fullscreen")) then
			RAYLIB.ToggleFullscreen()
			screenWidth, screenHeight = RAYLIB.GetScreenWidth(), RAYLIB.GetScreenHeight()
		end
	end

	local offset = screenHeight-30

	if Game.IsMode(Game["MODE_REPLAY"])then
		API.NumberToData(frame/Replay.GetMaxFrame(), replay_slider_value)
		RAYGUI.GuiSliderBar(10, offset, screenWidth-20, 20, "", "", replay_slider_value, 0.00, 1.00)
		offset = offset - 30
	end

	if showReplaysSaving then
		if (RAYGUI.GuiButton(10, offset, screenWidth-20, 20, "Save")) then
			showReplaysSaving = false

			local text = API.StringFromData(Data["ReplayName_GuiTextBox"])

			if text ~= "" then
				Replay.Export(text .. ".bin")
			else
				Replay.Export("utitiledreplayfile.bin")
			end

			API.StringToData("", Data["ReplayName_GuiTextBox"])
		end

		offset = offset - 30

		local data = Data["ReplayName_GuiTextBox"]
		local edit = editValue["ReplayName_GuiTextBox"]

		if RAYGUI.GuiTextBox(10, offset, screenWidth-20, 20, data, 1024, edit) then
			editValue["ReplayName_GuiTextBox"] = editValue["ReplayName_GuiTextBox"] == false
		end

		offset = offset - 30
	end

	if showConsole and RAYGUI.GuiTextBox(10, offset, screenWidth-20, 20, Data["Console_GuiTextBox"], 1024, editValue["Console_GuiTextBox"]) then
			editValue["Console_GuiTextBox"] = editValue["Console_GuiTextBox"] == false
	end
	
	if showConsoleMessages then
		for i in messages do
			RAYLIB.DrawText(messages[i], 10, offset-20*(#messages-i+1), 20, RAYLIB["BLACK"])
		end
	end
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
