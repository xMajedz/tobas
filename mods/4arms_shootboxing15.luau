local require = API["require"]
local Math = require("Math")
local ms = require("modstuff")
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "m_shootboxing15"
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


if type(TBM["player"][1]["joint"]["l_shoulder"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["l_shoulder"]["pos"] = TBM["player"][1]["joint"]["l_shoulder"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["l_shoulder_bottom"] = table.clone(TBM["player"][1]["joint"]["l_shoulder"])
TBM["player"][1]["joint"]["l_shoulder_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["l_shoulder"]["pos"])
TBM["player"][1]["joint"]["l_shoulder_bottom"]["pos"][3] = TBM["player"][1]["joint"]["l_shoulder"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["l_shoulder_bottom"]["connections"] = "l_pecs l_biceps_bottom"

if type(TBM["player"][1]["body"]["l_biceps"]["pos"]) == "string" then
	TBM["player"][1]["body"]["l_biceps"]["pos"] = TBM["player"][1]["body"]["l_biceps"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["l_biceps_bottom"] = table.clone(TBM["player"][1]["body"]["l_biceps"])
TBM["player"][1]["body"]["l_biceps_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["l_biceps"]["pos"])
TBM["player"][1]["body"]["l_biceps_bottom"]["pos"][3] = TBM["player"][1]["body"]["l_biceps"]["pos"][3] - 0.4

if type(TBM["player"][1]["joint"]["l_elbow"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["l_elbow"]["pos"] = TBM["player"][1]["joint"]["l_elbow"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["l_elbow_bottom"] = table.clone(TBM["player"][1]["joint"]["l_elbow"])
TBM["player"][1]["joint"]["l_elbow_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["l_elbow"]["pos"])
TBM["player"][1]["joint"]["l_elbow_bottom"]["pos"][3] = TBM["player"][1]["joint"]["l_elbow"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["l_elbow_bottom"]["connections"] = "l_biceps_bottom l_triceps_bottom"

if type(TBM["player"][1]["body"]["l_triceps"]["pos"]) == "string" then
	TBM["player"][1]["body"]["l_triceps"]["pos"] = TBM["player"][1]["body"]["l_triceps"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["l_triceps_bottom"] = table.clone(TBM["player"][1]["body"]["l_triceps"])
TBM["player"][1]["body"]["l_triceps_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["l_triceps"]["pos"])
TBM["player"][1]["body"]["l_triceps_bottom"]["pos"][3] = TBM["player"][1]["body"]["l_triceps"]["pos"][3] - 0.4

if type(TBM["player"][1]["joint"]["l_wrist"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["l_wrist"]["pos"] = TBM["player"][1]["joint"]["l_wrist"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["l_wrist_bottom"] = table.clone(TBM["player"][1]["joint"]["l_wrist"])
TBM["player"][1]["joint"]["l_wrist_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["l_wrist"]["pos"])
TBM["player"][1]["joint"]["l_wrist_bottom"]["pos"][3] = TBM["player"][1]["joint"]["l_wrist"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["l_wrist_bottom"]["connections"] = "l_triceps_bottom l_hand_bottom"

if type(TBM["player"][1]["body"]["l_hand"]["pos"]) == "string" then
	TBM["player"][1]["body"]["l_hand"]["pos"] = TBM["player"][1]["body"]["l_hand"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["l_hand_bottom"] = table.clone(TBM["player"][1]["body"]["l_hand"])
TBM["player"][1]["body"]["l_hand_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["l_hand"]["pos"])
TBM["player"][1]["body"]["l_hand_bottom"]["pos"][3] = TBM["player"][1]["body"]["l_hand"]["pos"][3] - 0.4



if type(TBM["player"][1]["joint"]["r_shoulder"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["r_shoulder"]["pos"] = TBM["player"][1]["joint"]["r_shoulder"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["r_shoulder_bottom"] = table.clone(TBM["player"][1]["joint"]["r_shoulder"])
TBM["player"][1]["joint"]["r_shoulder_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["r_shoulder"]["pos"])
TBM["player"][1]["joint"]["r_shoulder_bottom"]["pos"][3] = TBM["player"][1]["joint"]["r_shoulder"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["r_shoulder_bottom"]["connections"] = "r_pecs r_biceps_bottom"

if type(TBM["player"][1]["body"]["r_biceps"]["pos"]) == "string" then
	TBM["player"][1]["body"]["r_biceps"]["pos"] = TBM["player"][1]["body"]["r_biceps"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["r_biceps_bottom"] = table.clone(TBM["player"][1]["body"]["r_biceps"])
TBM["player"][1]["body"]["r_biceps_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["r_biceps"]["pos"])
TBM["player"][1]["body"]["r_biceps_bottom"]["pos"][3] = TBM["player"][1]["body"]["r_biceps"]["pos"][3] - 0.4

if type(TBM["player"][1]["joint"]["r_elbow"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["r_elbow"]["pos"] = TBM["player"][1]["joint"]["r_elbow"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["r_elbow_bottom"] = table.clone(TBM["player"][1]["joint"]["r_elbow"])
TBM["player"][1]["joint"]["r_elbow_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["r_elbow"]["pos"])
TBM["player"][1]["joint"]["r_elbow_bottom"]["pos"][3] = TBM["player"][1]["joint"]["r_elbow"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["r_elbow_bottom"]["connections"] = "r_biceps_bottom r_triceps_bottom"

if type(TBM["player"][1]["body"]["r_triceps"]["pos"]) == "string" then
	TBM["player"][1]["body"]["r_triceps"]["pos"] = TBM["player"][1]["body"]["r_triceps"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["r_triceps_bottom"] = table.clone(TBM["player"][1]["body"]["r_triceps"])
TBM["player"][1]["body"]["r_triceps_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["r_triceps"]["pos"])
TBM["player"][1]["body"]["r_triceps_bottom"]["pos"][3] = TBM["player"][1]["body"]["r_triceps"]["pos"][3] - 0.4

if type(TBM["player"][1]["joint"]["r_wrist"]["pos"]) == "string" then
	TBM["player"][1]["joint"]["r_wrist"]["pos"] = TBM["player"][1]["joint"]["r_wrist"]["pos"]:split(" ")
end
TBM["player"][1]["joint"]["r_wrist_bottom"] = table.clone(TBM["player"][1]["joint"]["r_wrist"])
TBM["player"][1]["joint"]["r_wrist_bottom"]["pos"] = table.clone(TBM["player"][1]["joint"]["r_wrist"]["pos"])
TBM["player"][1]["joint"]["r_wrist_bottom"]["pos"][3] = TBM["player"][1]["joint"]["r_wrist"]["pos"][3] - 0.4

TBM["player"][1]["joint"]["r_wrist_bottom"]["connections"] = "r_triceps_bottom r_hand_bottom"

if type(TBM["player"][1]["body"]["r_hand"]["pos"]) == "string" then
	TBM["player"][1]["body"]["r_hand"]["pos"] = TBM["player"][1]["body"]["r_hand"]["pos"]:split(" ")
end
TBM["player"][1]["body"]["r_hand_bottom"] = table.clone(TBM["player"][1]["body"]["r_hand"])
TBM["player"][1]["body"]["r_hand_bottom"]["pos"] = table.clone(TBM["player"][1]["body"]["r_hand"]["pos"])
TBM["player"][1]["body"]["r_hand_bottom"]["pos"][3] = TBM["player"][1]["body"]["r_hand"]["pos"][3] - 0.4

for i = 2, TBM["numplayers"] do
	TBM["player"][i] = ms.clone_player(TBM["player"][1])
end

for p_idx, p in TBM["player"] do
	ms.player_set_origin(p)
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
end

ms.loadTBM(TBM)
