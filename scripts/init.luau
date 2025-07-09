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

function init.Net()
	local title = "Offline"
	if _G["Net"] then
		title = "Online"
	end
	RAYGUI.GuiLabel(20, 20, window.width-20, 20, `#172#{title}`)
end

function init.Mod()
	local mod = Game.GetMod()
	local o_count = Game.GetObjectCount()
	local p_count = Game.GetPlayerCount()
	local p_extra = "";
	for i = 1, p_count do
		p_extra = p_extra .. " P" .. (i - 1) .. ":"
		p_extra = p_extra .. " B: " .. Game.GetPlayerBodyCount(i - 1)
		p_extra = p_extra .. " J: " .. Game.GetPlayerJointCount(i - 1)
	end
	RAYGUI.GuiLabel(20, 60, window.width-20, 20, `{mod} O:{o_count} P:{p_count} -{p_extra}`)
end

function init.GameFrame()
	local title = Game.GetGameFrame()
	RAYGUI.GuiLabel(20, 100, 90, 20, tostring(title))
end

function init.ReactionCount()
	local title = Game.GetReactionTime() - Game.GetReactionCount()
	RAYGUI.GuiLabel(20, 140, 90, 20, tostring(title))
end

function API.Draw2D.init()
	init.Net()
	init.Mod()
	init.GameFrame()
	init.ReactionCount()
end
