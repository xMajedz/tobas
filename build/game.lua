-- converted from tma_ergo_v4.tbm using convert.lua
turnframes {10}
gravity {0.00, 0.00, -27.50}
friction {1000.00}
engageplayerpos {0.00, 2.25, 1.77}
body "r_hand"
	shape "box"
	position {0.05, 0.00, 2.28}
	sides {0.40, 0.10, 0.12}
	density {0.1444}
body "l_butt"
	shape "box"
	position {1.19, 0.00, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
body "head"
	shape "sphere"
	position {1.00, -0.10, 2.65}
	radius {0.20}
	density {0.0676}
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
body "l_leg"
	shape "capsule"
	position {1.20, 0.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "groin"
	shape "box"
	position {1.00, 0.00, 1.53}
	sides {0.18, 0.15, 0.15}
	density {0.4212}
body "r_pecs"
	shape "capsule"
	position {0.77, -0.02, 2.20}
	orientation {0.97258090606102, 0.085089803642111, 0.21561599586216, 0.018863955312792}
	radius {0.10}
	length {0.15}
	density {0.4348}
body "l_pecs"
	shape "capsule"
	position {1.23, -0.02, 2.20}
	orientation {0.97258090606102, 0.085089803642111, -0.21561599586216, -0.018863955312792}
	radius {0.10}
	length {0.15}
	density {0.4348}
body "r_butt"
	shape "box"
	position {0.81, 0.00, 1.45}
	sides {0.10, 0.10, 0.10}
	density {2.8124}
body "l_biceps"
	shape "box"
	position {1.37, 0.02, 2.30}
	sides {0.06, 0.02, 0.02}
	density {0.0004}
body "r_thigh"
	shape "capsule"
	position {0.80, 0.00, 1.10}
	radius {0.14}
	length {0.30}
	density {0.3412}
body "l_triceps"
	shape "box"
	position {1.50, 0.00, 2.29}
	sides {0.40, 0.16, 0.16}
	density {0.1444}
body "r_leg"
	shape "capsule"
	position {0.80, 0.00, 0.50}
	radius {0.11}
	length {0.30}
	density {0.2512}
body "l_hand"
	shape "box"
	position {1.96, 0.00, 2.28}
	sides {0.40, 0.10, 0.12}
	density {0.1444}
body "r_foot"
	shape "box"
	position {0.80, -0.10, 0.10}
	sides {0.21, 0.45, 0.08}
	density {0.1096}
body "l_foot"
	shape "box"
	position {1.20, -0.10, 0.10}
	sides {0.21, 0.45, 0.08}
	density {0.1096}
body "r_biceps"
	shape "box"
	position {0.63, 0.02, 2.29}
	sides {0.06, 0.02, 0.02}
	density {0.0004}
body "breast"
	shape "capsule"
	position {1.00, -0.02, 2.20}
	orientation {0.99904822158186, -0.043619387365336, 0, 0}
	radius {0.10}
	length {0.25}
	density {0.5748}
body "r_triceps"
	shape "box"
	position {0.50, 0.00, 2.29}
	sides {0.40, 0.16, 0.16}
	density {0.1444}
body "stomach"
	shape "box"
	position {1.00, -0.03, 1.74}
	sides {0.37, 0.12, 0.20}
	density {0.3316}
joint "r_hip"
	shape "sphere"
	radius {0.16}
	position {0.81, 0.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {-1, -0.08, -0.08}
	range {1.10, -2.30}
	connections {"r_butt", "r_thigh"}
	connectionType "hinge"
joint "r_knee"
	shape "sphere"
	radius {0.13}
	position {0.80, 0.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {-1, -0, -0}
	range {2.30, 0.00}
	connections {"r_thigh", "r_leg"}
	connectionType "hinge"
joint "r_shoulder"
	shape "sphere"
	radius {0.14}
	position {0.70, 0.00, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.50}
	axis {-0, -1, -0}
	range {1.30, -1.50}
	connections {"r_pecs", "r_biceps"}
	connectionType "hinge"
joint "chest"
	shape "sphere"
	radius {0.16}
	position {1.00, -0.05, 2.07}
	density {0.0025}
	strength {2600}
	velocity {7.15}
	axis {-0, -0, -1}
	range {1.00, -1.00}
	connections {"breast", "chest"}
	connectionType "hinge"
joint "lumbar"
	shape "sphere"
	radius {0.14}
	position {1.00, -0.03, 1.85}
	density {0.0025}
	strength {4000}
	velocity {3.07}
	axis {-0, -1, -0}
	range {0.55, -0.55}
	connections {"chest", "stomach"}
	connectionType "hinge"
joint "l_glute"
	shape "sphere"
	radius {0.15}
	position {1.10, 0.07, 1.51}
	density {0.0025}
	strength {1600}
	velocity {5.62}
	axis {-0, -1, 0.25}
	range {1.00, -0.80}
	connections {"groin", "l_butt"}
	connectionType "hinge"
joint "r_pecs"
	shape "sphere"
	radius {0.17}
	position {0.87, -0.06, 2.22}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {0.4, -0, -1}
	range {0.7, -1.5}
	connections {"breast", "r_pecs"}
	connectionType "hinge"
joint "l_hip"
	shape "sphere"
	radius {0.16}
	position {1.19, 0.00, 1.40}
	density {0.0025}
	strength {2300}
	velocity {7.60}
	axis {-1, 0.08, 0.08}
	range {1.10, -2.30}
	connections {"l_butt", "l_thigh"}
	connectionType "hinge"
joint "l_pecs"
	shape "sphere"
	radius {0.17}
	position {1.13, -0.06, 2.22}
	density {0.0025}
	strength {670}
	velocity {11.50}
	axis {-0.4, -0, -1}
	range {1.50, -0.70}
	connections {"breast", "l_pecs"}
	connectionType "hinge"
joint "l_knee"
	shape "sphere"
	radius {0.13}
	position {1.20, 0.00, 0.80}
	density {0.0025}
	strength {1900}
	velocity {19.30}
	axis {-1, -0, -0}
	range {2.30, 0.00}
	connections {"l_thigh", "l_leg"}
	connectionType "hinge"
joint "abs"
	shape "sphere"
	radius {0.14}
	position {1.00, 0.00, 1.64}
	density {0.0025}
	strength {4600}
	velocity {3.18}
	axis {-1, -0, -0}
	range {0.35, -0.70}
	connections {"stomach", "groin"}
	connectionType "hinge"
joint "r_ankle"
	shape "sphere"
	radius {0.11}
	position {0.80, 0.00, 0.25}
	density {0.0025}
	strength {1200}
	velocity {7.75}
	axis {-1, -0, -0}
	range {1.10, -0.30}
	connections {"r_leg", "r_foot"}
	connectionType "hinge"
joint "l_ankle"
	shape "sphere"
	radius {0.11}
	position {1.20, 0.00, 0.25}
	density {0.0025}
	strength {1200}
	velocity {7.75}
	axis {-1, -0, -0}
	range {1.10, -0.30}
	connections {"l_leg", "l_foot"}
	connectionType "hinge"
joint "l_elbow"
	shape "sphere"
	radius {0.11}
	position {1.37, 0.02, 2.29}
	density {0.0025}
	strength {770}
	velocity {10.00}
	axis {1, -0, -0}
	range {1.35, -1.00}
	connections {"l_biceps", "l_triceps"}
	connectionType "hinge"
joint "neck"
	shape "sphere"
	radius {0.11}
	position {1.00, -0.07, 2.50}
	density {0.0025}
	strength {220}
	velocity {6.50}
	axis {-1, -0, -0}
	range {0.50, -0.50}
	connections {"head", "breast"}
	connectionType "hinge"
joint "l_shoulder"
	shape "sphere"
	radius {0.14}
	position {1.30, 0.00, 2.30}
	density {0.0025}
	strength {660}
	velocity {10.50}
	axis {-0, -1, -0}
	range {1.30, -1.50}
	connections {"l_pecs", "l_biceps"}
	connectionType "hinge"
joint "r_wrist"
	shape "sphere"
	radius {0.11}
	position {0.30, 0.00, 2.29}
	density {0.0025}
	strength {500}
	velocity {18.50}
	axis {-0, -0, -1}
	range {-0.05, -2.3}
	connections {"r_triceps", "r_hand"}
	connectionType "hinge"
joint "r_glute"
	shape "sphere"
	radius {0.15}
	position {0.90, 0.07, 1.51}
	density {0.0025}
	strength {1600}
	velocity {5.62}
	axis {-0, -1, 0.25}
	range {1.00, -0.80}
	connections {"groin", "r_butt"}
	connectionType "hinge"
joint "r_elbow"
	shape "sphere"
	radius {0.11}
	position {0.63, 0.02, 2.29}
	density {0.0025}
	strength {770}
	velocity {10.00}
	axis {-1, -0, -0}
	range {1, -1.35}
	connections {"r_biceps", "r_triceps"}
	connectionType "hinge"
joint "l_wrist"
	shape "sphere"
	radius {0.11}
	position {1.70, 0.00, 2.29}
	density {0.0025}
	strength {500}
	velocity {18.50}
	axis {-0, -0, -1}
	range {2.30, 0.05}
	connections {"l_triceps", "l_hand"}
	connectionType "hinge"
object "51"
	shape "box"
	position {-4.00, -5.10, 2.65}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "6"
	shape "box"
	position {-0.43, 2.28, 4.03}
	orientation {1, 0, 0, 0}
	sides {0.13, 0.15, 0.08}
	density {0.25}
object "52"
	shape "capsule"
	position {1.00, -0.10, 0.00}
	orientation {0.65328148243819, 0.65328148243819, -0.2705980500731, -0.2705980500731}
	radius {0.02}
	length {3.30}
	density {0.25}
	static()
object "7"
	shape "box"
	position {2.26, 2.18, 3.95}
	orientation {0.88701083317822, 0, -0.46174861323503, 0}
	sides {0.10, 0.04, 0.05}
	density {0.25}
object "84"
	shape "capsule"
	position {3.55, -6.25, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "85"
	shape "box"
	position {3.50, -6.13, 2.65}
	orientation {0.98078528040323, 0, 0, -0.19509032201613}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "53"
	shape "box"
	position {-4.00, 4.90, 2.65}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "54"
	shape "box"
	position {6.25, -5.35, 0.10}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {5.00, 2.56, 3.50}
	density {0.25}
	static()
object "88"
	shape "box"
	position {-4.25, 5.15, 0.10}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {5.00, 2.56, 3.50}
	density {0.25}
	static()
object "55"
	shape "box"
	position {-6.06, -0.10, 2.65}
	orientation {0.70710678118655, 0, 0, 0.70710678118655}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "89"
	shape "box"
	position {-6.64, -0.10, 0.10}
	orientation {0.70710678118655, 0, 0, 0.70710678118655}
	sides {16.00, 3.00, 3.50}
	density {0.25}
	static()
object "56"
	shape "box"
	position {8.06, -0.10, 2.65}
	orientation {0.70710678118655, 0, 0, 0.70710678118655}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "90"
	shape "box"
	position {-4.25, -5.35, 0.10}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {5.00, 2.56, 3.50}
	density {0.25}
	static()
object "57"
	shape "box"
	position {1.00, 6.96, 2.65}
	orientation {1, 0, 0, 0}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "87"
	shape "box"
	position {6.25, 5.15, 0.10}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {5.00, 2.56, 3.50}
	density {0.25}
	static()
object "58"
	shape "box"
	position {1.00, -7.16, 2.64}
	orientation {1, 0, 0, 0}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "86"
	shape "box"
	position {1.00, -7.74, 0.10}
	orientation {1, 0, 0, 0}
	sides {18.28, 3.00, 3.50}
	density {0.25}
	static()
object "59"
	shape "capsule"
	position {-3.35, -4.45, 4.90}
	orientation {0.65328148243819, 0.65328148243819, 0.2705980500731, 0.2705980500731}
	radius {0.07}
	length {5.11}
	density {0.25}
	static()
object "65"
	shape "capsule"
	position {1.00, 6.06, 4.90}
	orientation {0.70710678118655, 0, 0.70710678118655, 0}
	radius {0.07}
	length {5.08}
	density {0.25}
	static()
object "83"
	shape "capsule"
	position {5.35, -4.45, 4.90}
	orientation {0.65328148243819, 0.65328148243819, -0.2705980500731, -0.2705980500731}
	radius {0.07}
	length {5.11}
	density {0.25}
	static()
object "82"
	shape "capsule"
	position {7.15, -2.65, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "81"
	shape "box"
	position {7.03, -2.60, 2.65}
	orientation {0.83146961230255, 0, 0, -0.5555702330196}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "60"
	shape "box"
	position {8.64, -0.10, 0.10}
	orientation {0.70710678118655, 0, 0, 0.70710678118655}
	sides {16.00, 3.00, 3.50}
	density {0.25}
	static()
object "66"
	shape "capsule"
	position {-5.15, 2.45, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "1"
	shape "sphere"
	position {2.15, 2.28, 3.99}
	orientation {1, 0, 0, 0}
	radius {0.07}
	density {0.25}
object "61"
	shape "box"
	position {1.00, 7.54, 0.10}
	orientation {1, 0, 0, 0}
	sides {18.28, 3.00, 3.50}
	density {0.25}
	static()
object "79"
	shape "box"
	position {-1.50, 5.93, 2.65}
	orientation {0.98078528040323, 0, 0, -0.19509032201613}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "78"
	shape "capsule"
	position {-3.35, 4.25, 4.90}
	orientation {0.65328148243819, 0.65328148243819, -0.2705980500731, -0.2705980500731}
	radius {0.07}
	length {5.11}
	density {0.25}
	static()
object "62"
	shape "box"
	position {0.69, -0.17, -0.73}
	orientation {0.70710678118655, 0, 0, 0.70710678118655}
	sides {13.00, 13.00, 5.00}
	density {0.25}
	static()
object "68"
	shape "box"
	position {3.50, 5.93, 2.65}
	orientation {0.98078528040323, 0, 0, 0.19509032201613}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "69"
	shape "box"
	position {7.03, 2.40, 2.65}
	orientation {0.83146961230255, 0, 0, 0.5555702330196}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "63"
	shape "box"
	position {-1.50, -6.13, 2.65}
	orientation {0.98078528040323, 0, 0, 0.19509032201613}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "70"
	shape "capsule"
	position {-1.55, -6.25, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "71"
	shape "capsule"
	position {-5.15, -2.65, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "8"
	shape "box"
	position {-0.26, 2.18, 3.95}
	orientation {0.88701083317822, 0, 0.46174861323503, 0}
	sides {0.10, 0.04, 0.05}
	density {0.25}
object "64"
	shape "capsule"
	position {-1.55, 6.05, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "10"
	shape "box"
	position {2.28, -2.48, 4.02}
	orientation {0.99619469809175, 0, 0.087155742747658, 0}
	sides {0.19, 0.16, 0.10}
	density {0.25}
object "9"
	shape "sphere"
	position {2.15, -2.48, 3.99}
	orientation {1, 0, 0, 0}
	radius {0.07}
	density {0.25}
object "73"
	shape "capsule"
	position {1.00, -6.26, 4.90}
	orientation {0.70710678118655, 0, 0.70710678118655, 0}
	radius {0.07}
	length {5.08}
	density {0.25}
	static()
object "75"
	shape "capsule"
	position {-5.16, -0.10, 4.90}
	orientation {0.70710678118655, 0.70710678118655, 0, 0}
	radius {0.07}
	length {5.08}
	density {0.25}
	static()
object "11"
	shape "box"
	position {2.43, -2.48, 4.03}
	orientation {1, 0, 0, 0}
	sides {0.13, 0.15, 0.08}
	density {0.25}
object "74"
	shape "capsule"
	position {3.55, 6.05, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "12"
	shape "box"
	position {2.26, -2.38, 3.93}
	orientation {0.88701083317822, 0, -0.46174861323503, 0}
	sides {0.10, 0.04, 0.05}
	density {0.25}
object "72"
	shape "capsule"
	position {7.15, 2.45, 2.65}
	orientation {1, 0, 0, 0}
	radius {0.09}
	length {4.47}
	density {0.25}
	static()
object "13"
	shape "sphere"
	position {-0.15, -2.48, 3.99}
	orientation {1, 0, 0, 0}
	radius {0.07}
	density {0.25}
object "76"
	shape "capsule"
	position {5.35, 4.25, 4.90}
	orientation {0.65328148243819, 0.65328148243819, 0.2705980500731, 0.2705980500731}
	radius {0.07}
	length {5.11}
	density {0.25}
	static()
object "14"
	shape "box"
	position {-0.28, -2.48, 4.02}
	orientation {0.99619469809175, 0, -0.087155742747658, 0}
	sides {0.19, 0.16, 0.10}
	density {0.25}
object "77"
	shape "box"
	position {-5.03, 2.40, 2.65}
	orientation {0.83146961230255, 0, 0, -0.5555702330196}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "15"
	shape "box"
	position {-0.43, -2.48, 4.03}
	orientation {1, 0, 0, 0}
	sides {0.13, 0.15, 0.08}
	density {0.25}
object "67"
	shape "box"
	position {-5.03, -2.60, 2.65}
	orientation {0.83146961230255, 0, 0, 0.5555702330196}
	sides {0.35, 0.10, 4.65}
	density {0.25}
	static()
object "16"
	shape "box"
	position {-0.26, -2.38, 3.95}
	orientation {0.88701083317822, 0, 0.46174861323503, 0}
	sides {0.10, 0.04, 0.05}
	density {0.25}
object "3"
	shape "box"
	position {-0.28, 2.28, 4.02}
	orientation {0.99619469809175, 0, -0.087155742747658, 0}
	sides {0.19, 0.16, 0.10}
	density {0.25}
object "49"
	shape "box"
	position {6.00, 4.90, 2.66}
	orientation {0.92387953251129, 0, 0, 0.38268343236509}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "2"
	shape "sphere"
	position {-0.15, 2.28, 3.99}
	orientation {1, 0, 0, 0}
	radius {0.07}
	density {0.25}
object "4"
	shape "box"
	position {2.28, 2.28, 4.02}
	orientation {0.99619469809175, 0, 0.087155742747658, 0}
	sides {0.19, 0.16, 0.10}
	density {0.25}
object "80"
	shape "capsule"
	position {7.16, -0.10, 4.90}
	orientation {0.70710678118655, 0.70710678118655, 0, 0}
	radius {0.07}
	length {5.08}
	density {0.25}
	static()
object "50"
	shape "box"
	position {6.00, -5.10, 2.65}
	orientation {0.92387953251129, 0, 0, -0.38268343236509}
	sides {5.00, 1.95, 4.65}
	density {0.25}
	static()
object "5"
	shape "box"
	position {2.43, 2.28, 4.03}
	orientation {1, 0, 0, 0}
	sides {0.13, 0.15, 0.08}
	density {0.25}
