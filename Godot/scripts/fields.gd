extends Node

var sources_count = 0
var sources = [
	# ...
]
func add_as_source(node):
	sources.push_back(node)
	sources_count += 1
	return sources_count - 1
#func ask_for_my_id(node):
#	return sources[node]

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

#	var cache_id = 0
	for x in range(fs):
		for z in range(fs):
			var pp = P + Vector3(x * fx - ff, 0, z * fx - ff)
			var E = Gdnative.GDNATIVE.get_E_at_point(pp, 20.0)
			UI.vector(pp, E * 15, Color(1,0,0,40 * E.length_squared()), true)
#			calc_at_point(P + Vector3(x * fx - ff, 0, z * fx - ff), cache_id)
#			cache_id += 1

#	Gdnative.GDNATIVE.get_particle_test()
#	for i in range(500):
#		Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))
#	Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))
#	Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))
#	Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))
#	Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))
#	Gdnative.GDNATIVE.get_E_at_point(Vector3(25,1,48))

