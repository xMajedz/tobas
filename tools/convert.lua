local File = {Content = "", Lines = {}}

function File.ClearContent()
    File.Content = ""
    File.Lines = {}
end

function File.readLines(filename)
    if (filename) then
        f = io.open(filename, "r")
        --File.Content = f:read("*all")
        local line = f:read()
        while line ~= nil do
            line = line:gsub("\n", "")
            line = line:gsub("\r", "")
            table.insert(File.Lines, line)
            line = f:read()
        end
        f:close()
        return File.Lines
    else
        print("Cannot read file.")
        f:close()
        return nil
    end
end

function File.write(filename)
    f = io.open(filename, "w")
    f:write(File.Content)
    f:close()

    File.ClearContent()
end

function File.line(line)
    File.Content = File.Content .. line .. "\n"
end

function string.explode(str, sstr)
    if str ~= "" and sstr ~= "" then
        local parts = {}
        local a = 1
        local b = 1
        while true do
            b = str:find(sstr, a)
            if b == nil then
                table.insert(parts, str:sub(a, str:len()))
                break
            else
                table.insert(parts, str:sub(a, b - sstr:len()))
            end
            a = b + sstr:len()
        end
        return parts
    end
    return nil
end

local source = File.readLines(arg[1])

local data = { env_obj = {}, env_obj_joint = {}, body = {}, joint = {}}

data.body["head"] = {}
data.body["head"].shape = "sphere"
data.body["head"].pos = "0.00 0.85 2.67"
data.body["head"].sides = "0.20 0.00 0.00"
data.body["head"].radius = "0.20"
data.body["head"].density = "0.25"

data.body["breast"] = {}
data.body["breast"].shape = "box"
data.body["breast"].pos = "0.00 0.90 2.20"
data.body["breast"].sides = "0.20 0.10 0.30"
data.body["breast"].density = "0.25"

data.body["chest"] = {}
data.body["chest"].shape = "box"
data.body["chest"].pos = "0.00 0.90 1.95"
data.body["chest"].sides = "0.50 0.15 0.20"
data.body["chest"].density = "0.25"

data.body["stomach"] = {}
data.body["stomach"].shape = "box"
data.body["stomach"].pos = "0.00 0.95 1.75"
data.body["stomach"].sides = "0.35 0.15 0.20"
data.body["stomach"].density = "0.25"

data.body["groin"] = {}
data.body["groin"].shape = "box"
data.body["groin"].pos = "0.00 1.00 1.55"
data.body["groin"].sides = "0.20 0.15 0.20"
data.body["groin"].density = "0.25"

data.body["l_pecs"] = {}
data.body["l_pecs"].shape = "box"
data.body["l_pecs"].pos = "0.25 0.90 2.15"
data.body["l_pecs"].sides = "0.20 0.15 0.20"
data.body["l_pecs"].density = "0.25"

data.body["l_biceps"] = {}
data.body["l_biceps"].shape = "box"
data.body["l_biceps"].pos = "0.55 0.90 2.30"
data.body["l_biceps"].sides = "0.40 0.20 0.20"
data.body["l_biceps"].density = "0.25"

data.body["l_triceps"] = {}
data.body["l_triceps"].shape = "box"
data.body["l_triceps"].pos = "0.95 0.90 2.30"
data.body["l_triceps"].sides = "0.40 0.10 0.10"
data.body["l_triceps"].density = "0.25"

data.body["l_hand"] = {}
data.body["l_hand"].shape = "box"
data.body["l_hand"].pos = "1.35 0.85 2.30"
data.body["l_hand"].sides = "0.22 0.22 0.22"
data.body["l_hand"].density = "0.25"

data.body["r_pecs"] = {}
data.body["r_pecs"].shape = "box"
data.body["r_pecs"].pos = "-0.25 0.90 2.15"
data.body["r_pecs"].sides = "0.20 0.15 0.20"
data.body["r_pecs"].density = "0.25"

data.body["r_biceps"] = {}
data.body["r_biceps"].shape = "box"
data.body["r_biceps"].pos = "-0.55 0.90 2.30"
data.body["r_biceps"].sides = "0.40 0.20 0.20"
data.body["r_biceps"].density = "0.25"

data.body["r_triceps"] = {}
data.body["r_triceps"].shape = "box"
data.body["r_triceps"].pos = "-0.95 0.90 2.30"
data.body["r_triceps"].sides = "0.40 0.10 0.10"
data.body["r_triceps"].density = "0.25"

