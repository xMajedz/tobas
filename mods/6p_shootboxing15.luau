local require = API["require"]
local ms = require("modstuff")
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "6p_shootboxing15"
TBM["numplayers"] = 6
TBM["engageheight"] = 1.60
TBM["engagedistance"] = 5.00

TBM["env_obj"] = {}

TBM["player"][1]["engagepos"] = nil
TBM["player"][1]["body"]["head"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["l_hand"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["r_hand"]["flag"] = { interactive = true }

local b_pos, j_pos, q

j_pos = TBM["player"][1]["joint"]["l_shoulder"]["pos"]
b_pos = TBM["player"][1]["body"]["l_biceps"]["pos"]

b_pos[1] = b_pos[1] - j_pos[1]
b_pos[2] = b_pos[2] - j_pos[2]
b_pos[3] = b_pos[3] - j_pos[3]

q = RAYMATH.QuaternionFromMatrix(RAYMATH.MatrixRotateY(-90*RAYMATH["DEG2RAD"]))
b_pos = RAYMATH.Vector3RotateByQuaternion(b_pos, q)

b_pos[1] = b_pos[1] + j_pos[1]
b_pos[2] = b_pos[2] + j_pos[2]
b_pos[3] = b_pos[3] + j_pos[3]

TBM["player"][1]["body"]["l_biceps"]["pos"] = b_pos

j_pos = TBM["player"][1]["joint"]["r_shoulder"]["pos"]
b_pos = TBM["player"][1]["body"]["r_biceps"]["pos"]

b_pos[1] = b_pos[1] - j_pos[1]
b_pos[2] = b_pos[2] - j_pos[2]
b_pos[3] = b_pos[3] - j_pos[3]

q = RAYMATH.QuaternionFromMatrix(RAYMATH.MatrixRotateY(90*RAYMATH["DEG2RAD"]))
b_pos = RAYMATH.Vector3RotateByQuaternion(b_pos, q)

b_pos[1] = b_pos[1] + j_pos[1]
b_pos[2] = b_pos[2] + j_pos[2]
b_pos[3] = b_pos[3] + j_pos[3]

TBM["player"][1]["body"]["r_biceps"]["pos"] = b_pos

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
	ms.player_set_origin(p)
	ms.set_player_engagedistance(TBM, p, p_idx)
	ms.set_player_engageheight(TBM, p)
	ms.scale_player_joint_strength(p, 1000)
end

ms.loadTBM(TBM)
