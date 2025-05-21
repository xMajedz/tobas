-- converted from lawz_model_evo_02.tbm using convert.lua
engageplayerpos {1.00, 0.00, -2.17}
gravity {0.00, 0.00, -12.00}
body "r_biceps"
	shape "capsule"
	position {0.68, 1.02, 5.27}
	orientation {0.70710678118655, 0, -0.70710678118655, 0}
	radius {0.12}
	length {0.11}
	density {0.25}
body "r_triceps"
	shape "capsule"
	position {0.31, 1.03, 5.34}
	orientation {0.5, 0.5, -0.5, -0.5}
	radius {0.08}
	length {0.20}
	density {0.25}
body "r_hand"
	shape "box"
	position {-0.20, 1.01, 5.56}
	orientation {0.70710678118655, -0.70710678118655, 0, 0}
	sides {0.17, 0.13, 0.16}
	density {0.25}
body "l_butt"
	shape "sphere"
	position {1.13, 1.02, 4.73}
	radius {0.15}
	density {0.2750}
body "breast"
	shape "capsule"
	position {1.02, 1.06, 5.56}
	orientation {0.51503605202327, -0.48449753881345, 0.51503605202327, -0.48449753881345}
	radius {0.19}
	length {0.15}
	density {0.1000}
body "l_thigh"
	shape "capsule"
	position {1.19, 0.97, 4.35}
	orientation {0.99965732497556, 0, 0.026176948307873, 0}
	radius {0.18}
	length {0.25}
	density {0.25}
body "chest"
	shape "capsule"
	position {1.15, 0.99, 5.15}
	orientation {0.70710678118655, 0, 0.70710678118655, 0}
	radius {0.21}
	length {0.12}
	density {0.0750}
body "stomach"
	shape "capsule"
	position {0.84, 0.99, 5.03}
	orientation {0.70710678118655, 0, -0.70710678118655, 0}
	radius {0.17}
	length {0.10}
	density {0.0750}
body "groin"
	shape "capsule"
	position {1.00, 1.00, 4.81}
	radius {0.20}
	length {0.10}
	density {0.25}
body "r_pecs"
	shape "sphere"
	position {0.90, 1.00, 5.42}
	radius {0.23}
	density {0.25}
body "r_foot"
	shape "box"
	position {0.77, 0.95, 3.36}
	sides {0.21, 0.38, 0.10}
	density {0.25}
body "l_pecs"
	shape "sphere"
	position {1.10, 1.00, 5.42}
	radius {0.23}
	density {0.25}
body "r_butt"
	shape "sphere"
	position {0.87, 1.02, 4.73}
	radius {0.15}
	density {0.2750}
body "l_biceps"
	shape "capsule"
	position {1.32, 1.02, 5.27}
	orientation {0.70710678118655, 0, 0.70710678118655, 0}
	radius {0.12}
	length {0.11}
	density {0.25}
body "r_thigh"
	shape "capsule"
	position {0.81, 0.97, 4.35}
	orientation {0.99965732497556, 0, -0.026176948307873, 0}
	radius {0.18}
	length {0.25}
	density {0.25}
body "l_foot"
	shape "box"
	position {1.23, 0.95, 3.36}
	sides {0.21, 0.38, 0.10}
	density {0.25}
body "l_triceps"
	shape "capsule"
	position {1.69, 1.03, 5.34}
	orientation {0.5, 0.5, 0.5, 0.5}
	radius {0.08}
	length {0.20}
	density {0.25}
body "r_leg"
	shape "capsule"
	position {0.77, 1.05, 3.75}
	orientation {0.99957168190737, -0.026174705665207, -0.013085110093862, 0.00034264566664205}
	radius {0.11}
	length {0.32}
	density {0.25}
body "head"
	shape "sphere"
	position {1.00, 0.92, 5.87}
	radius {0.20}
	density {0.2000}
