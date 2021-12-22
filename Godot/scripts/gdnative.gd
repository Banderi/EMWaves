extends Node

const WAVES = preload("res://gdnative/waves.gdns")
onready var GDNATIVE = WAVES.new()






func load_electron_state(e_idx, e_t, e_p, e_v):
	GDNATIVE.load_electron_state(e_idx, e_t, e_p, e_v)


# Called when the node enters the scene tree for the first time.
func _ready():
#	print(GDNATIVE.get_heartbeat("test"))
	print(GDNATIVE.get_particle_test())
	pass # Replace with function body.