data.body["r_hand"] = {}
data.body["r_hand"].shape = "box"
data.body["r_hand"].pos = "-1.35 0.85 2.30"
data.body["r_hand"].sides = "0.22 0.22 0.22"
data.body["r_hand"].density = "0.25"

data.body["l_butt"] = {}
data.body["l_butt"].shape = "box"
data.body["l_butt"].pos = "0.20 1.00 1.45"
data.body["l_butt"].sides = "0.10 0.10 0.10"
data.body["l_butt"].density = "0.25"

data.body["l_thigh"] = {}
data.body["l_thigh"].shape = "cylinder"
data.body["l_thigh"].pos = "0.20 1.00 1.10"
data.body["l_thigh"].sides = "0.12 0.30 0.20"
data.body["l_thigh"].radius = "0.12"
data.body["l_thigh"].length = "0.30"
data.body["l_thigh"].density = "0.25"

data.body["l_leg"] = {}
data.body["l_leg"].shape = "cylinder"
data.body["l_leg"].pos = "0.20 1.00 0.50"
data.body["l_leg"].sides = "0.13 0.30 0.00"
data.body["l_leg"].radius = "0.13"
data.body["l_leg"].length = "0.30"
data.body["l_leg"].density = "0.25"

data.body["l_foot"] = {}
data.body["l_foot"].shape = "box"
data.body["l_foot"].pos = "0.20 0.90 0.10"
data.body["l_foot"].sides = "0.20 0.50 0.08"
data.body["l_foot"].density = "0.25"

data.body["r_butt"] = {}
data.body["r_butt"].shape = "box"
data.body["r_butt"].pos = "-0.20 1.00 1.45"
data.body["r_butt"].sides = "0.10 0.10 0.10"
data.body["r_butt"].density = "0.25"

data.body["r_thigh"] = {}
data.body["r_thigh"].shape = "cylinder"
data.body["r_thigh"].pos = "-0.20 1.00 1.10"
data.body["r_thigh"].sides = "0.12 0.30 0.00"
data.body["r_thigh"].radius = "0.12"
data.body["r_thigh"].length = "0.30"
data.body["r_thigh"].density = "0.25"

data.body["r_leg"] = {}
data.body["r_leg"].shape = "cylinder"
data.body["r_leg"].pos = "-0.20 1.00 0.50"
data.body["r_leg"].sides = "0.13 0.30 0.00"
data.body["r_leg"].radius = "0.13"
data.body["r_leg"].length = "0.30"
data.body["r_leg"].density = "0.25"

data.body["r_foot"] = {}
data.body["r_foot"].shape = "box"
data.body["r_foot"].pos = "-0.20 0.90 0.10"
data.body["r_foot"].sides = "0.20 0.50 0.08"
data.body["r_foot"].density = "0.25"

data.joint["neck"] = {}
data.joint["neck"].radius = "0.11"
data.joint["neck"].pos = "0.00 0.90 2.50"
data.joint["neck"].strength = "1.00"
data.joint["neck"].velocity = "50.00"
data.joint["neck"].axis = "1.00 0.00 0.00"
data.joint["neck"].range = "0.50 -0.50"
data.joint["neck"].connections = "\"head\", \"breast\""

data.joint["chest"] = {}
data.joint["chest"].radius = "0.18"
data.joint["chest"].pos = "0.00 0.90 2.05"
data.joint["chest"].strength = "4.00"
data.joint["chest"].velocity = "20.00"
data.joint["chest"].axis = "0.00 0.00 1.00"
data.joint["chest"].range = "1.00 -1.00"
data.joint["chest"].connections = "\"breast\", \"chest\""

data.joint["lumbar"] = {}
data.joint["lumbar"].radius = "0.16"
data.joint["lumbar"].pos = "0.00 0.95 1.85"
data.joint["lumbar"].strength = "3.00"
data.joint["lumbar"].velocity = "5.00"
data.joint["lumbar"].axis = "0.00 1.00 0.00"
data.joint["lumbar"].range = "0.50 -0.50"
data.joint["lumbar"].connections = "\"chest\", \"stomach\""

data.joint["abs"] = {}
data.joint["abs"].radius = "0.14"
data.joint["abs"].pos = "0.00 1.00 1.65"
data.joint["abs"].strength = "3.00"
data.joint["abs"].velocity = "5.00"
data.joint["abs"].axis = "1.00 0.00 0.00"
data.joint["abs"].range = "0.40 -0.60"
data.joint["abs"].connections = "\"stomach\", \"groin\""