body "l_hand"
	shape "box"
	position {2.20, 1.01, 5.56}
	orientation {0.70710678118655, -0.70710678118655, 0, 0}
	sides {0.17, 0.13, 0.16}
	density {0.25}
body "l_leg"
	shape "capsule"
	position {1.23, 1.05, 3.75}
	orientation {0.99957168190737, -0.026174705665207, 0.013085110093862, -0.00034264566664205}
	radius {0.11}
	length {0.32}
	density {0.25}
joint "neck"
	shape "sphere"
	radius {0.12}
	position {1.00, 0.98, 5.66}
	density {0.0025}
	strength {500}
	velocity {7.50}
	axis {-1, -0, -0}
	range {0.60, -0.60}
	connections {"head", "breast"}
	connectionType "hinge"
joint "l_ankle"
	shape "sphere"
	radius {0.08}
	position {1.23, 1.04, 3.46}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, -0}
	range {1.20, -0.40}
	connections {"l_leg", "l_foot"}
	connectionType "hinge"
joint "l_elbow"
	shape "sphere"
	radius {0.10}
	position {1.70, 1.02, 5.54}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {-2, 1.5, -4.5}
	range {3.00, 0.00}
	connections {"l_biceps", "l_triceps"}
	connectionType "hinge"
joint "r_hip"
	shape "sphere"
	radius {0.18}
	position {0.85, 0.98, 4.69}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, -0.2}
	range {0.40, -1.70}
	connections {"r_butt", "r_thigh"}
	connectionType "hinge"
joint "l_wrist"
	shape "sphere"
	radius {0.06}
	position {2.07, 1.03, 5.56}
	density {0.0025}
	strength {1000}
	velocity {9.00}
	axis {0.15, 1, 0.15}
	range {1.50, -1.50}
	connections {"l_triceps", "l_hand"}
	connectionType "hinge"
joint "r_ankle"
	shape "sphere"
	radius {0.08}
	position {0.77, 1.04, 3.46}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, -0}
	range {1.20, -0.40}
	connections {"r_leg", "r_foot"}
	connectionType "hinge"
joint "chest"
	shape "sphere"
	radius {0.16}
	position {1.00, 0.99, 5.56}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {-0, -0, -1}
	range {0.90, -0.90}
	connections {"breast", "chest"}
	connectionType "hinge"
joint "r_shoulder"
	shape "sphere"
	radius {0.16}
	position {0.71, 1.02, 5.50}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {0.1, -2.5, 0.1}
	range {0.50, -1.70}
	connections {"r_pecs", "r_biceps"}
	connectionType "hinge"
joint "l_glute"
	shape "sphere"
	radius {0.18}
	position {1.13, 1.02, 4.73}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-0.2, -1.6, 1}
	range {1.00, -0.25}
	connections {"groin", "l_butt"}
	connectionType "hinge"
joint "r_pecs"
	shape "sphere"
	radius {0.18}
	position {0.90, 1.00, 5.42}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {15, 0.5, -15}
	range {0.9, -2.2}
	connections {"breast", "r_pecs"}
	connectionType "hinge"
joint "r_elbow"
	shape "sphere"
	radius {0.10}
	position {0.30, 1.02, 5.54}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {2, 1.5, -4.5}
	range {-0, -3}
	connections {"r_biceps", "r_triceps"}
	connectionType "hinge"
joint "l_pecs"
	shape "sphere"
	radius {0.18}
	position {1.10, 1.00, 5.42}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {-15, -0.5, -15}
	range {2.20, -0.90}
	connections {"breast", "l_pecs"}
	connectionType "hinge"
joint "abs"
	shape "sphere"
	radius {0.19}
	position {1.00, 0.99, 5.03}
	density {0.0025}
	strength {2000}
	velocity {4.00}
	axis {-1, -0, -0}
	range {0.40, -0.40}
	connections {"stomach", "groin"}
	connectionType "hinge"
