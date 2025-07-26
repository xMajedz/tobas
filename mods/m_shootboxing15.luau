local require = API["require"]
local Math = require("Math")
local ms = require("modstuff")
local TBM = ms.parseTBM("shootboxing15")
TBM["mod"] = "m_shootboxing15"
TBM["numplayers"] = 2
TBM["engageheight"] = 1.60
TBM["engagedistance"] = 1.00

TBM["env_obj"] = {}

TBM["player"][1]["engagepos"] = nil
TBM["player"][1]["body"]["head"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["l_hand"]["flag"] = { interactive = true }
TBM["player"][1]["body"]["r_hand"]["flag"] = { interactive = true }

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
	--ms.set_player_engagedistance(TBM, p, p_idx)
	--ms.set_player_engageheight(TBM, p)
	--ms.scale_player_joint_strength(p, 1000)
end

ms.loadTBM(TBM)

function API.NewGame.temp()
	print(TBM["mod"])
end
