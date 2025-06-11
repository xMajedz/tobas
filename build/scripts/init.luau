dofile("scripts/drawstuff/drawstuff.luau")
dofile("scripts/modstuff/modstuff.luau")
dofile("scripts/Math/Math.luau")
ms.loadmod("shootboxing15")

local window = GetWindowSize()

local console_box = ds.new({
	posX = 20,
	posY = window.height - 40,
	width = window.width - 40,
	height = 20,
	color = DS_COLOR_GREEN,
	borderColor = DS_COLOR_BLACK,
	interactive = true,
})

function console_box:click_callback()
	console_box:new_child({
		posX = 20,
		posY = window.height - 20 * (3 + #console_box.children),
		width = window.width - 40,
		height = 20,
		color = DS_COLOR_BLUE,
		text = 1 + #console_box.children,
		fontSize = 14,
		borderColor = DS_COLOR_BLACK,
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
	game_frame.text = get_game_frame()
end

function reaction_count:update_callback()
	reaction_count.text = string.gsub(get_reaction_count(), "%..*", "")
end

function FileDropped.init(file)
	print(file)
end
