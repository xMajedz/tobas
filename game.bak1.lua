-- porting mma-slowed-overhaul.tbm

gravity { 0.00, -27.00, 0.00}

object "static_game_object"
	shape "box"
	position { 0.00, -0.50, 0.00 }
	sides { 20.00, 1.00, 20.00 }
	density { 0.25 }
	static()

body "head"
	shape "sphere"
	position { 0.00, 2.65, 0.95 }
	radius { 0.20 }
	density { 0.0676 }

body "breast"
	shape "box"
	position { 0.00, 2.21, 1.02 }
	sides { 0.21, 0.30, 0.10 }
	density { 0.5748 }

body "chest"
	shape "box"
	position { 0.00, 1.95, 1.04 }
	sides { 0.47, 0.20, 0.15 }
	density { 0.2680 }

body "stomach"
	shape "box"
	position { 0.00, 1.75, 1.06 }
	sides { 0.37, 0.20, 0.13 }
	density { 0.3316 }

body "groin"
	shape "box"
	position { 0.00, 1.55, 1.12 }
	sides { 0.20, 0.19, 0.15 }
	density { 0.4212 }

for k, v in pairs({ l = 1, r = -1 }) do
	body(k .. "_pecs")
		shape "box"
		position { v * 0.15, 2.13, 1.02 }
		orientation { 0.3826834, 0, 0, 0.9238795 }
		sides { 0.25, 0.22, 0.15 }
		density { 0.4348 }

	body(k .. "_biceps")
		shape "box"
		position { v * 0.55, 2.30, 1.02 }
		sides { 0.40, 0.16, 0.14 }
		density { 0.1688 }

	body(k .. "_triceps")
		shape "box"
		position { v * 0.95, 2.30, 1.02 }
		sides { 0.40, 0.11, 0.09 }
		density { 0.1644 }

	body(k .. "_hand")
		shape "box"
		position { v * 1.35, 2.32, 0.98 }
		sides { 0.17, 0.16, 0.15 }
		density { 0.0828 }

	body(k .. "_butt")
		shape "box"
		position { v * 0.19, 1.45, 1.12 }
		sides { 0.10, 0.10, 0.10 }
		density { 2.8124 }

	body(k .. "_thigh")
		shape "capsule"
		position { v * 0.20, 1.10, 1.10 }
		orientation { 0, 0, 0.7071068, 0.7071068 }
		radius { 0.14 }
		length { 0.30 }
		density { 0.3412 }

	body(k .. "_leg")
		shape "capsule"
		position { v * 0.20, 0.50, 1.10 }
		orientation { 0, 0, 0.7071068, 0.7071068 }
		radius { 0.11 }
		length { 0.30 }
		density { 0.2512 }

	body(k .. "_foot")
		shape "box"
		position { v * 0.20, 0.10, 1.05 }
		sides { 0.20, 0.08, 0.45 }
		density { 0.1096 }
end

joint "neck"
	shape "sphere"
	position { 0.00, 2.50, 1.00 }
	radius { 0.11 }
	density { 0.0005 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.80, 0.80 }
	connections { "head", "breast" }
	connectionType "hinge"

joint "chest"
	shape "sphere"
	position { 0.00, 2.07, 1.01 }
	radius { 0.16 }
	density { 0.0005 }
	axis { 0.00, 1.00, 0.00 }
	range { -1.00, 1.00 }
	connections { "breast", "chest" }
	connectionType "hinge"

joint "lumbar"
	shape "sphere"
	position { 0.00, 1.85, 1.06 }
	radius { 0.15 }
	density { 0.0005 }
	axis { 0.00, 0.00, 1.00 }
	range { -0.40, 0.40 }
	connections { "chest", "stomach" }
	connectionType "hinge"

joint "abs"
	shape "sphere"
	position { 0.00, 1.66, 1.10 }
	radius { 0.14 }
	density { 0.0005 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.60, 0.40 }
	connections { "stomach", "groin" }
	connectionType "hinge"

local jrange =
{
	pecs = 
	{
		l = { -1.50, 0.70 };
		r = { -0.70, 1.50 };
	},
	shoulder = 
	{
		l = { -2.00, 1.50 };
		r = { -1.50, 2.00 };
	},
	elbow = 
	{
		l = { -2.50, 0.00 };
		r = { 0.00, 2.50 };
	},
	wrist =
	{
		l = { -1.50, 1.50 };
		r = { -1.50, 1.50 };
	},
	glute =
	{
		l = { -1.35, 1.35 };
		r = { -1.35, 1.35 };
	},
	hip =
	{
		l = { -2.30, 0.40 };
		r = { -2.30, 0.40 };
	},
	knee =
	{
		l = { 0.00, 2.30 };
		r = { 0.00, 2.30 };
	},
	ankle =
	{
		l = { -0.60, 1.40 };
		r = { -0.60, 1.40 };
	}
}