joint "l_hip"
	shape "sphere"
	radius {0.18}
	position {1.15, 0.98, 4.69}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, 0.2}
	range {0.40, -1.70}
	connections {"l_butt", "l_thigh"}
	connectionType "hinge"
joint "r_knee"
	shape "sphere"
	radius {0.13}
	position {0.80, 0.99, 4.05}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, 0.1}
	range {2.30, 0.00}
	connections {"r_thigh", "r_leg"}
	connectionType "hinge"
joint "r_glute"
	shape "sphere"
	radius {0.18}
	position {0.87, 1.02, 4.73}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {0.2, -1.6, 1}
	range {1.00, -0.25}
	connections {"groin", "r_butt"}
	connectionType "hinge"
joint "lumbar"
	shape "sphere"
	radius {0.20}
	position {1.00, 0.99, 5.15}
	density {0.0025}
	strength {1800}
	velocity {4.00}
	axis {-0, -1, -0}
	range {0.25, -0.25}
	connections {"chest", "stomach"}
	connectionType "hinge"
joint "l_knee"
	shape "sphere"
	radius {0.13}
	position {1.20, 0.99, 4.05}
	density {0.0025}
	strength {1500}
	velocity {9.00}
	axis {-1, -0, -0.1}
	range {2.30, 0.00}
	connections {"l_thigh", "l_leg"}
	connectionType "hinge"
joint "l_shoulder"
	shape "sphere"
	radius {0.16}
	position {1.29, 1.02, 5.50}
	density {0.0025}
	strength {1000}
	velocity {10.00}
	axis {0.1, -2.5, 0.1}
	range {0.50, -1.70}
	connections {"l_pecs", "l_biceps"}
	connectionType "hinge"
joint "r_wrist"
	shape "sphere"
	radius {0.06}
	position {-0.07, 1.03, 5.56}
	density {0.0025}
	strength {1000}
	velocity {9.00}
	axis {-0.15, 1, 0.15}
	range {1.5, -1.5}
	connections {"r_triceps", "r_hand"}
	connectionType "hinge"
