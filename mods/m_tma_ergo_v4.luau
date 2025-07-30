local require = API["require"]
local Math = require("Math")
local ms = require("modstuff")
local TBM = ms.parseTBM("tma_ergo_v4")
TBM["mod"] = "m_tma_ergo_v4"
TBM["numplayers"] = 2
---TBM["engageheight"] = 1.60
TBM["engageheight"] = 3.40
TBM["engagedistance"] = 2.36

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

for i = 2, TBM["numplayers"] do
	TBM["player"][i] = ms.clone_player(TBM["player"][1])
end

for p_idx, p in TBM["player"] do
	ms.player_set_origin(p)
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
end

local offset = {0.00, 0.00, 0.00}
local count = 0
for o_idx, o in TBM["env_obj"] do
	if type(o["pos"]) == "string" then
		o["pos"] = o["pos"]:split(" ")
	end
	offset[1] = offset[1] + o["pos"][1]
	offset[2] = offset[2] + o["pos"][2]
	count = count + 1
end
for oj_idx, oj in TBM["env_obj_joint"] do
	if type(oj["pos"]) == "string" then
		oj["pos"] = oj["pos"]:split(" ")
	end
	offset[1] = offset[1] + oj["pos"][1]
	offset[2] = offset[2] + oj["pos"][2]
	count = count + 1
end
offset[1] = offset[1]/count
offset[2] = offset[2]/count
for o_idx, o in TBM["env_obj"] do
	o["pos"][1] = o["pos"][1] - offset[1]
	o["pos"][2] = o["pos"][2] - offset[2]
end
for oj_idx, oj in TBM["env_obj_joint"] do
	oj["pos"][1] = oj["pos"][1] - offset[1]
	oj["pos"][2] = oj["pos"][2] - offset[2]
end

ms.loadTBM(TBM)