data.joint["l_pecs"] = {}
data.joint["l_pecs"].radius = "0.18"
data.joint["l_pecs"].pos = "0.15, 0.85 2.20"
data.joint["l_pecs"].strength = "3.00"
data.joint["l_pecs"].velocity = "15.00"
data.joint["l_pecs"].axis = "0.00 0.00 1.00"
data.joint["l_pecs"].range = "1.50 -0.70"
data.joint["l_pecs"].connections = "\"breast\", \"l_pecs\""

data.joint["l_shoulder"] = {}
data.joint["l_shoulder"].radius = "0.18"
data.joint["l_shoulder"].pos = "0.35 0.90 2.30"
data.joint["l_shoulder"].strength = "4.00"
data.joint["l_shoulder"].velocity = "20.00"
data.joint["l_shoulder"].axis = "0.00 1.00 0.00"
data.joint["l_shoulder"].range = "1.20 -1.20"
data.joint["l_shoulder"].connections = "\"l_pecs\", \"l_biceps\""

data.joint["l_elbow"] = {}
data.joint["l_elbow"].radius = "0.16"
data.joint["l_elbow"].pos = "0.75 0.90 2.30"
data.joint["l_elbow"].strength = "2.00"
data.joint["l_elbow"].velocity = "40.00"
data.joint["l_elbow"].axis = "0.00 0.00 1.00"
data.joint["l_elbow"].range = "2.30 0.05"
data.joint["l_elbow"].connections = "\"l_biceps\", \"l_triceps\""

data.joint["l_wrist"] = {}
data.joint["l_wrist"].radius = "0.11"
data.joint["l_wrist"].pos = "1.20 0.90 2.30"
data.joint["l_wrist"].strength = "2.00"
data.joint["l_wrist"].velocity = "60.00"
data.joint["l_wrist"].axis = "0.00 0.00 1.00"
data.joint["l_wrist"].range = "2.00 -0.30"
data.joint["l_wrist"].connections = "\"l_triceps\", \"l_hand\""

data.joint["r_pecs"] = {}
data.joint["r_pecs"].radius = "0.18"
data.joint["r_pecs"].pos = "-0.15, 0.85 2.20"
data.joint["r_pecs"].strength = "3.00"
data.joint["r_pecs"].velocity = "15.00"
data.joint["r_pecs"].axis = "0.00 0.00 1.00"
data.joint["r_pecs"].range = "1.50 -0.70"
data.joint["r_pecs"].connections = "\"breast\", \"r_pecs\""

data.joint["r_shoulder"] = {}
data.joint["r_shoulder"].radius = "0.18"
data.joint["r_shoulder"].pos = "0.35 0.90 2.30"
data.joint["r_shoulder"].strength = "4.00"
data.joint["r_shoulder"].velocity = "20.00"
data.joint["r_shoulder"].axis = "0.00 1.00 0.00"
data.joint["r_shoulder"].range = "1.20 -1.20"
data.joint["r_shoulder"].connections = "\"r_pecs\", \"r_biceps\""

data.joint["r_elbow"] = {}
data.joint["r_elbow"].radius = "0.16"
data.joint["r_elbow"].pos = "0.75 0.90 2.30"
data.joint["r_elbow"].strength = "2.00"
data.joint["r_elbow"].velocity = "40.00"
data.joint["r_elbow"].axis = "0.00 0.00 1.00"
data.joint["r_elbow"].range = "2.30 0.05"
data.joint["r_elbow"].connections = "\"r_biceps\", \"r_triceps\""

data.joint["r_wrist"] = {}
data.joint["r_wrist"].radius = "0.11"
data.joint["r_wrist"].pos = "1.20 0.90 2.30"
data.joint["r_wrist"].strength = "2.00"
data.joint["r_wrist"].velocity = "60.00"
data.joint["r_wrist"].axis = "0.00 0.00 1.00"
data.joint["r_wrist"].range = "2.00 -0.30"
data.joint["r_wrist"].connections = "\"r_triceps\", \"r_hand\""

data.joint["l_glute"] = {}
data.joint["l_glute"].radius = "0.16"
data.joint["l_glute"].pos = "0.10 1.10 1.51"
data.joint["l_glute"].strength = "4.00"
data.joint["l_glute"].velocity = "10.00"
data.joint["l_glute"].axis = "0.00 1.00 0.00"
data.joint["l_glute"].range = "1.20 -0.20"
data.joint["l_glute"].connections = "\"groin\", \"l_butt\""

data.joint["l_hip"] = {}
data.joint["l_hip"].radius = "0.16"
data.joint["l_hip"].pos = "0.21 1.00 1.40"
data.joint["l_hip"].strength = "2.00"
data.joint["l_hip"].velocity = "12.00"
data.joint["l_hip"].axis = "1.00 0.00 0.00"
data.joint["l_hip"].range = "1.00 -1.80"
data.joint["l_hip"].connections = "\"l_butt\", \"l_thigh\""

