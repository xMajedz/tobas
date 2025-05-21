dofile("tools/string.lua")

local File = dofile("tools/File.lua")
local Math = dofile("tools/Math.lua")
local data = dofile("tools/default.lua")
local source = File.readLines(arg[1])
local euler2quat = function (u, v, w) return Math.MatrixToQuaternion(Math.EulerToMatrix(u, v, w)) end
local env_obj_num = ""
local env_obj_joint_num = ""
local body_name = ""
local joint_name = ""
local context = ""

for _, line in pairs(source) do
    if line:match("gravity ") then
        data.gravity = line:gsub(".*gravity ", "")
    elseif line:match("engageplayerpos ") then
        data.engageplayerpos = line:gsub(".*engageplayerpos ", "");
    elseif line:match("player ") then
        line = line:gsub(".*player ", "") + 0
        if line == 1 then
            break
        end
    elseif line:match("env_obj ") then
        context = "env_obj"
        env_obj_num = line:gsub(".*env_obj ", "")
        data.env_obj[env_obj_num] = {}
    elseif line:match("env_obj_joint") then
        context = "env_obj_joint"
        env_obj_joint_num = line:gsub(".*env_obj_joint ", "")
    elseif line:match("body ") then
        context = "body"
        body_name = line:gsub(".*body ", "")
    elseif line:match("joint ")  then
        context = "joint"
        joint_name = line:gsub(".*joint ", "")
    elseif line:match("shape ") then
        if context == "body" then
            data.body[body_name].shape = line:gsub(".*shape ", "")
        elseif context == "env_obj" then
            data.env_obj[env_obj_num].shape = line:gsub(".*shape ", "")
        end
    elseif line:match("pos ") then
        if context == "body" then
            data.body[body_name].pos = line:gsub(".*pos ", "")
        elseif context == "joint" then
            data.joint[joint_name].pos = line:gsub(".*pos ", "")
        elseif context == "env_obj" then
            data.env_obj[env_obj_num].pos = line:gsub(".*pos ", "")
        end
    elseif line:match("rot ") then
        if context == "body" then
            data.body[body_name].rot = line:gsub(".*rot ", "")
        elseif context == "env_obj" then
            data.env_obj[env_obj_num].rot = line:gsub(".*rot ", "")
        end
    elseif line:match("%s+sides ") then
        if context == "body" then
            line = line:gsub(".*sides ", "")
            if data.body[body_name].shape == "sphere" then
                line = line:explode(" ")
                data.body[body_name].radius = line[1]
            elseif data.body[body_name].shape == "cylinder" then
                line = line:explode(" ")
                data.body[body_name].radius = line[1]
                data.body[body_name].length = line[2]
            else
                data.body[body_name].sides = line
            end
        elseif context == "env_obj" then
            line = line:gsub(".*sides ", "")
            if data.env_obj[env_obj_num].shape == "sphere" then
                line = line:explode(" ")
                data.env_obj[env_obj_num].radius = line[1]
            elseif data.env_obj[env_obj_num].shape == "cylinder" then
                line = line:explode(" ")
                data.env_obj[env_obj_num].radius = line[1]
                data.env_obj[env_obj_num].length = line[2]
            else
                data.env_obj[env_obj_num].sides = line
            end
        end
    elseif line:match("radius ") then
        if context == "body" then
            data.body[body_name].radius = line:gsub(".*radius ", "")
        elseif context == "joint" then
            data.joint[joint_name].radius = line:gsub(".*radius ", "")
        end
    elseif line:match("density ") then
        if context == "body" then
            data.body[body_name].density = line:gsub(".*density ", "")
        end
    elseif line:match("axis ") then
        if context == "joint" then
            data.joint[joint_name].axis = line:gsub(".*axis ", "")
        end
    elseif line:match("range ") then
        if context == "joint" then
            data.joint[joint_name].range = line:gsub(".*range ", "")
        end
    elseif line:match("strength ") then
        if context == "joint" then
            data.joint[joint_name].strength = line:gsub(".*strength ", "")
        end
    elseif line:match("velocity ") then
        if context == "joint" then
            data.joint[joint_name].velocity = line:gsub(".*velocity ", "")
        end
    elseif line:match("flag ") then
        if context == "body" then
	    data.body[body_name].flag = line:gsub(".*flag ", "")
	elseif context == "env_obj" then
	    data.env_obj[env_obj_num].flag = line:gsub(".*flag ", "")
	end
    end
end

File.ClearContent()

File.line("-- converted from " .. arg[1] .. " using convert.lua")
data.engageplayerpos = data.engageplayerpos:explode(",")
File.line("engageplayerpos {"
    .. data.engageplayerpos[1] .. ", "
    .. data.engageplayerpos[2] .. ", "
    .. data.engageplayerpos[3] .. "}")
File.line("gravity {" .. data.gravity:gsub(" ",", ") .. "}")

