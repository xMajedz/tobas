local require = API["require"]
local ms = require("modstuff")

local loadmodTBM = ms["loadmodTBM"]
loadmodTBM("shootboxing15")

local mods_paths = RAYLIB.LoadDirectoryFiles("mods")
local scripts_paths = RAYLIB.LoadDirectoryFiles("scripts")
local replays_paths = RAYLIB.LoadDirectoryFiles("replays")


RAYGUI.GuiGetStyle(0, 16)
RAYGUI.GuiSetStyle(0, 16, 30)
RAYGUI.GuiGetStyle(0, 16)

local NewGame_init = API.GetCallback("NewGame", "init")

local window = API.GetWindowSize()

local init = {}

local log = API["log"]
log "log"

if _G["Net"] then
	print("internets")
end

function init.Mod()
	local p_count = Game.GetPlayerCount()
	local p = "";
	for i = 1, p_count do
		p = p .. " P" .. (i - 1) .. ":"
		p = p .. " B: " .. Game.GetPlayerBodyCount(i - 1)
		p = p .. " J: " .. Game.GetPlayerJointCount(i - 1)
	end
	local title = string.format(
		"%s O:%d P:%d%s",
		Game.GetMod(),
		Game.GetObjectCount(),
		p_count,
		p
	)
	RAYGUI.GuiLabel(20, 20, window.width-20, 20, title)
end

function init.GameFrame()
	local title = Game.GetGameFrame()
	RAYGUI.GuiLabel(20, 60, 90, 20, tostring(title))
end

function init.ReactionCount()
	local title = Game.GetReactionTime() - Game.GetReactionCount()
	RAYGUI.GuiLabel(20, 100, 90, 20, tostring(title))
end

function API.Draw2D.init()
	init.Mod()
	init.GameFrame()
	init.ReactionCount()
	--if RAYGUI.GuiButton(10, 20, window.width-20, 20, `#191#{console_message}`) then end
end
