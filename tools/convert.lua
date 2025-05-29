dofile("tools/string.lua")

local File = dofile("tools/File.lua")
local Math = dofile("tools/Math.lua")
local default = dofile("tools/default.lua")
local euler2quat = function (u, v, w) return Math.MatrixToQuaternion(Math.EulerToMatrix(u, v, w)) end
function table.clone(t)
	local result = {}
	for key, value in pairs(t) do
		if type(value) == "table" then
			result[key] = table.clone(value)
		else
			result[key] = value
		end
	end
	return result
end

local mod = table.clone(default)

makestuff = {}

function makestuff.parse(file)
	local source = File.readLines(file)
	local env_obj_num = ""
	local env_obj_joint_num = ""
	local player_name = ""
	local body_name = ""
	local joint_name = ""
	local context = ""
	local data = ""
	for _, line in pairs(source) do
	    if line:match("gravity ") then
	        mod.gravity = line:gsub(".*gravity ", "")
	    elseif line:match("numplayers ") then
	        mod.numplayers = line:gsub(".*numplayers ", "");
	    elseif line:match("turnframes ") then
	        mod.turnframes = line:gsub(".*turnframes ", "");
	    elseif line:match("engageplayerpos ") then
		line = line:gsub(".*engageplayerpos ", "")
	        mod.engageplayerpos = line:explode(",")
		local k = 0
		for i = 1, #mod.engageplayerpos/3 do
			mod.player[i].engagepos = {}
			for j = 0, 2 do
				local index = i + j + k
				mod.player[i].engagepos[j + 1] = mod.engageplayerpos[index]
			end
			k = k + 2
		end
	    elseif line:match("engageplayerrot ") then
	        line = line:gsub(".*engageplayerrot ", "")
	        mod.engageplayerrot = line:explode(",")
		local k = 0
		for i = 1, #mod.engageplayerrot/3 do
			mod.player[i].engagerot = {}
			for j = 0, 2 do
				local index = i + j + k
				mod.player[i].engagerot[j + 1] = mod.engageplayerrot[index]
			end
			k = k + 2
		end
	    elseif line:match("player ") then
		player_name = line:gsub(".*player ", "") + 1
	    elseif line:match("env_obj ") then
	        context = "env_obj"
	        data = line:gsub(".*env_obj ", "")
	        mod[context][data] = {}
	    elseif line:match("env_obj_joint") then
	        context = "env_obj_joint"
	        data = line:gsub(".*env_obj_joint ", "")
	        mod[context][data] = {}
	    elseif line:match("body ") then
	        context = "body"
	        data = line:gsub(".*body ", "")
	    elseif line:match("joint ")  then
	        context = "joint"
	        data = line:gsub(".*joint ", "")
	    elseif line:match("shape ") then
		    line = line:gsub(".*shape ", "")
		    if context == "body" or context == "joint" then
			mod.player[player_name][context][data].shape = line
		    else
			mod[context][data].shape = line
		    end
	    elseif line:match("pos ") then
		    line = line:gsub(".*pos ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].pos = line:explode(" ")
		    else
	        	mod[context][data].pos = line:explode(" ")
		    end
	    elseif line:match("rot ") then
		    line = line:gsub(".*rot ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].rot = line
		    else
	        	mod[context][data].rot = line
		    end
	    elseif line:match("%s+sides ") then
	        if context == "body" or context == "env_obj"  then
	            line = line:gsub(".*sides ", "")
		    if context == "body" or context == "joint" then
	               if mod.player[player_name][context][data].shape == "sphere" then
	                   line = line:explode(" ")
	                   mod.player[player_name][context][data].radius = line[1]
	               elseif mod.player[player_name][context][data].shape == "cylinder" then
	                   line = line:explode(" ")
	                   mod.player[player_name][context][data].radius = line[1]
	                   mod.player[player_name][context][data].length = line[2]
	               else
	                   mod.player[player_name][context][data].sides = line
	               end
		    else
	               if mod[context][data].shape == "sphere" then
	                   line = line:explode(" ")
	                   mod[context][data].radius = line[1]
	               elseif mod[context][data].shape == "cylinder" then
	                   line = line:explode(" ")
	                   mod[context][data].radius = line[1]
	                   mod[context][data].length = line[2]
	               else
	                   mod[context][data].sides = line
	               end
	            end
	        end
	    elseif line:match("radius ") then
		    line = line:gsub(".*radius ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].radius = line
		    else
	        	mod[context][data].radius = line
		    end
	    elseif line:match("density ") then
		    line = line:gsub(".*density ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].density = line
		    else
	        	mod[context][data].density = line
		    end
	    elseif line:match("axis ") then
		    line = line:gsub(".*axis ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].axis = line
		    else
	        	mod[context][data].axis = line
		    end
	    elseif line:match("range ") then
		    line = line:gsub(".*range ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].range = line
		    else
	        	mod[context][data].range = line
		    end
	    elseif line:match("strength ") then
		    line = line:gsub(".*strength ", "")
		    if context == "body" or context == "joint" then
	        	mod.player[player_name][context][data].strength = line
		    else
	        	mod[context][data].strength = line
		    end
	    elseif line:match("velocity ") then
		    line = line:gsub(".*velocity ", "")
		    if context == "body" or context == "joint" then
			mod.player[player_name][context][data].velocity = line 
		    else
			mod[context][data].velocity = line 
		    end
	    elseif line:match("flag ") then
		    if context == "" then
		    else
			line = line:gsub(".*flag ", "")
		    	if context == "body" or context == "joint" then
	        		mod.player[player_name][context][data].flag = line
			else
	        		mod[context][data].flag = line
			end
		    end
	    end
	end
end

function makestuff.convert(inputfile, outputfile)
	makestuff.parse(inputfile)

	local player_name_table = {}
	player_name_table[1] = "ichiji"
	player_name_table[2] = "niji"
	player_name_table[3] = "sanji"
	player_name_table[4] = "yonji"

	File.ClearContent()
	
	File.line("-- converted from " .. inputfile .. " using convert.lua")
	File.line("turnframes {" .. mod.turnframes .. "}")
	File.line("gravity {" .. mod.gravity:gsub(" ",", ") .. "}")
	File.line("friction {1000.00}")
	File.line("numplayers {" .. mod.numplayers .. "}")
	
	for env_obj_num, env_obj in pairs(mod.env_obj) do
	    File.line("object \"" .. env_obj_num .. "\"")
	    if env_obj.shape == nil then
	        File.line("\tshape \"box\"")
	    elseif env_obj.shape == "cylinder" then
	        File.line("\tshape \"capsule\"")
	    else
	        File.line("\tshape \"" .. env_obj.shape .. "\"")
	    end
	    local pos = {env_obj.pos[1], env_obj.pos[2], env_obj.pos[3]}
	    File.line("\tposition {"
	    	.. pos[1] .. ", "
	    	.. pos[2] .. ", "
	    	.. pos[3] .. "}")
	    if  env_obj.rot then
	        local rot = env_obj.rot:explode(" ")
		local RAD2DEG = Math.RAD2DEG
	        rot[1] = tonumber(rot[1]) * RAD2DEG
	        rot[2] = tonumber(rot[2]) * RAD2DEG
	        rot[3] = tonumber(rot[3]) * RAD2DEG
	        local q = euler2quat(rot[1], rot[2], rot[3])
	        File.line("\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
	    end
	    if env_obj.shape == "sphere" then
	        File.line("\tradius {" .. env_obj.radius .. "}")
	    elseif env_obj.shape == "cylinder" then
	        File.line("\tradius {" .. env_obj.radius .. "}")
	        File.line("\tlength {" .. env_obj.length .. "}")
	    else
	        File.line("\tsides {" .. env_obj.sides:gsub(" ", ", ") .. "}")
	    end
	    if env_obj.density then
	        File.line("\tdensity {".. env_obj.density .."}")
	    else
	        File.line("\tdensity {0.25}")
	    end
	    if env_obj.flag == "24" then
	    	File.line("\tstatic()")
	    end
	end

	for player_name, player in pairs(mod.player) do
		File.line("player \"" .. player_name_table[player_name] .."\"")
		if player.engagepos then
			print(player_name, player.engagepos[1], player.engagepos[2], player.engagepos[3])
			File.line("\tengageposition {"
			    .. player.engagepos[1] .. ", "
			    .. player.engagepos[2] .. ", "
	    		    .. player.engagepos[3] .. "}")
	    	end
		if player.engagerot then
			File.line("\tengagerotation {"
			    .. player.engagerot[1] .. ", "
			    .. player.engagerot[2] .. ", "
	    		    .. player.engagerot[3] .. "}")
	    	end
		for body_name, body in pairs(player.body) do
			File.line("\tbody \"" .. body_name .. "\"")
			if body.shape == "cylinder" then
			    File.line("\t\tshape \"capsule\"")
			else
			    File.line("\t\tshape \"" .. body.shape .. "\"")
			end
			local connections = {}
			connections["breast"] = "chest"
			connections["chest"] = "lumbar"
			connections["stomach"] = "abs"
	
			connections["l_pecs"] = "l_pecs"
			connections["r_pecs"] = "r_pecs"
	
			connections["l_biceps"] = "l_shoulder"
			connections["r_biceps"] = "r_shoulder"
	
			connections["l_triceps"] = "l_elbow"
			connections["r_triceps"] = "r_elbow"
	
			connections["groin"] = "abs"
	
			connections["l_thigh"] = "l_hip"
			connections["r_thigh"] = "r_hip"
	
			connections["l_leg"] = "l_knee"
			connections["r_leg"] = "r_knee"
	
			local pos = {body.pos[1], body.pos[2], body.pos[3]}
			if body.shape == "cylinder" and
				not body_name:match("groin") and
				not body_name:match("thigh") and
				not body_name:match("leg") then
				local RAD2DEG = Math.RAD2DEG
				local angel = {0.00, 0.00, 0.00}
				if body_name:match("l_") then
					angel[2] = 90.00
				end
				if body_name:match("r_") then
					angel[2] = -90.00
				end
				if body_name == "breast" then
					angel[1] = -90.00
				end
				if body_name == "chest" then 
					angel[2] = -90.00
				end
				if body_name == "stomach" then
					angel[2] = -90.00
				end
				local q = Math.MatrixToQuaternion(Math.EulerToMatrix(
					RAD2DEG * angel[1],
					RAD2DEG * angel[2],
					RAD2DEG * angel[3]))
				local q_inverse = {-q[1], q[2], q[3], q[4]}
					
				local joint_pos = player.joint[connections[body_name]].pos
				local jpos = {joint_pos[1], joint_pos[2], joint_pos[3]}
				pos[1] = pos[1] - jpos[1]
				pos[2] = pos[2] - jpos[2]
				pos[3] = pos[3] - jpos[3]
				
				local p = {0.00, pos[1], pos[2], pos[3]}
				p = Math.QuaternionMultiply(Math.QuaternionMultiply(q, p), q_inverse)
				p[2] = p[2] + jpos[1]
				p[3] = p[3] + jpos[2]
				p[4] = p[4] + jpos[3]
				File.line("\t\tposition {" .. p[2] .. ", " .. p[3] .. ", " .. p[4] .. "}")
			else
				File.line("\t\tposition {"
				.. pos[1] .. ", "
				.. pos[2] .. ", "
				.. pos[3] .. "}")
			end
			if  body.rot then
				local rot = body.rot:explode(" ")
				local RAD2DEG = Math.RAD2DEG
				rot[1] = tonumber(rot[1]) * RAD2DEG
				rot[2] = tonumber(rot[2]) * RAD2DEG
				rot[3] = tonumber(rot[3]) * RAD2DEG
				local q = euler2quat(rot[1], rot[2], rot[3])
				File.line("\t\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
			end
			if body_name == "head" or body.shape == "sphere" then
				File.line("\t\tradius {" .. body.radius .. "}")
			elseif body.shape == "cylinder" then
				File.line("\t\tradius {" .. body.radius .. "}")
				File.line("\t\tlength {" .. body.length .. "}")
			else
				File.line("\t\tsides {" .. body.sides:gsub(" ",", ") .. "}")
			end
			File.line("\t\tdensity {" .. body.density .. "}")
	end
	
		for joint_name, joint in pairs(player.joint) do
			File.line("\tjoint \"" .. joint_name .. "\"")
			File.line("\t\tshape \"sphere\"")
			File.line("\t\tradius {" .. joint.radius .. "}")
			local pos = {joint.pos[1], joint.pos[2], joint.pos[3]}
			File.line("\t\tposition {"
			.. pos[1] .. ", "
			.. pos[2] .. ", "
			.. pos[3] .. "}")
			File.line("\t\tdensity {0.0025}")
			File.line("\t\tstrength {" .. tonumber(joint.strength) * 10*10*10 .. "}")
			File.line("\t\tvelocity {" .. joint.velocity .. "}")
			if joint.axis[1] == nil then
				joint.axis = joint.axis:explode(" ")
			end
			joint.axis[1] = tonumber(joint.axis[1]) * -1
			joint.axis[2] = tonumber(joint.axis[2]) * -1
			joint.axis[3] = tonumber(joint.axis[3]) * -1
			File.line("\t\taxis {"
				.. joint.axis[1] .. ", "
				.. joint.axis[2] .. ", "
				.. joint.axis[3] .. "}")
			if joint.range[1] == nil then
				joint.range = joint.range:explode(" ")
			end
			if joint_name:match("r_pecs") or joint_name:match("r_elbow") or joint_name:match("r_wrist") then
				local r = joint.range[1]
				joint.range[1] = tonumber(joint.range[2]) * -1
				joint.range[2] = tonumber(r) * -1
			end
			File.line("\t\trange {"
			.. joint.range[1] .. ", "
			.. joint.range[2] .. "}")
			File.line("\t\tconnections {" .. joint.connections .. "}")
			File.line("\t\tconnectionType \"hinge\"")
		end
	end
	File.write(outputfile or "out.lua")
end