data.joint["l_knee"] = {}
data.joint["l_knee"].radius = "0.14"
data.joint["l_knee"].pos = "0.20 1.00 0.80"
data.joint["l_knee"].strength = "4.00"
data.joint["l_knee"].velocity = "40.00"
data.joint["l_knee"].axis = "1.00 0.00 0.00"
data.joint["l_knee"].range = "2.30 0.00"
data.joint["l_knee"].connections = "\"l_thigh\", \"l_leg\""

data.joint["l_ankle"] = {}
data.joint["l_ankle"].radius = "0.11"
data.joint["l_ankle"].pos = "0.20 1.05 0.25"
data.joint["l_ankle"].strength = "1.00"
data.joint["l_ankle"].velocity = "8.00"
data.joint["l_ankle"].axis = "1.00 0.00 0.00"
data.joint["l_ankle"].range = "1.10 -0.30"
data.joint["l_ankle"].connections = "\"l_leg\", \"l_foot\""

data.joint["r_glute"] = {}
data.joint["r_glute"].radius = "0.16"
data.joint["r_glute"].pos = "-0.10 1.10 1.51"
data.joint["r_glute"].strength = "4.00"
data.joint["r_glute"].velocity = "10.00"
data.joint["r_glute"].axis = "0.00 1.00 0.00"
data.joint["r_glute"].range = "1.20 -0.20"
data.joint["r_glute"].connections = "\"groin\", \"r_butt\""

data.joint["r_hip"] = {}
data.joint["r_hip"].radius = "0.16"
data.joint["r_hip"].pos = "-0.21 1.00 1.40"
data.joint["r_hip"].strength = "2.00"
data.joint["r_hip"].velocity = "12.00"
data.joint["r_hip"].axis = "1.00 0.00 0.00"
data.joint["r_hip"].range = "1.00 -1.80"
data.joint["r_hip"].connections = "\"r_butt\", \"r_thigh\""

data.joint["r_knee"] = {}
data.joint["r_knee"].radius = "0.14"
data.joint["r_knee"].pos = "-0.20 1.00 0.80"
data.joint["r_knee"].strength = "4.00"
data.joint["r_knee"].velocity = "40.00"
data.joint["r_knee"].axis = "1.00 0.00 0.00"
data.joint["r_knee"].range = "2.30 0.00"
data.joint["r_knee"].connections = "\"r_thigh\", \"r_leg\""

data.joint["r_ankle"] = {}
data.joint["r_ankle"].radius = "0.11"
data.joint["r_ankle"].pos = "-0.20 1.05 0.25"
data.joint["r_ankle"].strength = "1.00"
data.joint["r_ankle"].velocity = "8.00"
data.joint["r_ankle"].axis = "1.00 0.00 0.00"
data.joint["r_ankle"].range = "1.10 -0.30"
data.joint["r_ankle"].connections = "\"r_leg\", \"r_foot\""

local env_obj_num = ""

local env_obj_joint_num = ""

local body_name = ""

local joint_name = ""

local context = ""

for _, line in pairs(source) do
    if line:match("gravity ") then
        data.gravity = line:gsub(".*gravity ", "")
    elseif line:match("player ") then
        line = line:gsub(".*player ", "") + 0
        if line == 1 then
            break
        end
    elseif line:match("env_obj ") then
        context = "env_obj"
        env_obj_num = line:gsub(".*env_obj ", "")
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
        end
    elseif line:match("pos ") then
        if context == "body" then
            data.body[body_name].pos = line:gsub(".*pos ", "")
        elseif context == "joint" then
            data.joint[joint_name].pos = line:gsub(".*pos ", "")
        end
    elseif line:match("rot ") then
        if context == "body" then
            data.body[body_name].rot = line:gsub(".*rot ", "")
        end
    elseif line:match("sides ") then
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
    end
end

File.ClearContent()

File.line("-- converted from " .. arg[1] .. " using convert.lua")

File.line("globalplane {}")

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
--        File.line("\torientation {" .. data.body[body_name].rot:gsub(" ", ", ") .. "}")
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
    File.line("\taxis {" .. data.joint[joint_name].axis:gsub(" ",", ") .. "}")
    File.line("\trange {" .. data.joint[joint_name].range:gsub(" ",", ") .. "}")
    File.line("\tconnections {" .. data.joint[joint_name].connections .. "}")
    File.line("\tconnectionType \"hinge\"")
end

File.write(arg[2] or "output.lua")
