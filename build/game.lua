-- converted from TEQmmaWIP18.tbm using convert.lua
globalplane {}
gravity {0.00, 0.00, -84.00}
body "l_pecs"
	shape "box"
	position {1.19, 1.02, 2.42}
	orientation {0.99691733373313, 0, -0.078459095727845, 0}
	sides {0.26, 0.12, 0.44}
	density {0.0939}
body "l_foot"
	shape "box"
	position {1.20, 0.88, 0.09}
	sides {0.18, 0.47, 0.07}
	density {0.0240}
body "l_biceps"
	shape "box"
	position {1.62, 1.02, 2.47}
	sides {0.44, 0.17, 0.19}
	density {0.0444}
body "r_butt"
	shape "box"
	position {0.83, 1.00, 1.76}
	sides {0.10, 0.09, 0.11}
	density {0.0750}
body "l_triceps"
	shape "box"
	position {2.07, 1.02, 2.47}
	sides {0.46, 0.10, 0.10}
	density {0.0258}
body "r_thigh"
	shape "capsule"
	position {0.80, 1.00, 1.30}
	radius {0.14}
	length {0.48}
	density {0.1686}
body "head"
	shape "sphere"
	position {1.00, 0.96, 2.91}
	radius {0.20}
	density {0.1164}
body "r_leg"
	shape "capsule"
	position {0.80, 1.01, 0.56}
	radius {0.11}
	length {0.44}
	density {0.0696}
body "r_pecs"
	shape "box"
	position {0.81, 1.02, 2.42}
	orientation {0.99691733373313, 0, 0.078459095727845, 0}
	sides {0.26, 0.12, 0.44}
	density {0.0939}
body "r_biceps"
	shape "box"
	position {0.38, 1.02, 2.47}
	sides {0.44, 0.17, 0.19}
	density {0.0444}
body "r_triceps"
	shape "box"
	position {-0.07, 1.02, 2.47}
	sides {0.46, 0.10, 0.10}
	density {0.0258}
body "r_hand"
	shape "box"
	position {-0.48, 1.02, 2.47}
	sides {0.18, 0.18, 0.18}
	density {0.0120}
body "breast"
	shape "capsule"
	position {1.00, 1.01, 2.52}
	radius {0.11}
	length {0.17}
	density {0.0939}
body "l_thigh"
	shape "capsule"
	position {1.20, 1.00, 1.30}
	radius {0.14}
	length {0.48}
	density {0.1686}
body "chest"
	shape "box"
	position {1.00, 0.96, 2.08}
	sides {0.50, 0.12, 0.19}
	density {0.0960}
body "r_foot"
	shape "box"
	position {0.80, 0.88, 0.09}
	sides {0.18, 0.47, 0.07}
	density {0.0240}
body "stomach"
	shape "box"
	position {1.00, 0.97, 1.90}
	sides {0.44, 0.10, 0.17}
	density {0.0960}
body "l_hand"
	shape "box"
	position {2.48, 1.02, 2.47}
	sides {0.18, 0.18, 0.18}
	density {0.0120}
body "groin"
	shape "box"
	position {1.00, 1.00, 1.73}
	sides {0.15, 0.17, 0.17}
	density {0.0750}
body "l_leg"
	shape "capsule"
	position {1.20, 1.01, 0.56}
	radius {0.11}
	length {0.44}
	density {0.0696}
body "l_butt"
	shape "box"
	position {1.17, 1.00, 1.76}
	sides {0.10, 0.09, 0.11}
	density {0.0750}
joint "r_wrist"
	shape "sphere"
	radius {0.09}
	position {-0.30, 1.02, 2.47}
	density {0.0025}
	strength {180}
	velocity {30.00}
	axis {-0, -0, -1}
	range {0.3, -2}
	connections {"r_triceps", "r_hand"}
	connectionType "hinge"
joint "l_pecs"
	shape "sphere"
	radius {0.20}
	position {1.11, 0.97, 2.38}
	density {0.0025}
	strength {1500}
	velocity {12.00}
	axis {-0, -0, -1}
	range {1.50, -0.70}
	connections {"breast", "l_pecs"}
	connectionType "hinge"
joint "lumbar"
	shape "sphere"
	radius {0.15}
	position {1.00, 0.96, 2.00}
	density {0.0025}
	strength {1800}
	velocity {6.00}
	axis {-0, -1, -0}
	range {0.50, -0.50}
	connections {"chest", "stomach"}
	connectionType "hinge"
joint "l_ankle"
	shape "sphere"
	radius {0.09}
	position {1.20, 1.00, 0.20}
	density {0.0025}
	strength {1100}
	velocity {10.00}
	axis {-1, -0, -0}
	range {1.10, -0.30}
	connections {"l_leg", "l_foot"}
	connectionType "hinge"
joint "r_glute"
	shape "sphere"
	radius {0.16}
	position {0.89, 1.06, 1.70}
	density {0.0025}
	strength {1700}
	velocity {8.50}
	axis {-0, -1, -0}
	range {1.20, -0.20}
	connections {"groin", "r_butt"}
	connectionType "hinge"
