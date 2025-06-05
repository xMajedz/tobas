dofile("scripts/modstuff/default.luau")
dofile("scripts/filestuff/filestuff.luau")
dofile("scripts/Math/Math.luau")

ms = {}
ms.player_name_table = {}
ms.player_name_table[1] = "ichiji"
ms.player_name_table[2] = "niji"
ms.player_name_table[3] = "sanji"
ms.player_name_table[4] = "yonji"

ms.connections = {}
ms.connections["breast"] = "chest"
ms.connections["chest"] = "lumbar"
ms.connections["stomach"] = "abs"
ms.connections["l_pecs"] = "l_pecs"
ms.connections["r_pecs"] = "r_pecs"
ms.connections["l_biceps"] = "l_shoulder"
ms.connections["r_biceps"] = "r_shoulder"
ms.connections["l_triceps"] = "l_elbow"
ms.connections["r_triceps"] = "r_elbow"
ms.connections["groin"] = "abs"
ms.connections["l_thigh"] = "l_hip"
ms.connections["r_thigh"] = "r_hip"
ms.connections["l_leg"] = "l_knee"
ms.connections["r_leg"] = "r_knee"

function ms.loadmod(modname)
	dofile("mods/" .. modname .. ".luau")
end

function ms.loadmodTBM(modname)
	ms.loadTBM(ms.parseTBM(modname))
end

function ms.loadTBM(TBM)
	local euler2quat = function (u, v, w)
		return Math.MatrixToQuaternion(Math.EulerToMatrix(u, v, w))
	end

	gravity {TBM["gravity"][1] + 0, TBM["gravity"][2] + 0, TBM["gravity"][3] + 0}
	turnframes {TBM["turnframes"] + 0}
	if TBM["engageheight"] then
		engageheight {TBM["engageheight"] + 0}
	end
	if TBM["engagedistance"] then
		engagedistance {TBM["engagedistance"] + 0}
	end
	numplayers {TBM["numplayers"] + 0}
	friction {TBM["friction"] or 1000.00}
	
	for env_obj_num, env_obj in TBM["env_obj"] do
		object(env_obj_num)
		if env_obj["shape"] then
			shape(env_obj["shape"])
		elseif env_obj.shape == "cylinder" then
			shape "capsule"
		else
			shape "box"
		end
		position {env_obj["pos"][1] + 0, env_obj["pos"][2] + 0, env_obj["pos"][3] + 0}
		if  env_obj["rot"] then
			local RAD2DEG = Math.RAD2DEG
			local q = euler2quat(env_obj["rot"][1] * RAD2DEG, env_obj["rot"][2] * RAD2DEG, env_obj["rot"][3] * RAD2DEG)
			orientation {q[1], q[2], q[3], q[4]}
		end
		if env_obj["shape"] == "sphere" then
			radius {env_obj["radius"] + 0}
		elseif env_obj["shape"] == "cylinder" then
			radius {env_obj["radius"] + 0}
			length {env_obj["length"] + 0}
		else
			sides {env_obj["sides"][1] + 0, env_obj["sides"][2] + 0, env_obj["sides"][3] + 0}
		end
		if env_obj["density"] then
			density {env_obj["density"] + 0}
		else
			density {0.25}
		end
		if env_obj["flag"] == "24" then
			static()
		end
	end
	for player_name, p in TBM["player"] do
		player(ms.player_name_table[player_name])
		if p["engagepos"] then
			engageposition {p["engagepos"][1] + 0, p["engagepos"][2] + 0, p["engagepos"][3] + 0}
    end
		if p["engagerot"] then
			engagerotation {p["engagerot"][1] + 0, p["engagerot"][2] + 0, p["engagerot"][3] + 0}
    end
		for body_name, b in p["body"] do
			body(body_name)
			if b["shape"] == "cylinder" then
				shape "capsule"
			else
				shape(b["shape"])
			end
       if type(b["pos"]) == "string" then
							 b["pos"] = b["pos"]:split(" ")
			 end
			position {b["pos"][1] + 0, b["pos"][2] + 0, b["pos"][3] + 0}
			if b["rot"] then
				local RAD2DEG = Math.RAD2DEG	
				local q = euler2quat(b["rot"][1] * RAD2DEG, b["rot"][2] * RAD2DEG, b["rot"][3] * RAD2DEG)
				orientation {q[1], q[2], q[3], q[4]}
			end
			if body_name == "head" or b["shape"] == "sphere" then
				radius {b["radius"] + 0}
			elseif b["shape"] == "cylinder" then
				radius {b["radius"] + 0}
				length {b["length"] + 0}
			else
       if type(b["sides"]) == "string" then
							 b["sides"] = b["sides"]:split(" ")
			 end
				sides {b["sides"][1] + 0, b["sides"][2] + 0, b["sides"][3] + 0}
			end
			density {b["density"] + 0}
	  end
		for joint_name, j in p["joint"] do
			joint(joint_name)
			shape "sphere"
			radius {j["radius"]}
       if type(j["pos"]) == "string" then
							 j["pos"] = j["pos"]:split(" ")
			 end
			position {j["pos"][1], j["pos"][2], j["pos"][3]}
			density {0.0025}
			strength {j["strength"]}
			velocity {j["velocity"]}
       if type(j["axis"]) == "string" then
							 j["axis"] = j["axis"]:split(" ")
			 end
			axis {j["axis"][1], j["axis"][2], j["axis"][3]}
       if type(j["range"]) == "string" then
							 j["range"] = j["range"]:split(" ")
			 end
			range {j["range"][1], j["range"][2]}
       if type(j["connections"]) == "string" then
			j["connections"] = j["connections"]:split(" ")
			end
			connections {j["connections"][1], j["connections"][2]}
			connectionType "hinge"
		end
	end
