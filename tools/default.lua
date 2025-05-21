local default = {env_obj = {}, env_obj_joint = {}, body = {}, joint = {}}

default["body"]["head"] = {}
default["body"]["head"]["shape"] = "sphere"
default["body"]["head"]["pos"] = "0.00 0.85 2.67"
default["body"]["head"]["sides"] = "0.20 0.00 0.00"
default["body"]["head"]["radius"] = "0.20"
default["body"]["head"]["density"] = "0.25"

default["body"]["breast"] = {}
default["body"]["breast"]["shape"] = "box"
default["body"]["breast"]["pos"] = "0.00 0.90 2.20"
default["body"]["breast"]["sides"] = "0.20 0.10 0.30"
default["body"]["breast"]["density"] = "0.25"

default["body"]["chest"] = {}
default["body"]["chest"]["shape"] = "box"
default["body"]["chest"]["pos"] = "0.00 0.90 1.95"
default["body"]["chest"]["sides"] = "0.50 0.15 0.20"
default["body"]["chest"]["density"] = "0.25"

default["body"]["stomach"] = {}
default["body"]["stomach"]["shape"] = "box"
default["body"]["stomach"]["pos"] = "0.00 0.95 1.75"
default["body"]["stomach"]["sides"] = "0.35 0.15 0.20"
default["body"]["stomach"]["density"] = "0.25"

default["body"]["groin"] = {}
default["body"]["groin"]["shape"] = "box"
default["body"]["groin"]["pos"] = "0.00 1.00 1.55"
default["body"]["groin"]["sides"] = "0.20 0.15 0.20"
default["body"]["groin"]["density"] = "0.25"

default["body"]["l_pecs"] = {}
default["body"]["l_pecs"]["shape"] = "box"
default["body"]["l_pecs"]["pos"] = "0.25 0.90 2.15"
default["body"]["l_pecs"]["sides"] = "0.20 0.15 0.20"
default["body"]["l_pecs"]["density"] = "0.25"

default["body"]["l_biceps"] = {}
default["body"]["l_biceps"]["shape"] = "box"
default["body"]["l_biceps"]["pos"] = "0.55 0.90 2.30"
default["body"]["l_biceps"]["sides"] = "0.40 0.20 0.20"
default["body"]["l_biceps"]["density"] = "0.25"

default["body"]["l_triceps"] = {}
default["body"]["l_triceps"]["shape"] = "box"
default["body"]["l_triceps"]["pos"] = "0.95 0.90 2.30"
default["body"]["l_triceps"]["sides"] = "0.40 0.10 0.10"
default["body"]["l_triceps"]["density"] = "0.25"

default["body"]["l_hand"] = {}
default["body"]["l_hand"]["shape"] = "box"
default["body"]["l_hand"]["pos"] = "1.35 0.85 2.30"
default["body"]["l_hand"]["sides"] = "0.22 0.22 0.22"
default["body"]["l_hand"]["density"] = "0.25"

default["body"]["r_pecs"] = {}
default["body"]["r_pecs"]["shape"] = "box"
default["body"]["r_pecs"]["pos"] = "-0.25 0.90 2.15"
default["body"]["r_pecs"]["sides"] = "0.20 0.15 0.20"
default["body"]["r_pecs"]["density"] = "0.25"

default["body"]["r_biceps"] = {}
default["body"]["r_biceps"]["shape"] = "box"
default["body"]["r_biceps"]["pos"] = "-0.55 0.90 2.30"
default["body"]["r_biceps"]["sides"] = "0.40 0.20 0.20"
default["body"]["r_biceps"]["density"] = "0.25"

default["body"]["r_triceps"] = {}
default["body"]["r_triceps"]["shape"] = "box"
default["body"]["r_triceps"]["pos"] = "-0.95 0.90 2.30"
default["body"]["r_triceps"]["sides"] = "0.40 0.10 0.10"
default["body"]["r_triceps"]["density"] = "0.25"

default["body"]["r_hand"] = {}
default["body"]["r_hand"]["shape"] = "box"
default["body"]["r_hand"]["pos"] = "-1.35 0.85 2.30"
default["body"]["r_hand"]["sides"] = "0.22 0.22 0.22"
default["body"]["r_hand"]["density"] = "0.25"

default["body"]["l_butt"] = {}
default["body"]["l_butt"]["shape"] = "box"
default["body"]["l_butt"]["pos"] = "0.20 1.00 1.45"
default["body"]["l_butt"]["sides"] = "0.10 0.10 0.10"
default["body"]["l_butt"]["density"] = "0.25"

