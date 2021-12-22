extends Node

var sources = []

func calc_at_point(point, cache_id):
	var E = Vector3()
	for s in sources:
		if s.visible:
			E += s.velocity_from_time_ago(point, cache_id)
	UI.vector(point, E * 15, Color(1,0,0,40 * E.length_squared()), true)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):

	var P = Vector3(0,2,0)
	var fs = 50
	var ff = fs/2 - 0.5*(1-fs%2)
	var fx = 1.0

	var cache_id = 0
	for x in range(fs):
		for z in range(fs):
			calc_at_point(P + Vector3(x * fx - ff, 0, z * fx - ff), cache_id)
			cache_id += 1