for k, v in pairs({ l = 1, r = -1 }) do
	joint(k .. "_pecs")
		shape "sphere"
		position { v * 0.15, 2.20, 0.98 }
		radius { 0.15 }
		density { 0.0005 }
		axis { 0.00, 1.00, 0.00 }
		range { jrange.pecs[k][1], jrange.pecs[k][2] }
		connections { "breast", k .. "_pecs" }
		connectionType "hinge"

	joint(k .. "_shoulder")
		shape "sphere"
		position { v * 0.35, 2.30, 1.00 }
		radius { 0.13 }
		density { 0.0005 }
		axis { 0.00, 0.00, 1.00 }
		range { jrange.shoulder[k][1], jrange.shoulder[k][2] }
		connections { k .. "_pecs", k .. "_biceps" }
		connectionType "hinge"

	joint(k .. "_elbow")
		shape "sphere"
		position { v * 0.75, 2.30, 1.02 }
		radius { 0.11 }
		density { 0.0005 }
		axis { 0.00, 1.00, 1.00 }
		range { jrange.elbow[k][1], jrange.elbow[k][2] }
		connections { k .. "_biceps", k .. "_triceps" }
		connectionType "hinge"

	joint(k .. "_wrist")
		shape "sphere"
		position { v * 1.20, 2.30, 1.00 }
		radius { 0.08 }
		density { 0.0005 }
		axis { 0.00, 1.00, 0.00 }
		range { jrange.wrist[k][1], jrange.wrist[k][2] }
		connections { k .. "_triceps", k .. "_hand" }
		connectionType "hinge"

	joint(k .. "_glute")
		shape "sphere"
		position { v * 0.10, 1.51, 1.20 }
		radius { 0.15 }
		density { 0.0005 }
		axis { 0.00, 0.00, 1.00 }
		range { jrange.glute[k][1], jrange.glute[k][2] }
		connections { "groin", k .."_butt" }
		connectionType "hinge"

	joint(k .. "_hip")
		shape "sphere"
		position { v * 0.21, 1.40, 1.10 }
		radius { 0.15 }
		density { 0.0005 }
		axis { 1.00, 0.00, 0.00 }
		range { jrange.hip[k][1], jrange.hip[k][2] }
		connections { k .. "_butt", k .. "_thigh" }
		connectionType "hinge"

	joint(k .. "_knee")
		shape "sphere"
		position { v * 0.20, 0.80, 1.10 }
		radius { 0.13 }
		density { 0.0005 }
		axis { 1.00, 0.00, 0.00 }
		range { jrange.knee[k][1], jrange.knee[k][2] }
		connections { k .. "_thigh", k .. "_leg" }
		connectionType "hinge"

	joint(k .. "_ankle")
		shape "sphere"
		position { v * 0.20, 0.25, 1.15 }
		radius { 0.11 }
		density { 0.0005 }
		axis { 1.00, 0.00, 0.00 }
		range { jrange.ankle[k][1], jrange.ankle[k][2] }
		connections { k .. "_leg", k .. "_foot" }
		connectionType "hinge"
end
--[[
object "dynamic_game_object_1"
	shape "box"
	position { 0.00, 2.00, 0.00 }
	sides { 1.00, 1.00, 1.00 }
	density { 0.25 }

object "dynamic_game_object_2"
	shape "box"
	position { 0.50, 1.00, 0.00 }
	sides { 1.00, 1.00, 1.00 }
	density { 0.25 }
	composite "dynamic_game_object_1"
]]
--[[
local k = 1
local i = -4.5
while i < 5 do
	local j = -4.5
	while j < 5 do
		object("dynamic_game_object_" .. k)
			shape "box"
			position { i, k * 1.50 + 1.50, j }
			sides { 1.00, 1.00, 1.00 }
			density { 0.25 }
		j = j + 2
		k = k + 1
	end
		i = i + 2
end
]]
--[[
local k = 1
local i = -4.5
while i < 5 do
	local j = -4.5
	while j < 5 do
		object("dynamic_game_object_" .. k)
			shape "sphere"
			position { i, k * 1.50 + 1.50, j}
			radius { 0.50 }
			density { 0.25 }
		j = j + 2
		k = k + 1
	end
		i = i + 2
end
]]
--[[
local k = 1
local i = -4.5
while i < 5 do
	local j = -4.5
	while j < 5 do
		object("dynamic_game_object_" .. k)
				shape "capsule"
				position { i, k * 1.50 + 1.50, j }
				radius { 0.25 }
				length { 0.50 }
				density { 0.25 }
		j = j + 2
		k = k + 1
	end
		i = i + 2
end
]]
--[[
body "body"
	shape "capsule"
	position { 1.00, 1.50, 1.00 }
	radius { 0.25 }
	length { 0.50 }
	density { 0.25 }
	]]
--[[
joint "joint"
	shape "capsule"
	position { -1.00, 1.50, -1.00 }
	radius { 0.25 }
	length { 0.50 }
	density { 0.25 }
]]