end

function ms.parseTBM(filename)
	fs.readfile("mods/" .. filename:gsub(".tbm", "") .. ".tbm")
	local TBM = table.clone(default)
	local env_obj_num = ""
	local env_obj_joint_num = ""
	local player_name = ""
	local body_name = ""
	local joint_name = ""
	local context = ""
	local data = ""
	for _, line in fs.lines do
		if line:match("version ") then
			TBM["version"] = line:gsub(".*version ", "")
		elseif line:match("turnframes ") then
			TBM["turnframes"] = line:gsub(".*turnframes ", "");
		elseif line:match("numplayers ") then
			TBM["numplayers"] = line:gsub(".*numplayers ", "");
		elseif line:match("gravity ") then
			line = line:gsub(".*gravity ", "")
			TBM["gravity"] = line:split(" ")
		elseif line:match("engageplayerpos ") then
			line = line:gsub(".*engageplayerpos ", "")
			TBM["engageplayerpos"] = line:split(",")
			local k = 0
			for i = 1, #TBM["engageplayerpos"]/3 do
				TBM["player"][i]["engagepos"] = {}
				for j = 0, 2 do
					local index = i + j + k
					TBM["player"][i]["engagepos"][j + 1] = TBM["engageplayerpos"][index]
				end
				k = k + 2
			end
		elseif line:match("engageplayerrot ") then
			line = line:gsub(".*engageplayerrot ", "")
			TBM["engageplayerrot"] = line:split(",")
			local k = 0
			for i = 1, #TBM["engageplayerrot"]/3 do
				TBM["player"][i]["engagerot"] = {}
				for j = 0, 2 do
					local index = i + j + k
					TBM["player"][i]["engagerot"][j + 1] = TBM["engageplayerrot"][index]
				end
				k = k + 2
			end
		elseif line:match("player ") then
			player_name = line:gsub(".*player ", "") + 1
		elseif line:match("env_obj ") then
			context = "env_obj"
			data = line:gsub(".*env_obj ", "")
			TBM[context][data] = {}
		elseif line:match("env_obj_joint") then
			context = "env_obj_joint"
			data = line:gsub(".*env_obj_joint ", "")
			TBM[context][data] = {}
		elseif line:match("body ") then
			context = "body"
			data = line:gsub(".*body ", "")
		elseif line:match("joint ")  then
			context = "joint"
			data = line:gsub(".*joint ", "")
		elseif line:match("shape ") then
			line = line:gsub(".*shape ", "")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["shape"] = line
			else
				TBM[context][data]["shape"] = line
			end
		elseif line:match("pos ") then
			line = line:gsub(".*pos ", "")
			line = line:split(" ")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["pos"] = line
			else
				TBM[context][data]["pos"] = line
			end
		elseif line:match("rot ") then
			line = line:gsub(".*rot ", "")
			line = line:split(" ")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["rot"] = line
			else
				TBM[context][data]["rot"] = line
			end
		elseif line:match("sides ") then
			line = line:gsub(".*sides ", "")
			line = line:split(" ")
      if context == "body" or context == "joint" then
					if TBM["player"][player_name][context][data]["shape"] == "sphere" then
						TBM["player"][player_name][context][data]["radius"] = line[1]
					elseif TBM["player"][player_name][context][data]["shape"] == "cylinder" then
						TBM["player"][player_name][context][data]["radius"] = line[1]
						TBM["player"][player_name][context][data]["length"] = line[2]
					else
						TBM["player"][player_name][context][data]["sides"] = line
					end
			else
					if TBM[context][data]["shape"] == "sphere" then
						TBM[context][data]["radius"] = line[1]
					elseif TBM[context][data]["shape"] == "cylinder" then
						TBM[context][data]["radius"] = line[1]
						TBM[context][data]["length"] = line[2]
					else
						TBM[context][data]["sides"] = line
					end
      end
		elseif line:match("radius ") then
			line = line:gsub(".*radius ", "")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["radius"] = line
			else
				TBM[context][data]["radius"] = line
			end
		elseif line:match("density ") then
			line = line:gsub(".*density ", "")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["density"] = line
			else
				TBM[context][data]["density"] = line
			end
		elseif line:match("axis ") then
			line = line:gsub(".*axis ", "")
      line = line:split(" ")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["axis"] = line
			else
				TBM[context][data]["axis"] = line
			end
		elseif line:match("range ") then
			line = line:gsub(".*range ", "")
      line = line:split(" ")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["range"] = line
			else
				TBM[context][data]["range"] = line
			end
		elseif line:match("strength ") then
			line = line:gsub(".*strength ", "")
			if context == "body" or context == "joint" then
				TBM["player"][player_name][context][data]["strength"] = line
			else
				TBM[context][data]["strength"] = line
			end
		elseif line:match("velocity ") then
			line = line:gsub(".*velocity ", "")
			if context == "body" or context == "joint" then
			TBM["player"][player_name][context][data]["velocity"] = line 
			else
			TBM[context][data]["velocity"] = line 
			end
		elseif line:match("flag ") then
			line = line:gsub(".*flag ", "")
			if context == "" then
				TBM["flag"] = line
			elseif context == "body" or context == "joint" then
					TBM["player"][player_name][context][data]["flag"] = line
			else
					TBM[context][data]["flag"] = line
			end
		end
	end
	return TBM
end

return ms