joint "l_shoulder"
	shape "sphere"
	radius {0.16}
	position {1.40, 1.02, 2.47}
	density {0.0025}
	strength {840}
	velocity {17.00}
	axis {-0, -1, -0}
	range {1.40, -1.40}
	connections {"l_pecs", "l_biceps"}
	connectionType "hinge"
joint "r_pecs"
	shape "sphere"
	radius {0.20}
	position {0.89, 0.97, 2.38}
	density {0.0025}
	strength {1500}
	velocity {12.00}
	axis {-0, -0, -1}
	range {0.7, -1.5}
	connections {"breast", "r_pecs"}
	connectionType "hinge"
joint "neck"
	shape "sphere"
	radius {0.10}
	position {1.00, 1.00, 2.72}
	density {0.0025}
	strength {1000}
	velocity {42.50}
	axis {-1, -0, -0}
	range {0.50, -0.50}
	connections {"head", "breast"}
	connectionType "hinge"
joint "l_elbow"
	shape "sphere"
	radius {0.14}
	position {1.84, 1.02, 2.47}
	density {0.0025}
	strength {570}
	velocity {34.00}
	axis {-0, -0, -1}
	range {2.30, 0.05}
	connections {"l_biceps", "l_triceps"}
	connectionType "hinge"
joint "r_ankle"
	shape "sphere"
	radius {0.09}
	position {0.80, 1.00, 0.20}
	density {0.0025}
	strength {1100}
	velocity {10.00}
	axis {-1, -0, -0}
	range {1.10, -0.30}
	connections {"r_leg", "r_foot"}
	connectionType "hinge"
joint "l_wrist"
	shape "sphere"
	radius {0.09}
	position {2.30, 1.02, 2.47}
	density {0.0025}
	strength {180}
	velocity {30.00}
	axis {-0, -0, -1}
	range {2.00, -0.30}
	connections {"l_triceps", "l_hand"}
	connectionType "hinge"
joint "r_knee"
	shape "sphere"
	radius {0.13}
	position {0.80, 1.00, 0.92}
	density {0.0025}
	strength {2000}
	velocity {30.00}
	axis {-1, -0, -0}
	range {2.30, 0.00}
	connections {"r_thigh", "r_leg"}
	connectionType "hinge"
joint "chest"
	shape "sphere"
	radius {0.17}
	position {1.00, 0.96, 2.20}
	density {0.0025}
	strength {2100}
	velocity {13.50}
	axis {-0, -0, -1}
	range {1.00, -1.00}
	connections {"breast", "chest"}
	connectionType "hinge"
joint "r_hip"
	shape "sphere"
	radius {0.14}
	position {0.80, 1.00, 1.68}
	density {0.0025}
	strength {2370}
	velocity {10.20}
	axis {-1, -0, -0}
	range {1.00, -1.80}
	connections {"r_butt", "r_thigh"}
	connectionType "hinge"
joint "abs"
	shape "sphere"
	radius {0.14}
	position {1.00, 0.99, 1.81}
	density {0.0025}
	strength {2300}
	velocity {6.00}
	axis {-0, -0, -0}
	range {0.40, -0.60}
	connections {"stomach", "groin"}
	connectionType "hinge"
joint "l_hip"
	shape "sphere"
	radius {0.14}
	position {1.20, 1.00, 1.68}
	density {0.0025}
	strength {2370}
	velocity {10.20}
	axis {-1, -0, -0}
	range {1.00, -1.80}
	connections {"l_butt", "l_thigh"}
	connectionType "hinge"
joint "r_shoulder"
	shape "sphere"
	radius {0.16}
	position {0.60, 1.02, 2.47}
	density {0.0025}
	strength {840}
	velocity {17.00}
	axis {-0, -1, -0}
	range {1.40, -1.40}
	connections {"r_pecs", "r_biceps"}
	connectionType "hinge"
joint "l_glute"
	shape "sphere"
	radius {0.16}
	position {1.11, 1.06, 1.70}
	density {0.0025}
	strength {1700}
	velocity {8.50}
	axis {-0, -1, -0}
	range {1.20, -0.20}
	connections {"groin", "l_butt"}
	connectionType "hinge"
joint "r_elbow"
	shape "sphere"
	radius {0.14}
	position {0.16, 1.02, 2.47}
	density {0.0025}
	strength {570}
	velocity {34.00}
	axis {-0, -0, -1}
	range {-0.05, -2.3}
	connections {"r_biceps", "r_triceps"}
	connectionType "hinge"
joint "l_knee"
	shape "sphere"
	radius {0.13}
	position {1.20, 1.00, 0.92}
	density {0.0025}
	strength {2000}
	velocity {30.00}
	axis {-1, -0, -0}
	range {2.30, 0.00}
	connections {"l_thigh", "l_leg"}
	connectionType "hinge"
