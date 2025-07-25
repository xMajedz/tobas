local require = API["require"]
local Math = require("Math")
local ms = require("modstuff")
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "6p_shootboxing15"
TBM["numplayers"] = 6
TBM["engageheight"] = 4.00
TBM["engagedistance"] = 2.00

for i = 2, TBM["numplayers"] do
	TBM["player"][i] = table.clone(TBM["player"][1])
	TBM["player"][i]["body"] = table.clone(TBM["player"][1]["body"])
	TBM["player"][i]["joint"] = table.clone(TBM["player"][1]["joint"])
	for body_name, b in TBM["player"][1]["body"] do
		TBM["player"][i]["body"][body_name] = table.clone(TBM["player"][1]["body"][body_name])
		TBM["player"][i]["body"][body_name]["pos"] = table.clone(TBM["player"][1]["body"][body_name]["pos"])
	end
	for joint_name, j in TBM["player"][1]["joint"] do
		TBM["player"][i]["joint"][joint_name] = table.clone(TBM["player"][1]["joint"][joint_name])
		TBM["player"][i]["joint"][joint_name]["pos"] = table.clone(TBM["player"][1]["joint"][joint_name]["pos"])
	end
end

for p_idx, p in TBM["player"] do
	local offset = ms.calculate_player_offset(p)
	ms.set_player_offset(p, {-offset[1], -offset[2], 0.00 })
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.scale_player_joint_strength(p)
end

ms.loadTBM(TBM)
