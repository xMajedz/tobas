local require = API["require"]
--local ds = require("drawstuff")
local ms = require("modstuff")

local loadmod = API["loadmod"]
--loadmod("shootboxing15")

local loadmodTBM = ms["loadmodTBM"]
--loadmodTBM("shootboxing15")

local mods_paths = RAYLIB.LoadDirectoryFiles("mods")
local scripts_paths = RAYLIB.LoadDirectoryFiles("scripts")
local replays_paths = RAYLIB.LoadDirectoryFiles("replays")

local init = {}

print(RAYGUI.GuiGetStyle(0, 16))
print(RAYGUI.GuiSetStyle(0, 16, 30))
print(RAYGUI.GuiGetStyle(0, 16))

local NewGame_init = API.GetCallback("NewGame", "init")

function init.Mod()
	local title = Game.GetMod()
	RAYGUI.GuiLabel(20, 20, 90, 20, title)
end

function init.GameFrame()
	local title = Game.GetGameFrame()
	RAYGUI.GuiLabel(20, 60, 90, 20, tostring(title))
end

function init.ReactionCount()
	local title = Game.GetReactionTime() - Game.GetReactionCount()
	RAYGUI.GuiLabel(20, 100, 90, 20, tostring(title))
end

local window = API.GetWindowSize()

function API.Draw2D.init()
	init.Mod()
	init.GameFrame()
	init.ReactionCount()
	--if RAYGUI.GuiButton(10, 20, window.width-20, 20, `#191#{console_message}`) then end
end