default["body"]["l_thigh"] = {}
default["body"]["l_thigh"]["shape"] = "cylinder"
default["body"]["l_thigh"]["pos"] = "0.20 1.00 1.10"
default["body"]["l_thigh"]["sides"] = "0.12 0.30 0.20"
default["body"]["l_thigh"]["radius"] = "0.12"
default["body"]["l_thigh"]["length"] = "0.30"
default["body"]["l_thigh"]["density"] = "0.25"

default["body"]["l_leg"] = {}
default["body"]["l_leg"]["shape"] = "cylinder"
default["body"]["l_leg"]["pos"] = "0.20 1.00 0.50"
default["body"]["l_leg"]["sides"] = "0.13 0.30 0.00"
default["body"]["l_leg"]["radius"] = "0.13"
default["body"]["l_leg"]["length"] = "0.30"
default["body"]["l_leg"]["density"] = "0.25"

default["body"]["l_foot"] = {}
default["body"]["l_foot"]["shape"] = "box"
default["body"]["l_foot"]["pos"] = "0.20 0.90 0.10"
default["body"]["l_foot"]["sides"] = "0.20 0.50 0.08"
default["body"]["l_foot"]["density"] = "0.25"

default["body"]["r_butt"] = {}
default["body"]["r_butt"]["shape"] = "box"
default["body"]["r_butt"]["pos"] = "-0.20 1.00 1.45"
default["body"]["r_butt"]["sides"] = "0.10 0.10 0.10"
default["body"]["r_butt"]["density"] = "0.25"

default["body"]["r_thigh"] = {}
default["body"]["r_thigh"]["shape"] = "cylinder"
default["body"]["r_thigh"]["pos"] = "-0.20 1.00 1.10"
default["body"]["r_thigh"]["sides"] = "0.12 0.30 0.00"
default["body"]["r_thigh"]["radius"] = "0.12"
default["body"]["r_thigh"]["length"] = "0.30"
default["body"]["r_thigh"]["density"] = "0.25"

default["body"]["r_leg"] = {}
default["body"]["r_leg"]["shape"] = "cylinder"
default["body"]["r_leg"]["pos"] = "-0.20 1.00 0.50"
default["body"]["r_leg"]["sides"] = "0.13 0.30 0.00"
default["body"]["r_leg"]["radius"] = "0.13"
default["body"]["r_leg"]["length"] = "0.30"
default["body"]["r_leg"]["density"] = "0.25"

default["body"]["r_foot"] = {}
default["body"]["r_foot"]["shape"] = "box"
default["body"]["r_foot"]["pos"] = "-0.20 0.90 0.10"
default["body"]["r_foot"]["sides"] = "0.20 0.50 0.08"
default["body"]["r_foot"]["density"] = "0.25"

default["joint"]["neck"] = {}
default["joint"]["neck"]["radius"] = "0.11"
default["joint"]["neck"]["pos"] = "0.00 0.90 2.50"
default["joint"]["neck"]["strength"] = "1.00"
default["joint"]["neck"]["velocity"] = "50.00"
default["joint"]["neck"]["axis"] = "1.00 0.00 0.00"
default["joint"]["neck"]["range"] = "0.50 -0.50"
default["joint"]["neck"]["connections"] = "\"head\", \"breast\""

default["joint"]["chest"] = {}
default["joint"]["chest"]["radius"] = "0.18"
default["joint"]["chest"]["pos"] = "0.00 0.90 2.05"
default["joint"]["chest"]["strength"] = "4.00"
default["joint"]["chest"]["velocity"] = "20.00"
default["joint"]["chest"]["axis"] = "0.00 0.00 1.00"
default["joint"]["chest"]["range"] = "1.00 -1.00"
default["joint"]["chest"]["connections"] = "\"breast\", \"chest\""

default["joint"]["lumbar"] = {}
default["joint"]["lumbar"]["radius"] = "0.16"
default["joint"]["lumbar"]["pos"] = "0.00 0.95 1.85"
default["joint"]["lumbar"]["strength"] = "3.00"
default["joint"]["lumbar"]["velocity"] = "5.00"
default["joint"]["lumbar"]["axis"] = "0.00 1.00 0.00"
default["joint"]["lumbar"]["range"] = "0.50 -0.50"
default["joint"]["lumbar"]["connections"] = "\"chest\", \"stomach\""

default["joint"]["abs"] = {}
default["joint"]["abs"]["radius"] = "0.14"
default["joint"]["abs"]["pos"] = "0.00 1.00 1.65"
default["joint"]["abs"]["strength"] = "3.00"
default["joint"]["abs"]["velocity"] = "5.00"
default["joint"]["abs"]["axis"] = "1.00 0.00 0.00"
default["joint"]["abs"]["range"] = "0.40 -0.60"
default["joint"]["abs"]["connections"] = "\"stomach\", \"groin\""

