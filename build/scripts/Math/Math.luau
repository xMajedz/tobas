Math = {DEG2RAD = math.pi/180}

function Math.MatrixMultiply(A, B)
	local M = {}
	if #A[1] == #B then
		for i = 1, #A do
			local Row = {}
			for j = 1, #B[1] do
				local Sum = 0
				for k = 1, #B[1] do
					Sum = Sum + A[i][k] * B[k][j]
				end
				table.insert(Row, Sum)
			end
			table.insert(M, Row)
		end
	end
	return M
end

function Math.EulerToMatrix(u, v, w)
	local C = math.cos
	local S = math.sin
	local Rx = {
		{1, 0, 0},
		{0, C(u), -S(u)},
		{0, S(u), C(u)},
	}
	local Ry = {
		{C(v), 0, S(v)},
		{0, 1, 0},
		{-S(v), 0, C(v)},
	}
	local Rz = {
		{C(w), -S(w), 0},
		{S(w), C(w), 0},
		{0, 0, 1}
	}
	return Math.MatrixMultiply(Math.MatrixMultiply(Rx, Ry), Rz)
end

function Math.MatrixToQuaternion(M)
	local sqrt = math.sqrt
	local Trace = M[1][1] + M[2][2] + M[3][3]
	local S, Qw, Qx, Qy, Qz
	if Trace > 0 then
		S = sqrt(1 + Trace) * 2
		Qw = 0.25 * S 
		Qx = (M[3][2] - M[2][3])/S
		Qy = (M[1][3] - M[3][1])/S
		Qz = (M[2][1] - M[1][2])/S
	elseif ((M[1][1] > M[2][2]) and (M[1][1] > M[3][3])) then
		S = sqrt(1 + M[1][1] - M[2][2] - M[3][3]) * 2
		Qw = (M[3][2] - M[2][3])/S
		Qx = 0.25 * S
		Qy = (M[1][2] + M[2][1])/S
		Qz = (M[1][3] + M[3][1])/S
	elseif (M[2][2] > M[3][3]) then
		S = sqrt(1 + M[2][2] - M[1][1] - M[3][3]) * 2
		Qw = (M[1][3] - M[3][1])/S
		Qx = (M[1][2] + M[2][1])/S
		Qy = 0.25 * S
		Qz = (M[2][3] + M[3][2])/S
	else
		S = sqrt(1 + M[3][3] - M[1][1] - M[2][2]) * 2
		Qw = (M[2][1] - M[1][2])/S
		Qx = (M[1][3] + M[3][1])/S
		Qy = (M[2][3] + M[3][2])/S
		Qz = 0.25 * S
	end
	return {Qw, Qx, Qy, Qz}
end

function Math.QuaternionMultiply (q, p)
	return {
		q[1] * p[1] - q[2] * p[2] - q[3] * p[3] - q[4] * p[4],
		q[1] * p[2] + q[2] * p[1] - q[3] * p[4] + q[4] * p[3],
		q[1] * p[3] + q[2] * p[4] + q[3] * p[1] - q[4] * p[2],
		q[1] * p[4] - q[2] * p[3] + q[3] * p[2] + q[4] * p[1],
	}
end

function Math.EulerToQuaternion (u, v, w)
	local C = math.cos
	local S = math.sin
	return {
		C(0.5 * u) * C(0.5 * v) * C(0.5 * w) + S(0.5 * u) * S(0.5 * v) * S(0.5 * w),
		S(0.5 * u) * C(0.5 * v) * C(0.5 * w) - C(0.5 * u) * S(0.5 * v) * S(0.5 * w),
		C(0.5 * u) * S(0.5 * v) * C(0.5 * w) + S(0.5 * u) * C(0.5 * v) * S(0.5 * w),
		C(0.5 * u) * C(0.5 * v) * S(0.5 * w) - S(0.5 * u) * S(0.5 * v) * C(0.5 * w),
	}
end

return Math