object "10"
	shape "box"
	position {4.00, -5.10, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {3.00, 1.15, 0.12}
	density {0.25}
	static()
object "11"
	shape "box"
	position {4.56, -0.10, 1.04}
	orientation {1, 0, 0, 0}
	sides {0.04, 5.05, 0.03}
	density {0.25}
object "12"
	shape "box"
	position {5.40, -0.10, 1.00}
	orientation {1, 0, 0, 0}
	sides {1.10, 4.50, 0.20}
	density {0.25}
	static()
object "13"
	shape "box"
	position {-2.56, -0.10, 1.04}
	orientation {1, 0, 0, 0}
	sides {0.04, 5.05, 0.03}
	density {0.25}
object "15"
	shape "box"
	position {1.00, -0.10, 1.01}
	orientation {1, 0, 0, 0}
	sides {7.30, 12.90, 0.08}
	density {0.25}
	static()
object "19"
	shape "box"
	position {5.50, -3.60, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {3.00, 2.90, 0.20}
	density {0.25}
	static()
object "21"
	shape "box"
	position {1.00, -4.40, 1.00}
	orientation {1, 0, 0, 0}
	sides {4.74, 1.52, 0.12}
	density {0.25}
	static()
object "23"
	shape "box"
	position {-2.00, -5.10, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {3.00, 1.15, 0.12}
	density {0.25}
	static()
object "25"
	shape "box"
	position {7.00, -0.10, 1.00}
	orientation {1, 0, 0, 0}
	sides {1.00, 4.65, 0.20}
	density {0.25}
	static()
object "26"
	shape "box"
	position {0.50, 0.15, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.85, 0.10}
	density {0.25}
	static()
object "29"
	shape "box"
	position {0.00, -0.17, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.51, 0.10}
	density {0.25}
	static()
object "43"
	shape "box"
	position {3.00, 0.15, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.85, 0.10}
	density {0.25}
	static()
object "34"
	shape "box"
	position {7.50, -2.60, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {1.61, 1.15, 0.20}
	density {0.25}
	static()
object "44"
	shape "box"
	position {2.50, 0.15, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.85, 0.10}
	density {0.25}
	static()
object "45"
	shape "box"
	position {2.00, 0.15, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.85, 0.10}
	density {0.25}
	static()
object "46"
	shape "box"
	position {1.50, 0.15, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.85, 0.10}
	density {0.25}
	static()
object "48"
	shape "box"
	position {-0.50, -0.10, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.35, 0.10}
	density {0.25}
	static()
object "39"
	shape "box"
	position {-5.50, -2.60, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {1.61, 1.15, 0.20}
	density {0.25}
	static()
object "1"
	shape "box"
	position {6.50, -0.10, 1.04}
	orientation {1, 0, 0, 0}
	sides {3.90, 5.32, 0.01}
	density {0.25}
	static()
object "40"
	shape "box"
	position {-5.50, 2.40, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {1.61, 1.15, 0.20}
	density {0.25}
	static()
object "52"
	shape "box"
	position {2.50, -0.60, 1.01}
	orientation {1, 0, 0, 0}
	sides {1.35, 0.35, 0.10}
	density {0.25}
	static()
object "2"
	shape "box"
	position {-4.50, -0.10, 1.04}
	orientation {1, 0, 0, 0}
	sides {3.90, 5.32, 0.01}
	density {0.25}
	static()
object "3"
	shape "box"
	position {-2.00, 4.90, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {3.00, 1.15, 0.12}
	density {0.25}
	static()
object "55"
	shape "box"
	position {-0.50, 0.40, 1.01}
	orientation {1, 0, 0, 0}
	sides {1.35, 0.35, 0.10}
	density {0.25}
	static()
object "4"
	shape "box"
	position {1.00, 4.20, 1.00}
	orientation {1, 0, 0, 0}
	sides {4.74, 1.52, 0.12}
	density {0.25}
	static()
object "5"
	shape "box"
	position {-3.50, 3.40, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {3.00, 2.90, 0.20}
	density {0.25}
	static()
object "54"
	shape "box"
	position {1.00, -0.10, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.40, 0.35, 0.10}
	density {0.25}
	static()
object "53"
	shape "box"
	position {-0.50, -0.60, 1.01}
	orientation {1, 0, 0, 0}
	sides {1.35, 0.35, 0.10}
	density {0.25}
	static()
object "6"
	shape "box"
	position {-3.50, -3.60, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {3.00, 2.90, 0.20}
	density {0.25}
	static()
object "51"
	shape "box"
	position {1.00, -0.60, 1.01}
	orientation {1, 0, 0, 0}
	sides {1.35, 0.35, 0.10}
	density {0.25}
	static()
object "9"
	shape "box"
	position {5.50, 3.40, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {3.00, 2.90, 0.20}
	density {0.25}
	static()
object "7"
	shape "box"
	position {-3.40, -0.10, 1.00}
	orientation {1, 0, 0, 0}
	sides {1.10, 4.50, 0.20}
	density {0.25}
	static()
object "35"
	shape "box"
	position {7.50, 2.40, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {1.61, 1.15, 0.20}
	density {0.25}
	static()
object "38"
	shape "box"
	position {-5.00, -0.10, 1.00}
	orientation {1, 0, 0, 0}
	sides {1.00, 4.65, 0.20}
	density {0.25}
	static()
object "8"
	shape "box"
	position {4.00, 4.90, 1.00}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {3.00, 1.15, 0.12}
	density {0.25}
	static()
object "36"
	shape "box"
	position {-1.00, -0.03, 1.01}
	orientation {1, 0, 0, 0}
	sides {0.35, 0.51, 0.10}
	density {0.25}
	static()
object "24"
	shape "box"
	position {4.00, -5.10, 1.00}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {3.00, 1.15, 0.12}
	density {0.25}
	static()