default["joint"]["l_pecs"] = {}
default["joint"]["l_pecs"]["radius"] = "0.18"
default["joint"]["l_pecs"]["pos"] = "0.15, 0.85 2.20"
default["joint"]["l_pecs"]["strength"] = "3.00"
default["joint"]["l_pecs"]["velocity"] = "15.00"
default["joint"]["l_pecs"]["axis"] = "0.00 0.00 1.00"
default["joint"]["l_pecs"]["range"] = "1.50 -0.70"
default["joint"]["l_pecs"]["connections"] = "\"breast\", \"l_pecs\""

default["joint"]["l_shoulder"] = {}
default["joint"]["l_shoulder"]["radius"] = "0.18"
default["joint"]["l_shoulder"]["pos"] = "0.35 0.90 2.30"
default["joint"]["l_shoulder"]["strength"] = "4.00"
default["joint"]["l_shoulder"]["velocity"] = "20.00"
default["joint"]["l_shoulder"]["axis"] = "0.00 1.00 0.00"
default["joint"]["l_shoulder"]["range"] = "1.20 -1.20"
default["joint"]["l_shoulder"]["connections"] = "\"l_pecs\", \"l_biceps\""

default["joint"]["l_elbow"] = {}
default["joint"]["l_elbow"]["radius"] = "0.16"
default["joint"]["l_elbow"]["pos"] = "0.75 0.90 2.30"
default["joint"]["l_elbow"]["strength"] = "2.00"
default["joint"]["l_elbow"]["velocity"] = "40.00"
default["joint"]["l_elbow"]["axis"] = "0.00 0.00 1.00"
default["joint"]["l_elbow"]["range"] = "2.30 0.05"
default["joint"]["l_elbow"]["connections"] = "\"l_biceps\", \"l_triceps\""

default["joint"]["l_wrist"] = {}
default["joint"]["l_wrist"]["radius"] = "0.11"
default["joint"]["l_wrist"]["pos"] = "1.20 0.90 2.30"
default["joint"]["l_wrist"]["strength"] = "2.00"
default["joint"]["l_wrist"]["velocity"] = "60.00"
default["joint"]["l_wrist"]["axis"] = "0.00 0.00 1.00"
default["joint"]["l_wrist"]["range"] = "2.00 -0.30"
default["joint"]["l_wrist"]["connections"] = "\"l_triceps\", \"l_hand\""

default["joint"]["r_pecs"] = {}
default["joint"]["r_pecs"]["radius"] = "0.18"
default["joint"]["r_pecs"]["pos"] = "-0.15, 0.85 2.20"
default["joint"]["r_pecs"]["strength"] = "3.00"
default["joint"]["r_pecs"]["velocity"] = "15.00"
default["joint"]["r_pecs"]["axis"] = "0.00 0.00 1.00"
default["joint"]["r_pecs"]["range"] = "1.50 -0.70"
default["joint"]["r_pecs"]["connections"] = "\"breast\", \"r_pecs\""

default["joint"]["r_shoulder"] = {}
default["joint"]["r_shoulder"]["radius"] = "0.18"
default["joint"]["r_shoulder"]["pos"] = "0.35 0.90 2.30"
default["joint"]["r_shoulder"]["strength"] = "4.00"
default["joint"]["r_shoulder"]["velocity"] = "20.00"
default["joint"]["r_shoulder"]["axis"] = "0.00 1.00 0.00"
default["joint"]["r_shoulder"]["range"] = "1.20 -1.20"
default["joint"]["r_shoulder"]["connections"] = "\"r_pecs\", \"r_biceps\""

default["joint"]["r_elbow"] = {}
default["joint"]["r_elbow"]["radius"] = "0.16"
default["joint"]["r_elbow"]["pos"] = "0.75 0.90 2.30"
default["joint"]["r_elbow"]["strength"] = "2.00"
default["joint"]["r_elbow"]["velocity"] = "40.00"
default["joint"]["r_elbow"]["axis"] = "0.00 0.00 1.00"
default["joint"]["r_elbow"]["range"] = "2.30 0.05"
default["joint"]["r_elbow"]["connections"] = "\"r_biceps\", \"r_triceps\""

default["joint"]["r_wrist"] = {}
default["joint"]["r_wrist"]["radius"] = "0.11"
default["joint"]["r_wrist"]["pos"] = "1.20 0.90 2.30"
default["joint"]["r_wrist"]["strength"] = "2.00"
default["joint"]["r_wrist"]["velocity"] = "60.00"
default["joint"]["r_wrist"]["axis"] = "0.00 0.00 1.00"
default["joint"]["r_wrist"]["range"] = "2.00 -0.30"
default["joint"]["r_wrist"]["connections"] = "\"r_triceps\", \"r_hand\""

