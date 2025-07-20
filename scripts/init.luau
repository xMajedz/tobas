local require = API["require"]
local ms = require("modstuff")

local loadmodTBM = ms["loadmodTBM"]
--loadmodTBM("shootboxing15")
local loadmod = API["loadmod"]
loadmod("shootboxing15")

local showMenu = false
local showRules = false
local showMods = false
local showReplays = false
local showScripts = false
local showSettings = false

local showConsole = false

local screenWidth, screenHeight = API.GetWindowSize()

local mod, o_count, p_count, p_details, frame, reaction_count, rules, gravity, console_message

local command = {}

function command.rt() 
	print("rt")
end

RAYGUI.GuiSetStyle(0, 16, 20)

function API.NewGame.init()
	mod = Game.GetMod()
	o_count = Game.GetObjectCount()
	p_count = Game.GetPlayerCount()
	p_details = ""
	for i = 0, p_count - 1 do
		local b_count = Game.GetPlayerBodyCount(i)
		local j_count = Game.GetPlayerJointCount(i)
		p_details = p_details .. "p" .. i .. ": "
		p_details = p_details .. "b:" .. b_count .. " "
		p_details = p_details .. "j:" .. j_count .. " "
	end

	rules = Game.GetGamerules()
	gravity = `{rules.gravity.x}, {rules.gravity.y}, {rules.gravity.z}`
end

function API.Update.init(dt)
	frame = Game.GetGameFrame()
	reaction_count = math.round(Game.GetReactionTime() - Game.GetReactionCount())

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_ESCAPE"]) then
		showMenu = showMenu == false
	end

	if RAYLIB.IsKeyPressed(RAYLIB["KEY_ENTER"]) then
		if showConsole and RAYGUI["_VAR"]["GuiTextBox"] ~= "" then
			API.log(RAYGUI["_VAR"]["GuiTextBox"])
			RAYGUI["_VAR"]["GuiTextBox"] = ""
		end

		showConsole = showConsole == false
	end
end

function API.Draw.init()
	if showMenu then
		if RAYGUI.GuiButton(screenWidth-80, 20, screenWidth-40, 25, "Rules") then
			showRules = showRules == false
		end
	
		if RAYGUI.GuiButton(20, 45, 200, 25, "Mods") then
			showMods = showMods == false
		end
	
		if RAYGUI.GuiButton(20, 70, 200, 25, "Replays") then
			showReplays = showReplays == false
		end
	
		if RAYGUI.GuiButton(20, 95, 200, 25, "Scripts") then
			showScripts = showScripts == false
		end
	
		if RAYGUI.GuiButton(20, 120, 200, 25, "Settings") then
			showSettings = showSettings == false
		end
	else
		RAYGUI.GuiLabel(10, 10, screenWidth-10, 10, `{mod}: o:{o_count} p:{p_count} {p_details}`)
		RAYGUI.GuiLabel(10, 30, screenWidth-10, 10, `{frame}`)
		RAYGUI.GuiLabel(10, 50, screenWidth-10, 10, `{reaction_count}`)
		RAYGUI.GuiLabel(10, 70, screenWidth-10, 10, console_message)
	end

	if showRules then
		RAYGUI.GuiGroupBox(screenWidth-220, 20, 200, 400, "Rules")
		RAYGUI.GuiLabel(screenWidth-200, 20, 200, 40, rules.mod)
		RAYGUI.GuiLabel(screenWidth-200, 40, 200, 40, gravity)
		RAYGUI.GuiLabel(screenWidth-200,  60, 200, 40, `{rules.numplayers}`)
		RAYGUI.GuiLabel(screenWidth-200,  80, 200, 40, `{rules.turnframes}`)
		RAYGUI.GuiLabel(screenWidth-200, 100, 200, 40, `{rules.max_contacts}`)
		RAYGUI.GuiLabel(screenWidth-200, 120, 200, 40, `{rules.reaction_time}`)
		RAYGUI.GuiLabel(screenWidth-200, 140, 200, 40, `{rules.engagedistance}`)
		RAYGUI.GuiLabel(screenWidth-200, 160, 200, 40, `{rules.engageheight}`)
		RAYGUI.GuiLabel(screenWidth-200, 180, 200, 40, `{rules.friction}`)
	end

	if showMods then 
		RAYGUI.GuiGroupBox(screenWidth-220, 20, 200, 400, "Mods")
		local mods = RAYLIB.LoadDirectoryFiles("mods");
		for i = 1, #mods do
			local name = mods[i]:gsub("mods/", "")
			if RAYGUI.GuiButton(screenWidth-220, 20 * (i + 1), 200, 20, name) then
				API.log(name)
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
	end
end

function API.Console.init(message)
	if message:match("/.+") then
		local cmd = command[message:gsub("/", "")]
		local void = cmd and cmd()
	end
	console_message = message
end
