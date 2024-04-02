gravity { 0.00, 0.00, -27.00 }

globalplane()

object "dynamic_box"
	shape "box"
	sides { 0.50, 0.50, 0.50 }
	density { 0.25 }
	position { 2.00, 0.00, 2.00 }

object "dynamic_sphere"
	shape "sphere"
	radius { 0.25 }
	density { 0.25 }
	position { 1.00, 0.00, 2.00 }

object "dynamic_capsule"
	shape "capsule"
	radius { 0.25 }
	length { 1.00 }
	density { 0.25 }
	position { -1.00, 0.00, 2.00 }

object "dynamic_cylinder"
	shape "cylinder"
	radius { 0.25 }
	length { 1.00 }
	density { 0.25 }
	position { -2.00, 0.00, 2.00 }

body "head"
	shape "sphere"
	radius { 0.20 }
	density { 0.25 }
	position { 0.00, 0.92, 2.91 }

body "breast"
	shape "sphere"
	radius { 0.21 }
	density { 0.25 }
	position { 0.00, 1.06, 2.52 }

body "chest"
	shape "sphere"
	radius { 0.21 }
	density { 0.25 }
	position { 0.00, 1.06, 2.32 }

body "stomach"
	shape "sphere"
	radius { 0.20 }
	density { 0.25 }
	position { 0.00, 1.00, 2.10 }

body "groin"
	shape "capsule"
	radius { 0.20 }
	length { 0.10 }
	density { 0.25 }
	position { 0.00, 1.00, 1.87 }

body "l butt"
	shape "sphere"
	radius { 0.15 }
	density { 0.25 }
	position { 0.14, 1.02, 1.75 }

body "l thigh"
	shape "capsule"
	radius { 0.16 }
	length { 0.21 }
	density { 0.25 }
	position { 0.20, 0.97, 1.40 }

body "l leg"
	shape "capsule"
	radius { 0.12 }
	length { 0.25 }
	density { 0.25 }
	position { 0.20, 1.04, 0.75 }

body "l foot"
	shape "box"
	sides { 0.18, 0.30, 0.09 }
	density { 0.25 }
	position { 0.18, 0.97, 0.33 }
	
body "r butt"
	shape "sphere"
	radius { 0.15 }
	density { 0.25 }
	position { -0.14, 1.02, 1.75 }

body "r thigh"
	shape "capsule"
	radius { 0.16 }
	length { 0.21 }
	density { 0.25 }
	position { -0.20, 0.97, 1.40 }

body "r leg"
	shape "capsule"
	radius { 0.12 }
	length { 0.25 }
	density { 0.25 }
	position { -0.20, 1.04, 0.75 }

body "r foot"
	shape "box"
	sides { 0.18, 0.30, 0.09 }
	density { 0.25 }
	position { -0.18, 0.97, 0.33 }

body "l pecs"
	shape "sphere"
	radius { 0.24 }
	density { 0.25 }
	position { 0.12, 1.00, 2.47 }

body "l biceps"
	shape "capsule"
	radius { 0.13 }
	length { 0.05 }
	density { 0.25 }
	position { 0.57, 1.02, 2.53 }
	orientation { 0.7071068, 0.00, 0.7071068, 0.00 }

body "l triceps"
	shape "capsule"
	radius { 0.09 }
	length { 0.13 }
	density { 0.25 }
	position { 0.93, 1.02, 2.53 }
	orientation { 0.7071068, 0.00, 0.7071068, 0.00 }

body "l hand"
	shape "box"
	sides { 0.15, 0.13, 0.14 }
	density { 0.25 }
	position { 1.27, 1.02, 2.52 }

body "r pecs"
	shape "sphere"
	radius { 0.24 }
	density { 0.25 }
	position { -0.12, 1.00, 2.47 }

body "r biceps"
	shape "capsule"
	radius { 0.13 }
	length { 0.05 }
	density { 0.25 }
	position { -0.57, 1.02, 2.53 }
	orientation { 0.7071068, 0.00, -0.7071068, 0.00 }

body "r triceps"
	shape "capsule"
	radius { 0.09 }
	length { 0.13 }
	density { 0.25 }
	position { -0.93, 1.02, 2.53 }
	orientation { 0.7071068, 0.00, -0.7071068, 0.00 }

body "r hand"
	shape "box"
	sides { 0.15, 0.13, 0.14 }
	density { 0.25 }
	position { -1.27, 1.02, 2.52 }

joint "neck"
	shape "sphere"
	radius { 0.14 }
	density { 0.25 }
	position { 0.00, 0.99, 2.68 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.80, 0.80 }
	connections { "head", "breast" }
	connectionType "hinge"

