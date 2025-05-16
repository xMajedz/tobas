-- converted from blank-mma-slowed.tbm using convert.lua
globalplane {}
gravity {0.00, 0.00, -27.50}
body "r_triceps"
	shape "box"
	position {0.05, 0.92, 2.30}
	sides {0.40, 0.09, 0.11}
	density {0.1644}
body "r_hand"
	shape "box"
	position {-0.35, 0.88, 2.32}
	sides {0.17, 0.15, 0.16}
	density {0.0828}
body "breast"
	shape "box"
	position {1.00, 0.92, 2.21}
	sides {0.21, 0.10, 0.30}
	density {0.5748}
body "l_thigh"
	shape "capsule"
	position {0.20, 1.00, 1.10}
	radius {0.14}
	length {0.30}
	density {0.3412}
body "chest"
	shape "box"
	position {1.00, 0.94, 1.95}
	sides {0.47, 0.15, 0.20}
	density {0.2680}
body "stomach"
	shape "box"
	position {1.00, 0.96, 1.75}
	sides {0.37, 0.13, 0.20}
	density {0.3316}
body "l_leg"
	shape "capsule"
	position {0.20, 1.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "l_pecs"
	shape "box"
	position {1.15, 0.92, 2.13}
	sides {0.25, 0.15, 0.22}
	density {0.4348}
body "l_foot"
	shape "box"
	position {1.20, 0.95, 0.10}
	sides {0.20, 0.45, 0.08}
	density {0.1096}
body "l_biceps"
	shape "box"
	position {1.55, 0.92, 2.30}
	sides {0.40, 0.14, 0.16}
	density {0.1688}
body "r_butt"
	shape "box"
	position {0.81, 1.02, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
body "l_triceps"
	shape "box"
	position {1.95, 0.92, 2.30}
	sides {0.40, 0.09, 0.11}
	density {0.1644}
body "r_thigh"
	shape "capsule"
	position {0.80, 1.00, 1.11}
	radius {0.14}
	length {0.30}
	density {0.3412}
body "r_foot"
	shape "box"
	position {0.80, 0.95, 0.10}
	sides {0.20, 0.45, 0.08}
	density {0.1096}
body "head"
	shape "sphere"
	position {0.00, 0.85, 2.67}
	radius {0.20}
	density {0.0676}
body "r_leg"
	shape "capsule"
	position {-0.20, 1.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "l_hand"
	shape "box"
	position {2.35, 0.88, 2.32}
	sides {0.17, 0.15, 0.16}
	density {0.0828}
body "r_pecs"
	shape "box"
	position {0.85, 0.92, 2.13}
	sides {0.25, 0.15, 0.22}
	density {0.4348}
body "groin"
	shape "box"
	position {0.00, 1.00, 1.55}
	sides {0.20, 0.15, 0.19}
	density {0.4212}
body "r_biceps"
	shape "box"
	position {0.45, 0.92, 2.30}
	sides {0.40, 0.14, 0.16}
	density {0.1688}
body "l_butt"
	shape "box"
	position {1.19, 1.02, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
joint "r_knee"
	shape "sphere"
	radius {0.13}
	position {-0.20, 1.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {1.00, 0.00, 0.00}
	range {2.30, 0.00}
	connections {"r_thigh", "r_leg"}
	connectionType "hinge"
joint "r_shoulder"
	shape "sphere"
	radius {0.13}
	position {0.35, 0.90, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.00}
	axis {0.00, 1.00, 0.00}
	range {1.20, -1.20}
	connections {"r_pecs", "r_biceps"}
	connectionType "hinge"
joint "r_elbow"
	shape "sphere"
	radius {0.11}
	position {0.25, 0.92, 2.30}
	density {0.0025}
	strength {600}
	velocity {18.50}
	axis {0.00, 0.65, 1.00}
	range {2.30, 0.05}
	connections {"r_biceps", "r_triceps"}
	connectionType "hinge"
joint "r_wrist"
	shape "sphere"
	radius {0.08}
	position {1.20, 0.90, 2.30}
	density {0.0025}
	strength {450}
	velocity {15.00}
	axis {0.00, 0.00, 1.00}
	range {1.85, 0.00}
	connections {"r_triceps", "r_hand"}
	connectionType "hinge"
joint "chest"
	shape "sphere"
	radius {0.16}
	position {1.00, 0.91, 2.07}
	density {0.0025}
	strength {2600}
	velocity {7.15}
	axis {0.00, 0.00, 1.00}
	range {1.00, -1.00}
	connections {"breast", "chest"}
	connectionType "hinge"
joint "l_hip"
	shape "sphere"
	radius {0.15}
	position {0.21, 1.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {1.00, 0.00, 0.00}
	range {1.10, -1.90}
	connections {"l_butt", "l_thigh"}
	connectionType "hinge"
joint "l_pecs"
	shape "sphere"
	radius {0.15}
	position {1.15, 0.88, 2.20}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {0.00, 0.00, 1.00}
	range {1.50, -0.70}
	connections {"breast", "l_pecs"}
	connectionType "hinge"
joint "lumbar"
	shape "sphere"
	radius {0.15}
	position {1.00, 0.96, 1.85}
	density {0.0025}
	strength {4000}
	velocity {3.07}
	axis {0.00, 1.00, 0.00}
	range {0.55, -0.55}
	connections {"chest", "stomach"}
	connectionType "hinge"
joint "r_ankle"
	shape "sphere"
	radius {0.11}
	position {-0.20, 1.05, 0.25}
	density {0.0025}
	strength {900}
	velocity {7.75}
	axis {1.00, 0.00, 0.00}
	range {1.10, -0.30}
	connections {"r_leg", "r_foot"}
	connectionType "hinge"
joint "l_elbow"
	shape "sphere"
	radius {0.11}
	position {1.75, 0.92, 2.30}
	density {0.0025}
	strength {600}
	velocity {18.50}
	axis {0.00, 0.65, 1.00}
	range {2.30, 0.05}
	connections {"l_biceps", "l_triceps"}
	connectionType "hinge"
joint "l_ankle"
	shape "sphere"
	radius {0.11}
	position {0.20, 1.05, 0.25}
	density {0.0025}
	strength {900}
	velocity {7.75}
	axis {1.00, 0.00, 0.00}
	range {1.10, -0.30}
	connections {"l_leg", "l_foot"}
	connectionType "hinge"
joint "r_hip"
	shape "sphere"
	radius {0.15}
	position {-0.21, 1.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {1.00, 0.00, 0.00}
	range {1.10, -1.90}
	connections {"r_butt", "r_thigh"}
	connectionType "hinge"
joint "r_glute"
	shape "sphere"
	radius {0.15}
	position {-0.10, 1.10, 1.51}
	density {0.0025}
	strength {1550}
	velocity {5.62}
	axis {0.00, 1.00, -0.25}
	range {1.00, -0.40}
	connections {"groin", "r_butt"}
	connectionType "hinge"
joint "abs"
	shape "sphere"
	radius {0.14}
	position {1.00, 1.00, 1.66}
	density {0.0025}
	strength {4600}
	velocity {3.18}
	axis {1.00, 0.00, 0.00}
	range {0.35, -0.70}
	connections {"stomach", "groin"}
	connectionType "hinge"
joint "l_shoulder"
	shape "sphere"
	radius {0.13}
	position {1.35, 0.92, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.00}
	axis {0.00, 1.00, 0.00}
	range {1.20, -1.20}
	connections {"l_pecs", "l_biceps"}
	connectionType "hinge"
joint "l_wrist"
	shape "sphere"
	radius {0.08}
	position {1.20, 0.90, 2.30}
	density {0.0025}
	strength {450}
	velocity {15.00}
	axis {0.00, 0.00, 1.00}
	range {1.85, 0.00}
	connections {"l_triceps", "l_hand"}
	connectionType "hinge"
joint "r_pecs"
	shape "sphere"
	radius {0.15}
	position {0.85, 0.88, 2.20}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {0.00, 0.00, 1.00}
	range {1.50, -0.70}
	connections {"breast", "r_pecs"}
	connectionType "hinge"
joint "l_glute"
	shape "sphere"
	radius {0.15}
	position {0.10, 1.10, 1.51}
	density {0.0025}
	strength {1550}
	velocity {5.62}
	axis {0.00, 1.00, -0.25}
	range {1.00, -0.40}
	connections {"groin", "l_butt"}
	connectionType "hinge"
joint "neck"
	shape "sphere"
	radius {0.11}
	position {0.00, 0.90, 2.50}
	density {0.0025}
	strength {220}
	velocity {6.50}
	axis {1.00, 0.00, 0.00}
	range {0.50, -0.50}
	connections {"head", "breast"}
	connectionType "hinge"
joint "l_knee"
	shape "sphere"
	radius {0.13}
	position {0.20, 1.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {1.00, 0.00, 0.00}
	range {2.30, 0.00}
	connections {"l_thigh", "l_leg"}
	connectionType "hinge"
