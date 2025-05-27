dofile("tools/string.lua")

local File = dofile("tools/File.lua")
local Math = dofile("tools/Math.lua")
local mod = dofile("tools/default.lua")
local source = File.readLines(arg[1])
local euler2quat = function (u, v, w) return Math.MatrixToQuaternion(Math.EulerToMatrix(u, v, w)) end
local env_obj_num = ""
local env_obj_joint_num = ""
local body_name = ""
local joint_name = ""
local context = ""
local data = ""

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
        line = line:gsub(".*player ", "") + 0
        if line > 0 then
            break
        end
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
        mod[context][data].shape = line:gsub(".*shape ", "")
    elseif line:match("pos ") then
        mod[context][data].pos = line:gsub(".*pos ", "")
    elseif line:match("rot ") then
        mod[context][data].rot = line:gsub(".*rot ", "")
    elseif line:match("%s+sides ") then
        if context == "body" or context == "env_obj"  then
            line = line:gsub(".*sides ", "")
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
    elseif line:match("radius ") then
        mod[context][data].radius = line:gsub(".*radius ", "")
    elseif line:match("density ") then
        mod[context][data].density = line:gsub(".*density ", "")
    elseif line:match("axis ") then
        mod[context][data].axis = line:gsub(".*axis ", "")
    elseif line:match("range ") then
        mod[context][data].sides = line:gsub(".*range ", "")
    elseif line:match("strength ") then
        mod[context][data].strength = line:gsub(".*strength ", "")
    elseif line:match("velocity ") then
        mod[context][data].velocity = line:gsub(".*velocity ", "")
    elseif line:match("flag ") then
	    if context == "" then
	    else
        	mod[context][data].flag = line:gsub(".*flag ", "")
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
for body_name, body in pairs(mod.body) do
    File.line("body \"" .. body_name .. "\"")
    if mod.body[body_name].shape == "cylinder" then
        File.line("\tshape \"capsule\"")
    else
        File.line("\tshape \"" .. mod.body[body_name].shape .. "\"")
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

    if mod.body[body_name].shape == "cylinder" and
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

	local pos = mod.body[body_name].pos:explode(" ")
	pos[1] = tonumber(pos[1]) + 1.00
	pos[2] = tonumber(pos[2]) - 0.10
	pos[3] = tonumber(pos[3]) + 0.00

	local jpos = mod.joint[connections[body_name]].pos:explode(" ")
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
    	File.line("\tposition {" .. p[2] .. ", " .. p[3] .. ", " .. p[4] .. "}")
    else
	mod.body[body_name].pos = mod.body[body_name].pos:explode(" ")
	mod.body[body_name].pos[1] = tonumber(mod.body[body_name].pos[1]) + 1.00
	mod.body[body_name].pos[2] = tonumber(mod.body[body_name].pos[2]) - 0.10
	mod.body[body_name].pos[3] = tonumber(mod.body[body_name].pos[3]) + 0.00
    	File.line("\tposition {"
	.. mod.body[body_name].pos[1] .. ", "
	.. mod.body[body_name].pos[2] .. ", "
	.. mod.body[body_name].pos[3] .. "}")
    end
    if  mod.body[body_name].rot then
        local rot = mod.body[body_name].rot:explode(" ")
	local RAD2DEG = Math.RAD2DEG
        rot[1] = tonumber(rot[1]) * RAD2DEG
        rot[2] = tonumber(rot[2]) * RAD2DEG
        rot[3] = tonumber(rot[3]) * RAD2DEG
        local q = euler2quat(rot[1], rot[2], rot[3])
        File.line("\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
    end
    if body_name == "head" or mod.body[body_name].shape == "sphere" then
        File.line("\tradius {" .. mod.body[body_name].radius .. "}")
    elseif mod.body[body_name].shape == "cylinder" then
        File.line("\tradius {" .. mod.body[body_name].radius .. "}")
        File.line("\tlength {" .. mod.body[body_name].length .. "}")
    else
        File.line("\tsides {" .. mod.body[body_name].sides:gsub(" ",", ") .. "}")
    end
    File.line("\tdensity {" .. mod.body[body_name].density .. "}")
end

for joint_name, joint in pairs(mod.joint) do
    File.line("joint \"" .. joint_name .. "\"")
    File.line("\tshape \"sphere\"")
    File.line("\tradius {" .. mod.joint[joint_name].radius .. "}")
    mod.joint[joint_name].pos = mod.joint[joint_name].pos:explode(" ")
    mod.joint[joint_name].pos[1] = tonumber(mod.joint[joint_name].pos[1]) + 1.00
    mod.joint[joint_name].pos[2] = tonumber(mod.joint[joint_name].pos[2]) - 0.10
    mod.joint[joint_name].pos[3] = tonumber(mod.joint[joint_name].pos[3]) + 0.00
    File.line("\tposition {"
    .. mod.joint[joint_name].pos[1] .. ", "
    .. mod.joint[joint_name].pos[2] .. ", "
    .. mod.joint[joint_name].pos[3] .. "}")
    File.line("\tdensity {0.0025}")
    File.line("\tstrength {" .. tonumber(mod.joint[joint_name].strength) * 10*10*10 .. "}")
    File.line("\tvelocity {" .. mod.joint[joint_name].velocity .. "}")
    mod.joint[joint_name].axis = mod.joint[joint_name].axis:explode(" ")
    mod.joint[joint_name].axis[1] = tonumber(mod.joint[joint_name].axis[1]) * -1
    mod.joint[joint_name].axis[2] = tonumber(mod.joint[joint_name].axis[2]) * -1
    mod.joint[joint_name].axis[3] = tonumber(mod.joint[joint_name].axis[3]) * -1
    File.line("\taxis {"
        .. mod.joint[joint_name].axis[1] .. ", "
        .. mod.joint[joint_name].axis[2] .. ", "
        .. mod.joint[joint_name].axis[3] .. "}")
    mod.joint[joint_name].range = mod.joint[joint_name].range:explode(" ")
    if joint_name:match("r_pecs") or joint_name:match("r_elbow") or joint_name:match("r_wrist") then
        local r = mod.joint[joint_name].range[1]
        mod.joint[joint_name].range[1] = tonumber(mod.joint[joint_name].range[2]) * -1
        mod.joint[joint_name].range[2] = tonumber(r) * -1
    end
    File.line("\trange {"
    .. mod.joint[joint_name].range[1] .. ", "
    .. mod.joint[joint_name].range[2] .. "}")
    File.line("\tconnections {" .. mod.joint[joint_name].connections .. "}")
    File.line("\tconnectionType \"hinge\"")
end

for env_obj_num, env_obj in pairs(mod.env_obj) do
    File.line("object \"" .. env_obj_num .. "\"")
    if mod.env_obj[env_obj_num].shape == nil then
        File.line("\tshape \"box\"")
    elseif mod.env_obj[env_obj_num].shape == "cylinder" then
        File.line("\tshape \"capsule\"")
    else
        File.line("\tshape \"" .. mod.env_obj[env_obj_num].shape .. "\"")
    end
    File.line("\tposition {" .. mod.env_obj[env_obj_num].pos:gsub(" ", ", ") .. "}")
    if  mod.env_obj[env_obj_num].rot then
        local rot = mod.env_obj[env_obj_num].rot:explode(" ")
	local RAD2DEG = Math.RAD2DEG
        rot[1] = tonumber(rot[1]) * RAD2DEG
        rot[2] = tonumber(rot[2]) * RAD2DEG
        rot[3] = tonumber(rot[3]) * RAD2DEG
        local q = euler2quat(rot[1], rot[2], rot[3])
        File.line("\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
    end
    if mod.env_obj[env_obj_num].shape == "sphere" then
        File.line("\tradius {" .. mod.env_obj[env_obj_num].radius .. "}")
    elseif mod.env_obj[env_obj_num].shape == "cylinder" then
        File.line("\tradius {" .. mod.env_obj[env_obj_num].radius .. "}")
        File.line("\tlength {" .. mod.env_obj[env_obj_num].length .. "}")
    else
        File.line("\tsides {" .. mod.env_obj[env_obj_num].sides:gsub(" ", ", ") .. "}")
    end
    if mod.env_obj[env_obj_num].density then
        File.line("\tdensity {".. mod.env_obj[env_obj_num].density .."}")
    else
        File.line("\tdensity {0.25}")
    end
    if mod.env_obj[env_obj_num].flag == "24" then
    	File.line("\tstatic()")
    end
end

File.write(arg[2] or "output.lua")
