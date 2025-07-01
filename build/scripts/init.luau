local require = API["require"]
local ds = require("drawstuff")
local ms = require("modstuff")

local loadmod = API["loadmod"]
loadmod("shootboxing15")

local loadmodTBM = ms["loadmodTBM"]
--loadmodTBM("shootboxing15")

local scripts_paths = RL.LoadDirectoryFiles("scripts")
local mods_paths = RL.LoadDirectoryFiles("mods")

local window = API.GetWindowSize()

local console_box = ds.new({
	posX = 20,
	posY = window.height - 40,
	width = window.width - 40,
	height = 20,
	color = ds.DS_COLOR_GREEN,
	borderColor = ds.DS_COLOR_BLACK,
	interactive = true,
})

function console_box:click_callback()
	console_box:new_child({
		posX = 20,
		posY = window.height - 20 * (3 + #console_box.children),
		width = window.width - 40,
		height = 20,
		color = ds.DS_COLOR_BLUE,
		text = 1 + #console_box.children,
		fontSize = 14,
		borderColor = ds.DS_COLOR_BLACK,
	})
end

local game_frame = ds.new({
	posX = 20,
	posY = 20,
	fontSize = 30,
})

local reaction_count = ds.new({
	posX = 20,
	posY = 60,
	fontSize = 30,
})

function game_frame:update_callback()
	game_frame.text = GAME.get_game_frame()
end

function reaction_count:update_callback()
	reaction_count.text = math.round(GAME.get_reaction_time() - GAME.get_reaction_count())
end

function EVENT.FileDropped.init(filepath) print(filepath:gsub(".+mods/", "")) end

function EVENT.NearCallback.init(contact_pos) end
