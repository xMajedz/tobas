-- converted from tma_ergo_v4.tbm using convert.lua
globalplane {}
gravity {0.00, 0.00, -27.50}
body "l_thigh"
	shape "capsule"
	position {1.20, 0.00, 1.10}
	radius {0.14}
	length {0.30}
	density {0.3412}
body "chest"
	shape "box"
	position {1.00, -0.05, 1.94}
	sides {0.45, 0.14, 0.20}
	density {0.2680}
body "stomach"
	shape "box"
	position {1.00, -0.03, 1.74}
	sides {0.37, 0.12, 0.20}
	density {0.3316}
body "l_leg"
	shape "capsule"
	position {1.20, 0.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "l_pecs"
	shape "capsule"
	position {1.23, -0.02, 2.20}
	radius {0.10}
	length {0.15}
	density {0.4348}
body "l_foot"
	shape "box"
	position {1.20, -0.10, 0.10}
	sides {0.21, 0.45, 0.08}
	density {0.1096}
body "l_biceps"
	shape "box"
	position {1.37, 0.02, 2.30}
	sides {0.06, 0.02, 0.02}
	density {0.0004}
body "r_butt"
	shape "box"
	position {0.81, 0.00, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
body "l_triceps"
	shape "box"
	position {1.50, 0.00, 2.29}
	sides {0.40, 0.16, 0.16}
	density {0.1444}
body "r_thigh"
	shape "capsule"
	position {0.80, 0.00, 1.10}
	radius {0.14}
	length {0.30}
	density {0.3412}
body "head"
	shape "sphere"
	position {1.00, -0.10, 2.65}
	radius {0.20}
	density {0.0676}
body "r_leg"
	shape "capsule"
	position {0.80, 0.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "r_pecs"
	shape "capsule"
	position {0.77, -0.02, 2.20}
	radius {0.10}
	length {0.15}
	density {0.4348}
body "r_biceps"
	shape "box"
	position {0.63, 0.02, 2.29}
	sides {0.06, 0.02, 0.02}
	density {0.0004}
body "r_foot"
	shape "box"
	position {0.80, -0.10, 0.10}
	sides {0.21, 0.45, 0.08}
	density {0.1096}
body "r_triceps"
	shape "box"
	position {0.50, 0.00, 2.29}
	sides {0.40, 0.16, 0.16}
	density {0.1444}
body "l_hand"
	shape "box"
	position {1.96, 0.00, 2.28}
	sides {0.40, 0.09, 0.11}
	density {0.1444}
body "r_hand"
	shape "box"
	position {0.05, 0.00, 2.28}
	sides {0.40, 0.09, 0.11}
	density {0.1444}
body "breast"
	shape "capsule"
	position {1.00, -0.02, 2.20}
	radius {0.10}
	length {0.25}
	density {0.5748}
body "l_butt"
	shape "box"
	position {1.19, 0.00, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
body "groin"
	shape "box"
	position {1.00, 0.00, 1.53}
	sides {0.18, 0.15, 0.15}
	density {0.4212}
joint "l_ankle"
	shape "sphere"
	radius {0.11}
	position {1.20, 0.00, 0.25}
	density {0.0025}
	strength {1200}
	velocity {7.75}
	axis {1.00, 0.00, 0.00}
	range {1.10, -0.30}
	connections {"l_leg", "l_foot"}
	connectionType "hinge"
joint "chest"
	shape "sphere"
	radius {0.16}
	position {1.00, -0.05, 2.07}
	density {0.0025}
	strength {2600}
	velocity {7.15}
	axis {0.00, 0.00, 1.00}
	range {1.00, -1.00}
	connections {"breast", "chest"}
	connectionType "hinge"
joint "l_wrist"
	shape "sphere"
	radius {0.11}
	position {1.70, 0.00, 2.29}
	density {0.0025}
	strength {500}
	velocity {18.50}
	axis {0.00, 0.00, 1.00}
	range {2.30, 0.05}
	connections {"l_triceps", "l_hand"}
	connectionType "hinge"
joint "r_hip"
	shape "sphere"
	radius {0.16}
	position {0.81, 0.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {1.00, 0.08, 0.08}
	range {1.10, -2.30}
	connections {"r_butt", "r_thigh"}
	connectionType "hinge"
joint "l_pecs"
	shape "sphere"
	radius {0.17}
	position {1.13, -0.06, 2.22}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {0.40, 0.00, 1.00}
	range {1.50, -0.70}
	connections {"breast", "l_pecs"}
	connectionType "hinge"
joint "r_shoulder"
	shape "sphere"
	radius {0.14}
	position {0.70, 0.00, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.50}
	axis {0.00, 1.00, 0.00}
	range {1.30, -1.50}
	connections {"r_pecs", "r_biceps"}
	connectionType "hinge"
joint "r_elbow"
	shape "sphere"
	radius {0.11}
	position {0.63, 0.02, 2.29}
	density {0.0025}
	strength {770}
	velocity {10.00}
	axis {1.00, 0.00, 0.00}
	range {1.35, -1.00}
	connections {"r_biceps", "r_triceps"}
	connectionType "hinge"
joint "r_wrist"
	shape "sphere"
	radius {0.11}
	position {0.30, 0.00, 2.29}
	density {0.0025}
	strength {500}
	velocity {18.50}
	axis {0.00, 0.00, 1.00}
	range {2.30, 0.05}
	connections {"r_triceps", "r_hand"}
	connectionType "hinge"
joint "l_glute"
	shape "sphere"
	radius {0.15}
	position {1.10, 0.07, 1.51}
	density {0.0025}
	strength {1600}
	velocity {5.62}
	axis {0.00, 1.00, -0.25}
	range {1.00, -0.80}
	connections {"groin", "l_butt"}
	connectionType "hinge"
joint "r_ankle"
	shape "sphere"
	radius {0.11}
	position {0.80, 0.00, 0.25}
	density {0.0025}
	strength {1200}
	velocity {7.75}
	axis {1.00, 0.00, 0.00}
	range {1.10, -0.30}
	connections {"r_leg", "r_foot"}
	connectionType "hinge"
joint "abs"
	shape "sphere"
	radius {0.14}
	position {1.00, 0.00, 1.64}
	density {0.0025}
	strength {4600}
	velocity {3.18}
	axis {1.00, 0.00, 0.00}
	range {0.35, -0.70}
	connections {"stomach", "groin"}
	connectionType "hinge"
joint "l_hip"
	shape "sphere"
	radius {0.16}
	position {1.19, 0.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {1.00, -0.08, -0.08}
	range {1.10, -2.30}
	connections {"l_butt", "l_thigh"}
	connectionType "hinge"
joint "neck"
	shape "sphere"
	radius {0.11}
	position {1.00, -0.07, 2.50}
	density {0.0025}
	strength {220}
	velocity {6.50}
	axis {1.00, 0.00, 0.00}
	range {0.50, -0.50}
	connections {"head", "breast"}
	connectionType "hinge"
joint "r_knee"
	shape "sphere"
	radius {0.13}
	position {0.80, 0.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {1.00, 0.00, 0.00}
	range {2.30, 0.00}
	connections {"r_thigh", "r_leg"}
	connectionType "hinge"
joint "r_glute"
	shape "sphere"
	radius {0.15}
	position {0.90, 0.07, 1.51}
	density {0.0025}
	strength {1600}
	velocity {5.62}
	axis {0.00, 1.00, -0.25}
	range {1.00, -0.80}
	connections {"groin", "r_butt"}
	connectionType "hinge"
joint "l_shoulder"
	shape "sphere"
	radius {0.14}
	position {1.30, 0.00, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.50}
	axis {0.00, 1.00, 0.00}
	range {1.30, -1.50}
	connections {"l_pecs", "l_biceps"}
	connectionType "hinge"
joint "l_knee"
	shape "sphere"
	radius {0.13}
	position {1.20, 0.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {1.00, 0.00, 0.00}
	range {2.30, 0.00}
	connections {"l_thigh", "l_leg"}
	connectionType "hinge"
joint "lumbar"
	shape "sphere"
	radius {0.14}
	position {1.00, -0.03, 1.85}
	density {0.0025}
	strength {4000}
	velocity {3.07}
	axis {0.00, 1.00, 0.00}
	range {0.55, -0.55}
	connections {"chest", "stomach"}
	connectionType "hinge"
joint "r_pecs"
	shape "sphere"
	radius {0.17}
	position {0.87, -0.06, 2.22}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {-0.40, 0.00, 1.00}
	range {1.50, -0.70}
	connections {"breast", "r_pecs"}
	connectionType "hinge"
joint "l_elbow"
	shape "sphere"
	radius {0.11}
	position {1.37, 0.02, 2.29}
	density {0.0025}
	strength {770}
	velocity {10.00}
	axis {-1.00, 0.00, 0.00}
	range {1.35, -1.00}
	connections {"l_biceps", "l_triceps"}
	connectionType "hinge"
