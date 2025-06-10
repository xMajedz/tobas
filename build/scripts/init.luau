dofile("scripts/drawstuff/drawstuff.luau")
dofile("scripts/modstuff/modstuff.luau")
dofile("scripts/Math/Math.luau")

--ms.loadmodTBM("lawz_model_evo_02")
--local TBM = ms.parseTBM("lawz_model_evo_02")
--local TBM = ms.parseTBM("tma_ergo_v4")
local TBM = ms.parseTBM("shootboxing15")
TBM["engageheight"] = 3.00 
TBM["engagedistance"] = 2.00
--[[
for player_name, p in TBM["player"] do
	local pos = {0.00, 0.00, 0.00}
	local lowest = 10.00
	for body_name, b in p["body"] do
		if type(b["pos"]) == "string" then
			b["pos"] = b["pos"]:split(" ")
		end
		b["pos"][3] = b["pos"][3] + 0
		if lowest > b["pos"][3] then
			lowest = b["pos"][3]
		end
		pos[1] = pos[1] + b["pos"][1]
		pos[2] = pos[2] + b["pos"][2]
	end
	for joint_name, j in p["joint"] do
		if type(j["pos"]) == "string" then
			j["pos"] = j["pos"]:split(" ")
		end
		j["pos"][3] = j["pos"][3] + 0
		if lowest > j["pos"][3] then
			lowest = j["pos"][3]
		end
		pos[1] = pos[1] + j["pos"][1]
		pos[2] = pos[2] + j["pos"][2]
	end
	pos[1] = pos[1]/(#p["body"] + #p["joint"])
	pos[2] = pos[2]/(#p["body"] + #p["joint"])
	for body_name, b in p["body"] do
		b["pos"][1] = b["pos"][1] - pos[1]
		b["pos"][2] = b["pos"][2] - pos[2]
	end
	for joint_name, j in p["joint"] do
		j["pos"][1] = j["pos"][1] - pos[1]
		j["pos"][2] = j["pos"][2] - pos[2]
	end
end
]]
for player_name, p in TBM["player"] do
	for body_name, b in p["body"] do
		if type(b["pos"]) == "string" then
			b["pos"] = b["pos"]:split(" ")
		end
		if body_name == "l_biceps" then
			if type(p["joint"]["l_shoulder"]) == "string" then
				p["joint"]["l_shoulder"] = p["joint"]["l_shoulder"]:split()
			end
			local pos = {}
			pos[1] = 0.00
			pos[2] = b["pos"][1] - p["joint"]["l_shoulder"]["pos"][1]
			pos[3] = b["pos"][2] - p["joint"]["l_shoulder"]["pos"][2]
			pos[4] = b["pos"][3] - p["joint"]["l_shoulder"]["pos"][3]
			local m = Math.EulerToMatrix(0.00, -45 * Math.DEG2RAD, 0.00)
			local q = Math.MatrixToQuaternion(m)
			local q_inverse = {q[1], -q[2], -q[3], -q[4]}
			pos = Math.QuaternionMultiply(Math.QuaternionMultiply(q, pos), q_inverse)
			b["pos"][1] = pos[2] + p["joint"]["l_shoulder"]["pos"][1]
			b["pos"][2] = pos[3] + p["joint"]["l_shoulder"]["pos"][2]
			b["pos"][3] = pos[4] + p["joint"]["l_shoulder"]["pos"][3]
		end
		if body_name == "r_biceps" then
			if type(p["joint"]["r_shoulder"]) == "string" then
				p["joint"]["r_shoulder"] = p["joint"]["r_shoulder"]:split()
			end
			local pos = {}
			pos[1] = 0.00
			pos[2] = b["pos"][1] - p["joint"]["r_shoulder"]["pos"][1]
			pos[3] = b["pos"][2] - p["joint"]["r_shoulder"]["pos"][2]
			pos[4] = b["pos"][3] - p["joint"]["r_shoulder"]["pos"][3]
			local m = Math.EulerToMatrix(0.00, 45 * Math.DEG2RAD, 0.00)
			local q = Math.MatrixToQuaternion(m)
			local q_inverse = {q[1], -q[2], -q[3], -q[4]}
			pos = Math.QuaternionMultiply(Math.QuaternionMultiply(q, pos), q_inverse)
			b["pos"][1] = pos[2] + p["joint"]["r_shoulder"]["pos"][1]
			b["pos"][2] = pos[3] + p["joint"]["r_shoulder"]["pos"][2]
			b["pos"][3] = pos[4] + p["joint"]["r_shoulder"]["pos"][3]
		end
	end
	for joint_name, j in p["joint"] do
		j["strength"] = 10 * 10 * j["strength"]
	end
end
ms.loadTBM(TBM)
--ms.loadmodTBM("tma_ergo_v4")
--ms.loadmodTBM("shootboxing15")

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
	local paths = LoadDirectoryFiles("scripts")
	print(paths[1])
	console_box:new_child({
		posX = 20,
		posY = window.height - 20 * (3 + #console_box.children),
		width = window.width - 40,
		height = 20,
		color = DS_COLOR_BLUE,
		text = paths[1 + #console_box.children],
		fontSize = 11,
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
