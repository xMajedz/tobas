local require = API["require"]
local ms = require("modstuff")
local loadModifiedTBM = ms["loadModifiedTBM"]
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "ma_shootboxing15"
TBM["numplayers"] = 2
--TBM["engageheight"] = 1.60
TBM["engageheight"] = 3.70
TBM["engagedistance"] = 1.00

TBM["player"][1]["engagepos"] = nil
TBM["player"][1]["body"]["head"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["l_hand"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["r_hand"]["flag"] = { interactive = true }

ms.rotateY_body_around_joint(TBM["player"][1], "l_biceps", "l_shoulder", -90)
ms.rotateY_body_around_joint(TBM["player"][1], "r_biceps", "r_shoulder", 90)

ms.rotateY_joint_axis(TBM["player"][1], "neck", 180)
ms.rotateY_joint_axis(TBM["player"][1], "abs", 180)

ms.rotateY_joint_axis(TBM["player"][1], "l_hip", 180)
ms.rotateY_joint_axis(TBM["player"][1], "l_knee", 180)
ms.rotateY_joint_axis(TBM["player"][1], "l_ankle", 180)

ms.rotateY_joint_axis(TBM["player"][1], "r_hip", 180)
ms.rotateY_joint_axis(TBM["player"][1], "r_knee", 180)
ms.rotateY_joint_axis(TBM["player"][1], "r_ankle", 180)

ms.rotateY_joint_axis(TBM["player"][1], "r_pecs", 180)

ms.rotateY_joint_axis(TBM["player"][1], "r_elbow", 180)
ms.rotateZ_joint_axis(TBM["player"][1], "r_elbow", 180)

ms.rotateZ_joint_axis(TBM["player"][1], "r_glute", 180)

for joint_name in TBM["player"][1]["joint"] do
	TBM["player"][1]["joint"][joint_name]["strength"] = TBM["player"][1]["joint"][joint_name]["strength"] * 4
	TBM["player"][1]["joint"][joint_name]["strength_alt"] = TBM["player"][1]["joint"][joint_name]["strength"] * 4
end

TBM["player"][1]["joint"]["l_shoulder"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["l_shoulder"]["strength_alt"] = TBM["player"][1]["joint"]["l_shoulder"]["strength"]
TBM["player"][1]["joint"]["l_shoulder"]["velocity_alt"] = TBM["player"][1]["joint"]["l_shoulder"]["velocity"]
TBM["player"][1]["joint"]["l_shoulder"]["range_alt"] = {1.00, 0.00}
TBM["player"][1]["joint"]["l_shoulder"]["axis_alt"] = {1.00, 0.00, 0.00}

TBM["player"][1]["joint"]["r_shoulder"]["type"] = API["JOINT_UNIVERSAL"]
TBM["player"][1]["joint"]["r_shoulder"]["strength_alt"] = TBM["player"][1]["joint"]["r_shoulder"]["strength"]
TBM["player"][1]["joint"]["r_shoulder"]["velocity_alt"] = TBM["player"][1]["joint"]["r_shoulder"]["velocity"]
TBM["player"][1]["joint"]["r_shoulder"]["range_alt"] = {1.00, 0.50}
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
	TBM["player"][i] = ms.clone_player(TBM["player"][1])
end

for p_idx, p in TBM["player"] do
	ms.player_set_origin(p)
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
end

ms.loadTBM(TBM)
