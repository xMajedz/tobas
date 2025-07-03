local require = API["require"]
--local ds = require("drawstuff")
local ms = require("modstuff")

local loadmod = API["loadmod"]
--loadmod("shootboxing15")

local loadmodTBM = ms["loadmodTBM"]
loadmodTBM("shootboxing15")

--local scripts_paths = RAYLIB.LoadDirectoryFiles("scripts")
--local mods_paths = RAYLIB.LoadDirectoryFiles("mods")

local window = API.GetWindowSize()

--local console_box = ds.new({
--	posX = 20,
--	posY = window.height - 40,
--	width = window.width - 40,
--	height = 20,
--	color = ds.DS_COLOR_GREEN,
--	borderColor = ds.DS_COLOR_BLACK,
--	interactive = true,
--})
--
--function console_box:click_callback()
--	console_box:new_child({
--		posX = 20,
--		posY = window.height - 20 * (3 + #console_box.children),
--		width = window.width - 40,
--		height = 20,
--		color = ds.DS_COLOR_BLUE,
--		text = 1 + #console_box.children,
--		fontSize = 14,
--		borderColor = ds.DS_COLOR_BLACK,
--	})
--end
--
--local game_frame = ds.new({
--	posX = 20,
--	posY = 20,
--	fontSize = 30,
--})
--
--local reaction_count = ds.new({
--	posX = 20,
--	posY = 60,
--	fontSize = 30,
--})
--
--function game_frame:update_callback()
--	game_frame.text = GAME.get_game_frame()
--end
--
--function reaction_count:update_callback()
--	reaction_count.text = math.round(GAME.get_reaction_time() - GAME.get_reaction_count())
--end

function API.FileDropped.init(filepath)
	print(filepath:gsub(".+mods/", ""))
end

function API.NearCallback.init(contact_pos)
end

local showMessageBox = false

function API.Draw2D.init()
	--ds:drawstuff()
	if RAYGUI.GuiButton(24, 24, 120, 30, "#191#Show Message") then
		showMessageBox = true
	end

        if showMessageBox then
                local result = RAYGUI.GuiMessageBox(85, 70, 250, 100,
                    "#191#Message Box",
		    "Hi! This is a message!",
		    "Nice;Cool"
		)

                if result >= 0 then
			showMessageBox = false
		end
	end
end

function API.MouseButtonPressed.init()
	local MousePosition = RL.GetMousePosition()
	for i, o in ds.interactive_objects do
		local conditionX = MousePosition.x > o.posX and MousePosition.x < o.posX + o.width
		local conditionY = MousePosition.y > o.posY and MousePosition.y < o.posY + o.height
		if conditionX and conditionY then
			o:click_callback()
		end
	end
end
