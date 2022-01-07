extends Node

var sources_count = 0
var sources = [
	# ...
]
func add_as_source(node):
	sources.push_back(node)
	sources_count += 1
	return sources_count - 1

func wave_propagation_speed():
	return 1.0

func draw_vector(point, F, color):
	color.a = 0.10 * F.length_squared()
	UI.vector(point, F * 0.1, color, false, true)

func draw_field(point, component):
	var WAVE_SPEED = wave_propagation_speed()
	if component & 1:
		var E = Gdnative.GDNATIVE.get_field_at_point(point, WAVE_SPEED, 0)
		draw_vector(point, E, Color(1,0,0))
	if component & 2:
		var B = Gdnative.GDNATIVE.get_field_at_point(point, WAVE_SPEED, 1)
		draw_vector(point, B, Color(0,0,1))
	if component & 4:
		var u = Gdnative.GDNATIVE.get_field_at_point(point, WAVE_SPEED, 2)
		draw_vector(point, u, Color(1,1,0))

# Called every frame. 'delta' is the elapsed time since the previous frame.
var DISPLAY = 0
var COMPONENT = 1+2
func _process(delta):

	var P = Vector3(0,0,0)
	var fs = 40
	var ff = fs/2 - 0.5*(1-fs%2)
	var fx = 0.1

	match DISPLAY:
		0: # single EM wave in a straight line
			fs = 500
			ff = fs/2 - 0.5*(1-fs%2)
			fx = 0.03
			for x in range(fs):
				var pp = P + Vector3(x - ff, 0, 0) * fx
				draw_field(pp, COMPONENT)
		1: # 2D (flat square on the plane)
			fs = 30
			ff = fs/2 - 0.5*(1-fs%2)
			fx = 0.1
			for x in range(fs):
				for z in range(fs):
					var pp = P + Vector3(x - ff, 0, z - ff) * fx
					draw_field(pp, COMPONENT)
		2: # 3D (full cube)
			fs = 10
			ff = fs/2 - 0.5*(1-fs%2)
			fx = 0.1
			for y in range(fs):
				for x in range(fs):
					for z in range(fs):
						var pp = P + Vector3(x - ff, y - ff, z - ff) * fx
						draw_field(pp, COMPONENT)


