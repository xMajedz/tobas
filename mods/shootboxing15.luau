local require = API["require"]
local Math = require("Math")
local ms = require("modstuff")
local loadModifiedTBM = ms["loadModifiedTBM"]

loadModifiedTBM(function()
	local TBM = ms.parseTBM("shootboxing15")
	TBM["mod"] = "m_shootboxing15"
	TBM["reactiontime"] = 40
	TBM["numplayers"] = 4
	TBM["player"][3] = TBM["player"][1]
	TBM["player"][4] = TBM["player"][1]
	TBM["engageheight"] = 4.00
	TBM["engagedistance"] = 2.00
	for player_name, p in TBM["player"] do
		p["engagepos"] = nil
		for joint_name, j in p["joint"] do
			j["strength"] = 10 * j["strength"]
		end
	end
	return TBM
end)
