extends Node

var sources = []

#var fields = {
#	"E": [],
#	"B": []
#}

func calc_at_point(point):
	var E = Vector3()
	for s in sources:
		E += s.velocity_from_time_ago(point)
	UI.vector(point, E * 15, Color(1,0,0,40*E.length_squared()), true)



# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):

	var P = Vector3(0,2,0)
	var fs = 10
	var ff = fs/2
	var fx = 1.0
	for x in range(-fs, fs):
		for z in range(-fs, fs):
			calc_at_point(P + Vector3(x * fx, 0, z * fx))
