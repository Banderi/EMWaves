extends Node

#const gdns = preload("res://gdnative_cpp/wavescpp.gdns")
#onready var gdns = preload("res://gdnative_cpp/wavescpp.gdns")
#onready var gdns = preload("res://gdnative/waves.gdns")
var GDNATIVE = null

func init():
#	var gdns = load("res://gdnative_cpp/wavescpp.gdns")
	var gdns = load("res://gdnative/waves.gdns")

	# check if the loaded native script is valid
	print("GDNS >> Native Script: ", gdns)
	if gdns == null || !is_instance_valid(gdns):
		return

	# check if the loaded library is valid
	print("GDNS >> Library: ", gdns.library)
	if gdns.library == null || !is_instance_valid(gdns.library):
		return

	# check if the loaded library reference is valid
	GDNATIVE = gdns.new()
	print("GDNS >> Reference: ", GDNATIVE)
	if GDNATIVE == null || !is_instance_valid(GDNATIVE):
		return
	print("GDNS >> Reference name ", GDNATIVE.name)
	if GDNATIVE.name != "SimpleClass":
		return
	print("GDNS >> Reference value ", GDNATIVE.value)
	if GDNATIVE.value != 0:
		return

#    print("GDNS >> Call method ", ref.method(1))
#    if ref.method(1) != 1:
#        return
#    OS.exit_code = 0




#func load_electron_state(e_idx, e_t, e_p, e_v):
#	GDNATIVE.load_electron_state(e_idx, e_t, e_p, e_v)


# Called when the node enters the scene tree for the first time.
func _ready():
	init()
#	print(GDNATIVE.get_heartbeat("test"))
#	print(GDNATIVE.get_particle_test())
	pass # Replace with function body.