for body_name, body in pairs(data.body) do
    File.line("body \"" .. body_name .. "\"")
    if data.body[body_name].shape == "cylinder" then
        File.line("\tshape \"capsule\"")
    else
        File.line("\tshape \"" .. data.body[body_name].shape .. "\"")
    end
    File.line("\tposition {" .. data.body[body_name].pos:gsub(" ",", ") .. "}")
    if  data.body[body_name].rot then
        local XYZ = data.body[body_name].rot:explode(" ")
        XYZ[1] = tonumber(XYZ[1]) * math.pi/180
        XYZ[2] = tonumber(XYZ[2]) * math.pi/180
        XYZ[3] = tonumber(XYZ[3]) * math.pi/180
        local q = euler2quat(XYZ[1], XYZ[2], XYZ[3])
        File.line("\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
    end
    if body_name == "head" or data.body[body_name].shape == "sphere" then
        File.line("\tradius {" .. data.body[body_name].radius .. "}")
    elseif data.body[body_name].shape == "cylinder" then
        File.line("\tradius {" .. data.body[body_name].radius .. "}")
        File.line("\tlength {" .. data.body[body_name].length .. "}")
    else
        File.line("\tsides {" .. data.body[body_name].sides:gsub(" ",", ") .. "}")
    end
    File.line("\tdensity {" .. data.body[body_name].density .. "}")
end

for joint_name, joint in pairs(data.joint) do
    File.line("joint \"" .. joint_name .. "\"")
    File.line("\tshape \"sphere\"")
    File.line("\tradius {" .. data.joint[joint_name].radius .. "}")
    File.line("\tposition {".. data.joint[joint_name].pos:gsub(" ",", ") .. "}")
    File.line("\tdensity {0.0025}")
    File.line("\tstrength {" .. tonumber(data.joint[joint_name].strength) * 10*10*10 .. "}")
    File.line("\tvelocity {" .. data.joint[joint_name].velocity .. "}")
    data.joint[joint_name].axis = data.joint[joint_name].axis:explode(" ")
    data.joint[joint_name].axis[1] = tonumber(data.joint[joint_name].axis[1]) * -1
    data.joint[joint_name].axis[2] = tonumber(data.joint[joint_name].axis[2]) * -1
    data.joint[joint_name].axis[3] = tonumber(data.joint[joint_name].axis[3]) * -1
    File.line("\taxis {"
        .. data.joint[joint_name].axis[1] .. ", "
        .. data.joint[joint_name].axis[2] .. ", "
        .. data.joint[joint_name].axis[3] .. "}")
    data.joint[joint_name].range = data.joint[joint_name].range:explode(" ")
    if joint_name:match("r_pecs") or joint_name:match("r_elbow") or joint_name:match("r_wrist") then
        local r = data.joint[joint_name].range[1]
        data.joint[joint_name].range[1] = tonumber(data.joint[joint_name].range[2]) * -1
        data.joint[joint_name].range[2] = tonumber(r) * -1
    end
    File.line("\trange {"
    .. data.joint[joint_name].range[1] .. ", "
    .. data.joint[joint_name].range[2] .. "}")
    File.line("\tconnections {" .. data.joint[joint_name].connections .. "}")
    File.line("\tconnectionType \"hinge\"")
end

for env_obj_num, env_obj in pairs(data.env_obj) do
    File.line("object \"" .. env_obj_num .. "\"")
    if data.env_obj[env_obj_num].shape == nil then
        File.line("\tshape \"box\"")
    elseif data.env_obj[env_obj_num].shape == "cylinder" then
        File.line("\tshape \"capsule\"")
    else
        File.line("\tshape \"" .. data.env_obj[env_obj_num].shape .. "\"")
    end
    File.line("\tposition {" .. data.env_obj[env_obj_num].pos:gsub(" ", ", ") .. "}")
    if  data.env_obj[env_obj_num].rot then
        local XYZ = data.env_obj[env_obj_num].rot:explode(" ")
        XYZ[1] = tonumber(XYZ[1]) * math.pi/180
        XYZ[2] = tonumber(XYZ[2]) * math.pi/180
        XYZ[3] = tonumber(XYZ[3]) * math.pi/180
        local q = euler2quat(XYZ[1], XYZ[2], XYZ[3])
        File.line("\torientation {" .. q[1] .. ", " .. q[2] .. ", " .. q[3] .. ", " .. q[4] .. "}")
    end
    if data.env_obj[env_obj_num].shape == "sphere" then
        File.line("\tradius {" .. data.env_obj[env_obj_num].radius .. "}")
    elseif data.env_obj[env_obj_num].shape == "cylinder" then
        File.line("\tradius {" .. data.env_obj[env_obj_num].radius .. "}")
        File.line("\tlength {" .. data.env_obj[env_obj_num].length .. "}")
    else
        File.line("\tsides {" .. data.env_obj[env_obj_num].sides:gsub(" ", ", ") .. "}")
    end
    if data.env_obj[env_obj_num].density then
        File.line("\tdensity {".. data.env_obj[env_obj_num].density .."}")
    else
        File.line("\tdensity {0.25}")
    end
    if data.env_obj[env_obj_num].flag == "24" then
    	File.line("\tstatic()")
    end
end

File.write(arg[2] or "output.lua")