default["joint"]["l_glute"] = {}
default["joint"]["l_glute"]["radius"] = "0.16"
default["joint"]["l_glute"]["pos"] = "0.10 1.10 1.51"
default["joint"]["l_glute"]["strength"] = "4.00"
default["joint"]["l_glute"]["velocity"] = "10.00"
default["joint"]["l_glute"]["axis"] = "0.00 1.00 0.00"
default["joint"]["l_glute"]["range"] = "1.20 -0.20"
default["joint"]["l_glute"]["connections"] = "\"groin\", \"l_butt\""

default["joint"]["l_hip"] = {}
default["joint"]["l_hip"]["radius"] = "0.16"
default["joint"]["l_hip"]["pos"] = "0.21 1.00 1.40"
default["joint"]["l_hip"]["strength"] = "2.00"
default["joint"]["l_hip"]["velocity"] = "12.00"
default["joint"]["l_hip"]["axis"] = "1.00 0.00 0.00"
default["joint"]["l_hip"]["range"] = "1.00 -1.80"
default["joint"]["l_hip"]["connections"] = "\"l_butt\", \"l_thigh\""

default["joint"]["l_knee"] = {}
default["joint"]["l_knee"]["radius"] = "0.14"
default["joint"]["l_knee"]["pos"] = "0.20 1.00 0.80"
default["joint"]["l_knee"]["strength"] = "4.00"
default["joint"]["l_knee"]["velocity"] = "40.00"
default["joint"]["l_knee"]["axis"] = "1.00 0.00 0.00"
default["joint"]["l_knee"]["range"] = "2.30 0.00"
default["joint"]["l_knee"]["connections"] = "\"l_thigh\", \"l_leg\""

default["joint"]["l_ankle"] = {}
default["joint"]["l_ankle"]["radius"] = "0.11"
default["joint"]["l_ankle"]["pos"] = "0.20 1.05 0.25"
default["joint"]["l_ankle"]["strength"] = "1.00"
default["joint"]["l_ankle"]["velocity"] = "8.00"
default["joint"]["l_ankle"]["axis"] = "1.00 0.00 0.00"
default["joint"]["l_ankle"]["range"] = "1.10 -0.30"
default["joint"]["l_ankle"]["connections"] = "\"l_leg\", \"l_foot\""

default["joint"]["r_glute"] = {}
default["joint"]["r_glute"]["radius"] = "0.16"
default["joint"]["r_glute"]["pos"] = "-0.10 1.10 1.51"
default["joint"]["r_glute"]["strength"] = "4.00"
default["joint"]["r_glute"]["velocity"] = "10.00"
default["joint"]["r_glute"]["axis"] = "0.00 1.00 0.00"
default["joint"]["r_glute"]["range"] = "1.20 -0.20"
default["joint"]["r_glute"]["connections"] = "\"groin\", \"r_butt\""

default["joint"]["r_hip"] = {}
default["joint"]["r_hip"]["radius"] = "0.16"
default["joint"]["r_hip"]["pos"] = "-0.21 1.00 1.40"
default["joint"]["r_hip"]["strength"] = "2.00"
default["joint"]["r_hip"]["velocity"] = "12.00"
default["joint"]["r_hip"]["axis"] = "1.00 0.00 0.00"
default["joint"]["r_hip"]["range"] = "1.00 -1.80"
default["joint"]["r_hip"]["connections"] = "\"r_butt\", \"r_thigh\""

default["joint"]["r_knee"] = {}
default["joint"]["r_knee"]["radius"] = "0.14"
default["joint"]["r_knee"]["pos"] = "-0.20 1.00 0.80"
default["joint"]["r_knee"]["strength"] = "4.00"
default["joint"]["r_knee"]["velocity"] = "40.00"
default["joint"]["r_knee"]["axis"] = "1.00 0.00 0.00"
default["joint"]["r_knee"]["range"] = "2.30 0.00"
default["joint"]["r_knee"]["connections"] = "\"r_thigh\", \"r_leg\""

default["joint"]["r_ankle"] = {}
default["joint"]["r_ankle"]["radius"] = "0.11"
default["joint"]["r_ankle"]["pos"] = "-0.20 1.05 0.25"
default["joint"]["r_ankle"]["strength"] = "1.00"
default["joint"]["r_ankle"]["velocity"] = "8.00"
default["joint"]["r_ankle"]["axis"] = "1.00 0.00 0.00"
default["joint"]["r_ankle"]["range"] = "1.10 -0.30"
default["joint"]["r_ankle"]["connections"] = "\"r_leg\", \"r_foot\""

return default
