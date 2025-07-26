local require = API["require"]
local ms = require("modstuff")
local loadModifiedTBM = ms["loadModifiedTBM"]
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "ma_shootboxing15"
TBM["numplayers"] = 2
TBM["engageheight"] = 1.60
TBM["engagedistance"] = 1.00

TBM["env_obj"] = {}

TBM["player"][1]["engagepos"] = nil
TBM["player"][1]["body"]["head"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["l_hand"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["r_hand"]["flag"] = { interactive = true }

for joint_name in TBM["player"][1]["joint"] do
	TBM["player"][1]["joint"][joint_name]["strength"] = TBM["player"][1]["joint"][joint_name]["strength"] * 4
	TBM["player"][1]["joint"][joint_name]["strength_alt"] = TBM["player"][1]["joint"][joint_name]["strength"] * 4
end

TBM["player"][1]["joint"]["l_shoulder"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["l_shoulder"]["strength_alt"] = TBM["player"][1]["joint"]["l_shoulder"]["strength"]
TBM["player"][1]["joint"]["l_shoulder"]["velocity_alt"] = TBM["player"][1]["joint"]["l_shoulder"]["velocity"]
TBM["player"][1]["joint"]["l_shoulder"]["range_alt"] = TBM["player"][1]["joint"]["l_shoulder"]["range"]
TBM["player"][1]["joint"]["l_shoulder"]["axis_alt"] = {1.00, 0.00, 0.00}

TBM["player"][1]["joint"]["r_shoulder"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["r_shoulder"]["strength_alt"] = TBM["player"][1]["joint"]["r_shoulder"]["strength"]
TBM["player"][1]["joint"]["r_shoulder"]["velocity_alt"] = TBM["player"][1]["joint"]["r_shoulder"]["velocity"]
TBM["player"][1]["joint"]["r_shoulder"]["range_alt"] = TBM["player"][1]["joint"]["r_shoulder"]["range"]
TBM["player"][1]["joint"]["r_shoulder"]["axis_alt"] = {1.00, 0.00, 0.00}

TBM["player"][1]["joint"]["l_hip"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["l_hip"]["strength_alt"] = TBM["player"][1]["joint"]["l_hip"]["strength"]
TBM["player"][1]["joint"]["l_hip"]["velocity_alt"] = TBM["player"][1]["joint"]["l_hip"]["velocity"]
TBM["player"][1]["joint"]["l_hip"]["range_alt"] = TBM["player"][1]["joint"]["l_hip"]["range"]
TBM["player"][1]["joint"]["l_hip"]["axis_alt"] = {0.00, 0.00, 1.00} 

TBM["player"][1]["joint"]["r_hip"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["r_hip"]["strength_alt"] = TBM["player"][1]["joint"]["r_hip"]["strength"]
TBM["player"][1]["joint"]["r_hip"]["velocity_alt"] = TBM["player"][1]["joint"]["r_hip"]["velocity"]
TBM["player"][1]["joint"]["r_hip"]["range_alt"] = TBM["player"][1]["joint"]["r_hip"]["range"]
TBM["player"][1]["joint"]["r_hip"]["axis_alt"] = {0.00, 0.00, 1.00}

TBM["player"][1]["joint"]["l_ankle"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["l_ankle"]["strength_alt"] = TBM["player"][1]["joint"]["l_ankle"]["strength"] 
TBM["player"][1]["joint"]["l_ankle"]["velocity_alt"] = TBM["player"][1]["joint"]["l_ankle"]["velocity"]
TBM["player"][1]["joint"]["l_ankle"]["range_alt"] = {1.00, -1.00}
TBM["player"][1]["joint"]["l_ankle"]["axis_alt"] = {0.00, 1.00, 0.00}

TBM["player"][1]["joint"]["r_ankle"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["r_ankle"]["strength_alt"] = TBM["player"][1]["joint"]["r_ankle"]["strength"]
TBM["player"][1]["joint"]["r_ankle"]["velocity_alt"] = TBM["player"][1]["joint"]["r_ankle"]["velocity"]
TBM["player"][1]["joint"]["r_ankle"]["range_alt"] = {1.00, -1.00}
TBM["player"][1]["joint"]["r_ankle"]["axis_alt"] = {0.00, 1.00, 0.00}

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
	ms.set_player_offset(p, {-offset[1], -offset[2], -offset[3]})
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
	ms.scale_player_joint_strength(p, 60)
	ms.scale_player_joint_strength_alt(p, 60)
end

ms.loadTBM(TBM)
