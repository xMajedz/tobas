dofile("tools/string.lua")

local File = dofile("tools/File.lua")
local Math = dofile("tools/Math.lua")
local default_data = dofile("tools/default.lua")
local source = File.readLines(arg[1])
local euler2quat = function (u, v, w) return Math.MatrixToQuaternion(Math.EulerToMatrix(u, v, w)) end
local env_obj_num = ""
local env_obj_joint_num = ""
local player_name = ""
local body_name = ""
local joint_name = ""
local context = ""
local data = ""
local mod = {env_obj = {}, env_obj_joint = {}}
local player_name_table = {}
player_name_table[1] = "tori"
player_name_table[2] = "uke"
player_name_table[3] = "nage"
player_name_table[4] = "yonji"
mod.player = {}
mod.player[player_name_table[1]] = default_data
mod.player[player_name_table[2]] = default_data
mod.player[player_name_table[3]] = default_data
mod.player[player_name_table[4]] = default_data

for _, line in pairs(source) do
    if line:match("gravity ") then
        mod.gravity = line:gsub(".*gravity ", "")
    elseif line:match("turnframes ") then
        mod.turnframes = line:gsub(".*turnframes ", "");
    elseif line:match("engageplayerrot ") then
        mod.engageplayerrot = line:gsub(".*engageplayerrot ", "");
    elseif line:match("engageplayerpos ") then
        mod.engageplayerpos = line:gsub(".*engageplayerpos ", "");
    elseif line:match("player ") then
        line = line:gsub(".*player ", "")
	player_name = player_name_table[line + 1]
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
	    if context == "body" or context == "joint" then
		mod.player[player_name][context][data].shape = line:gsub(".*shape ", "")
	    else
		mod[context][data].shape = line:gsub(".*shape ", "")
	    end
    elseif line:match("pos ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].pos = line:gsub(".*pos ", "")
	    else
        	mod[context][data].pos = line:gsub(".*pos ", "")
	    end
    elseif line:match("rot ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].pos = line:gsub(".*rot ", "")
	    else
        	mod[context][data].rot = line:gsub(".*rot ", "")
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
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].radius = line:gsub(".*radius ", "")
	    else
        	mod[context][data].radius = line:gsub(".*radius ", "")
	    end
    elseif line:match("density ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].density = line:gsub(".*density ", "")
	    else
        	mod[context][data].density = line:gsub(".*density ", "")
	    end
    elseif line:match("axis ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].axis = line:gsub(".*axis ", "")
	    else
        	mod[context][data].axis = line:gsub(".*axis ", "")
	    end
    elseif line:match("range ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].range = line:gsub(".*range ", "")
	    else
        	mod[context][data].range = line:gsub(".*range ", "")
	    end
    elseif line:match("strength ") then
	    if context == "body" or context == "joint" then
        	mod.player[player_name][context][data].strength = line:gsub(".*strength ", "")
	    else
        	mod[context][data].strength = line:gsub(".*strength ", "")
	    end
    elseif line:match("velocity ") then
	    if context == "body" or context == "joint" then
		mod.player[player_name][context][data].velocity = line:gsub(".*velocity ", "")
	    else
		mod[context][data].velocity = line:gsub(".*velocity ", "")
	    end
    elseif line:match("flag ") then
	    if context == "" then
	    else
	    	if context == "body" or context == "joint" then
        		mod.player[player_name][context][data].flag = line:gsub(".*flag ", "")
		else
        		mod[context][data].flag = line:gsub(".*flag ", "")
		end
	    end
    end
end

File.ClearContent()

File.line("-- converted from " .. arg[1] .. " using convert.lua")
mod.engageplayerpos = mod.engageplayerpos:explode(",")
File.line("turnframes {" .. mod.turnframes .. "}")
File.line("gravity {" .. mod.gravity:gsub(" ",", ") .. "}")
File.line("friction {1000.00}")
File.line("engageplayerpos {"
    .. mod.engageplayerpos[1] .. ", "
    .. mod.engageplayerpos[2] .. ", "
    .. mod.engageplayerpos[3] .. "}")

for env_obj_num, env_obj in pairs(mod.env_obj) do
    File.line("object \"" .. env_obj_num .. "\"")
    if env_obj.shape == nil then
        File.line("\tshape \"box\"")
    elseif env_obj.shape == "cylinder" then
        File.line("\tshape \"capsule\"")
    else
        File.line("\tshape \"" .. env_obj.shape .. "\"")
    end
    File.line("\tposition {" .. env_obj.pos:gsub(" ", ", ") .. "}")
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
	File.line("player \"" .. player_name .."\"")
	for body_name, body in pairs(player.body) do
		File.line("\tbody \"" .. body_name .. "\"")
		if body.shape == "cylinder" then
		    File.line("\tshape \"capsule\"")
		else
		    File.line("\tshape \"" .. body.shape .. "\"")
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

		local pos = body.pos:explode(" ")
		pos[1] = tonumber(pos[1]) + 1.00
		pos[2] = tonumber(pos[2]) - 0.10
		pos[3] = tonumber(pos[3]) + 0.00

		if body.shape == "cylinder" and
			not body_name:match("groin") and
			not body_name:match("thigh") and
			not body_name:match("leg") then
			local RAD2DEG = Math.RAD2DEG
			local angel = {0.00, 0.00, 0.00}
			if body_name:match("l_") then
				angel[2] = -90.00
			end
			if body_name:match("r_") then
				angel[2] = 90.00
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
				
			local jpos = player.joint[connections[body_name]].pos
			if (jpos[1] == nil) then
				jpos = jpos:explode(" ")
			end
			jpos[1] = tonumber(jpos[1]) + 1.00
			jpos[2] = tonumber(jpos[2]) - 0.10
			jpos[3] = tonumber(jpos[3]) + 0.00

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
		if joint.pos[1] == nil then
			joint.pos = joint.pos:explode(" ")
		end
		joint.pos[1] = tonumber(joint.pos[1]) + 1.00
		joint.pos[2] = tonumber(joint.pos[2]) - 0.10
		joint.pos[3] = tonumber(joint.pos[3]) + 0.00
		File.line("\t\tposition {"
		.. joint.pos[1] .. ", "
		.. joint.pos[2] .. ", "
		.. joint.pos[3] .. "}")
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

File.write(arg[2] or "output.lua")
