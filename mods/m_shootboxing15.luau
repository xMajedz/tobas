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

for i = 2, TBM["numplayers"] do
	TBM["player"][i] = ms.clone_player(TBM["player"][1])
end

for p_idx, p in TBM["player"] do
	ms.player_set_origin(p)
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
end

ms.loadTBM(TBM)