joint "chest"
	shape "sphere"
	radius { 0.22 }
	density { 0.25 }
	position { 0.00, 1.00, 2.38 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { -1.00, 1.00 }
	connections { "breast", "chest" }
	connectionType "hinge"

joint "lumbar"
	shape "sphere"
	radius { 0.18 }
	density { 0.25 }
	position { 0.00, 0.96, 2.11 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 1.00, 0.00 }
	range { -0.40, 0.40 }
	connections { "chest", "stomach" }
	connectionType "hinge"

joint "abs"
	shape "sphere"
	radius { 0.16 }
	density { 0.25 }
	position { 0.00, 0.95, 1.95 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.60, 0.40 }
	connections { "stomach", "groin" }
	connectionType "hinge"

joint "l glute"
	shape "sphere"
	radius { 0.18 }
	density { 0.25 }
	position { 0.14, 1.02, 1.75 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 1.00, 0.00 }
	range { -1.35, 1.35 }
	connections { "groin", "l butt" }
	connectionType "hinge"

joint "l hip"
	shape "sphere"
	radius { 0.18 }
	density { 0.25 }
	position { 0.17, 0.98, 1.72 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.40, 2.30 }
	connections { "l butt", "l thigh" }
	connectionType "hinge"

joint "l knee"
	shape "sphere"
	radius { 0.13 }
	density { 0.25 }
	position { 0.20, 0.99, 1.07 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { 0.00, 2.30 }
	connections { "l thigh", "l leg" }
	connectionType "hinge"

joint "l ankle"
	shape "sphere"
	radius { 0.10 }
	density { 0.25 }
	position { 0.19, 1.04, 0.45 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.60, 1.40 }
	connections { "l leg", "l foot" }
	connectionType "hinge"

joint "r glute"
	shape "sphere"
	radius { 0.18 }
	density { 0.25 }
	position { -0.14, 1.02, 1.75 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 1.00, 0.00 }
	range { -1.35, 1.35 }
	connections { "groin", "r butt" }
	connectionType "hinge"

joint "r hip"
	shape "sphere"
	radius { 0.18 }
	density { 0.25 }
	position { -0.17, 0.98, 1.72 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.40, 2.30 }
	connections { "r butt", "r thigh" }
	connectionType "hinge"

joint "r knee"
	shape "sphere"
	radius { 0.13 }
	density { 0.25 }
	position { -0.20, 0.99, 1.07 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { 0.00, 2.30 }
	connections { "r thigh", "r leg" }
	connectionType "hinge"

joint "r ankle"
	shape "sphere"
	radius { 0.10 }
	density { 0.25 }
	position { -0.19, 1.04, 0.45 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 1.00, 0.00, 0.00 }
	range { -0.60, 1.40 }
	connections { "r leg", "r foot" }
	connectionType "hinge"

joint "l pecs"
	shape "sphere"
	radius { 0.22 }
	density { 0.25 }
	position { 0.10, 1.00, 2.45 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { -0.45, 1.75 }
	connections { "breast", "l pecs" }
	connectionType "hinge"

joint "l shoulder"
	shape "sphere"
	radius { 0.16 }
	density { 0.25 }
	position { 0.36, 1.02, 2.53 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 1.00, 0.00 }
	range { -2.00, 1.50 }
	connections { "l pecs", "l biceps" }
	connectionType "hinge"

joint "l elbow"
	shape "sphere"
	radius { 0.11 }
	density { 0.25 }
	position { 0.73, 1.02, 2.53 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { 0.00, 2.50 }
	connections { "l biceps", "l triceps" }
	connectionType "hinge"

joint "l wrist"
	shape "sphere"
	radius { 0.08 }
	density { 0.25 }
	position { 1.13, 1.02, 2.52 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { -1.50, 1.50 }
	connections { "l triceps", "l hand" }
	connectionType "hinge"

joint "r pecs"
	shape "sphere"
	radius { 0.22 }
	density { 0.25 }
	position { -0.10, 1.00, 2.45 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { -0.45, 1.75 }
	connections { "breast", "r pecs" }
	connectionType "hinge"

joint "r shoulder"
	shape "sphere"
	radius { 0.16 }
	density { 0.25 }
	position { -0.36, 1.02, 2.53 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 1.00, 0.00 }
	range { -2.00, 1.50 }
	connections { "r pecs", "r biceps" }
	connectionType "hinge"

joint "r elbow"
	shape "sphere"
	radius { 0.11 }
	density { 0.25 }
	position { -0.73, 1.02, 2.53 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { 0.00, 2.50 }
	connections { "r biceps", "r triceps" }
	connectionType "hinge"

joint "r wrist"
	shape "sphere"
	radius { 0.08 }
	density { 0.25 }
	position { -1.13, 1.02, 2.52 }
	strength { 220.00 }
	velocity { 650.00 }
	axis { 0.00, 0.00, 1.00 }
	range { -1.50, 1.50 }
	connections { "r triceps", "r hand" }
	connectionType "hinge"
