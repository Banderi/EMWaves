extends Spatial

var speed = 10.0
var intensity = Vector3(0, 0.1, 0)
var velocity = Vector3(0, -1, 0) # I guess we're going with this!
#var position = 0.0

var velocity_history = [
	# ...
]

func store_velocity():
	var max_history = 50
	velocity_history.push_back([life, translation, velocity])
	while velocity_history.size() > max_history:
		velocity_history.pop_front()


func SPEED_OF_LIGHT():
	return 10.0

func get_closet_to_timestamp(i, point_c, c):

	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()

	var H = velocity_history[i]

	var p = H[1]
	var v = H[2]

	var v_alpha = c.normalized() * v.dot(c) / SPEED_OF_LIGHT
	var delta_p = abs(point_c - p)
	var delta_vel = SPEED_OF_LIGHT - v_alpha
	var delta_t = delta_p / delta_vel


func check_if_closer(i, point, approx, USE_SQUARED = true):
	var H = velocity_history[i]
	var t = H[0]
	var p = H[1]
#	var v = H[2]

	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()

	var time_difference = abs(t - life)
	var ideal_distance_at_time = null
	var actual_distance = null
	if USE_SQUARED:
		ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
		ideal_distance_at_time = ideal_distance_at_time * ideal_distance_at_time
		actual_distance = (point - p).length_squared()
	else:
		ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
		actual_distance = (point - p).length()
	var distance_accuracy = abs(ideal_distance_at_time - actual_distance)

#	var VECTOR = (p - point)
#	var NORMAL = VECTOR.normalized()
#	var IDEAL = NORMAL * ideal_distance_at_time
#	UI.vector(point, IDEAL, Color(0,0,1,1))

	if distance_accuracy < approx:
#		closest = [i, H, distance_accuracy]
#		closest_accuracy = distance_accuracy
		return distance_accuracy
	return null


func velocity_from_time_ago(point):
	var closest_accuracy = 1000.0
	var closest_E = Vector3()

	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()
#	var CHECKED = 0
	var USE_SQUARED = true

	for i in velocity_history.size():

#		if CHECKED > 10:
#			return closest


		var H = velocity_history[i]
		var t = H[0]
		var p = H[1]
#		var v = H[2]

		var time_difference = abs(t - life)
		var ideal_distance_at_time = null
		var actual_distance = null
		if USE_SQUARED:
			ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
			ideal_distance_at_time = ideal_distance_at_time * ideal_distance_at_time
			actual_distance = (point - p).length_squared()
		else:
			ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
			actual_distance = (point - p).length()
		var distance_accuracy = abs(ideal_distance_at_time - actual_distance)

#		var VECTOR = (p - point)
#		var NORMAL = VECTOR.normalized()
#		var IDEAL = NORMAL * ideal_distance_at_time
#		UI.vector(point, IDEAL, Color(0,0,1,1))

		if distance_accuracy < closest_accuracy:
			closest_E = H[2]
			closest_accuracy = distance_accuracy







#		var cc = check_if_closer(i, point, closest_accuracy)
#		if cc != null:
#			closest_accuracy = cc
#			closest = [i, velocity_history[i], closest_accuracy]

#		CHECKED += 1

#	print(closest)
	return closest_E

# Called every frame. 'delta' is the elapsed time since the previous frame.
var life = 0.0
func _process(delta):
	life += delta

	velocity = sin(life * speed) * intensity
	store_velocity()
	translate(velocity) # move particle!!

#	UI.vector(translation, velocity * 20, Color(1,0,0,1)) # draw E vector around the electron

#	var fs = 10
#	var ff = fs/2
#	for x in range(-ff, ff):
#		for y in range(-ff, ff):
#			for z in range(-ff, ff):
#				var VEC = translation + Vector3(x, y, z)
#				UI.vector(VEC, velocity * 20, Color(1,0,0,1))

# Called when the node enters the scene tree for the first time.
func _ready():
	get_parent().sources.push_back(self)
	pass # Replace with function body.
