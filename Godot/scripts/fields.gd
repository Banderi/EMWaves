extends Node

var sources_count = 0
var sources = [
	# ...
]
func add_as_source(node):
	sources.push_back(node)
	sources_count += 1
	return sources_count - 1

func calc_at_point(point, cache_id):
	var E = Vector3()
	for s in sources:
		if s.visible:
			E += s.velocity_from_time_ago(point, cache_id)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):

	var P = Vector3(0,2,0)
	var fs = 20
	var ff = fs/2 - 0.5*(1-fs%2)
	var fx = 1.5

	for y in range(fs):
		for x in range(fs):
			for z in range(fs):
				var pp = P + Vector3(x - ff, y - ff, z - ff) * fx
				var E = Gdnative.GDNATIVE.get_E_at_point(pp, 20.0)
				UI.vector(pp, E * 5, Color(1,0,0,40 * E.length_squared()), false)

