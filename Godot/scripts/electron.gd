extends Spatial

var ELECTRON_ID = null

var speed = 10.0
var intensity = Vector3(0, 0.1, 0)
var velocity = Vector3(0, -1, 0) # I guess we're going with this!
#var position = 0.0

var velocity_history = [
	# ...
]

func store_velocity():
#	var max_history = 6000
#	velocity_history.push_back([life, translation, velocity])
#	while velocity_history.size() > max_history:
#		velocity_history.pop_front()
	var r = Gdnative.GDNATIVE.load_electron_state(ELECTRON_ID, life, translation)
#	print(r)
	pass

func SPEED_OF_LIGHT():
	return 20.0

func get_closet_to_timestamp(i, point_c, c):

	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()

	var H = velocity_history[i]

	var p = H[1]
	var v = H[2]

	var v_alpha = c.normalized() * v.dot(c) / SPEED_OF_LIGHT
	var delta_p = abs(point_c - p)
	var delta_vel = SPEED_OF_LIGHT - v_alpha
	var delta_t = delta_p / delta_vel
func check_if_closer(i, point, closest_accuracy, USE_SQUARED = true):
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

	if distance_accuracy < closest_accuracy:
#		closest = [i, H, distance_accuracy]
#		closest_accuracy = distance_accuracy
		return distance_accuracy
	return null

func method_2_test_i(i, point, DEBUG = null):
	if i < 0 || i > velocity_history.size() - 1: # make sure it's within bounds!
		return 1000
	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()
	var H = velocity_history[i]

	var t = H[0]
	var p = H[1]
	var v = H[2]

	var VECTOR = (p - point) # vector pointing from the P ("point") probe to the H ("p") source
	var NORMAL = VECTOR.normalized()
	var DELTA_T = abs(life - t) + (0.03)
	var c = NORMAL * SPEED_OF_LIGHT * DELTA_T # vector scaled by the SPEED OF LIGHT - this is where the signal would have STARTED

	var distance_discrepancy = (SPEED_OF_LIGHT * DELTA_T) - VECTOR.length()
	match DEBUG:
		null:
			pass
		0:
			var color_alpha = min(max(0.6 - 0.1 * abs(distance_discrepancy), 0),1)
			if distance_discrepancy < 0:
				var color = Color(Color(0,1,0,1)).linear_interpolate(Color(0,0,1,1), -distance_discrepancy)
				color.a = color_alpha
				UI.vector(point + c, -c, color, true)
			else:
				var color = Color(Color(0,1,0,1)).linear_interpolate(Color(1,0,0,1), distance_discrepancy)
				color.a = color_alpha
				UI.vector(point + c, -c, color, true)
		1:
			var rr = min(1, max(0, abs(0.2*pow(distance_discrepancy,1))))
			UI.vector(point, Vector3(0, abs(rr), 0), Color(1,1,0,1-rr), true)
		2:
#			var rr = min(1, max(0, abs(0.2*pow(distance_discrepancy,5))))
			if abs(distance_discrepancy) < 1.0:
				var color = Color(Color(0,1,0,1)).linear_interpolate(Color(1,0,0,1), abs(distance_discrepancy))
				color.a = min(max(0.5 - 1.3 * abs(distance_discrepancy), 0),1)
				UI.vector(point, Vector3(0, distance_discrepancy, 0), color, true)
		3:
			if abs(distance_discrepancy) < 0.1:
				UI.vector(point, Vector3(0, distance_discrepancy, 0), Color(0,1,0,1), true)


	return distance_discrepancy

var smart_index_cache_dict = {}
var smart_index_cache_array = []

func velocity_from_time_ago(point, cache_id = null):
	var closest_accuracy = 1000.0
	var closest_E = Vector3()

	var SPEED_OF_LIGHT = SPEED_OF_LIGHT()

	var METHOD = 5
	match METHOD:
		0: # go over ALL the records and choose the nearest approximation
			for i in velocity_history.size():
				var H = velocity_history[i]
				var t = H[0]
				var p = H[1]

				var time_difference = abs(t - life)
				var ideal_distance_at_time = null
				var actual_distance = null
				if true:
					ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
					ideal_distance_at_time = ideal_distance_at_time * ideal_distance_at_time
					actual_distance = (point - p).length_squared()
				else:
					ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
					actual_distance = (point - p).length()
				var distance_accuracy = abs(ideal_distance_at_time - actual_distance)

				if false:
					var VECTOR = (p - point)
					var NORMAL = VECTOR.normalized()
					var IDEAL = NORMAL * ideal_distance_at_time
					UI.vector(point, IDEAL, Color(0,0,1,1))

				if distance_accuracy < closest_accuracy:
					closest_E = H[2]
					closest_accuracy = distance_accuracy
		1: # same as one, but delegates the calc to a function call (stack frames == SLOW AS FUC)
			for i in velocity_history.size():
				var cc = check_if_closer(i, point, closest_accuracy)
				if cc != null:
					closest_accuracy = cc
					closest_E = velocity_history[i][2]
		2: # iterative process attempt (broken)
			var TESTS = 0
			var MAX_TESTS = 5
			var i = velocity_history.size() - 1  # start on the NEWEST point in memory.

			var SUB_CHECK = 0

			while abs(closest_accuracy) > 0.1 && TESTS < MAX_TESTS && i >= 0:
				var rr = method_2_test_i(i, point, -1)

				if abs(rr) <= 0.1: # already hit within min accuracy, return.
					return Vector3()
#					return velocity_history[i][2]
				else:
					if abs(rr) < abs(closest_accuracy): # better!
						SUB_CHECK = 0
						closest_accuracy = rr
						i += rr * 1
					else: # worse! >:(


						i -= 1
						i += rr * 1





						SUB_CHECK += 1
						pass
#					i += rr * 1


					TESTS += 1
#				UI.point(point + Vector3(0,-TESTS*0.1,0), Color(0,0,1,1))

#				var point_c = point + c
#				UI.vector(point, c, Color(0,0,1,1), true)

#				var v_alpha = v.project(VECTOR).length()
#				var delta_p = (point_c - p).length()
#				var delta_vel = SPEED_OF_LIGHT - v_alpha
#				var delta_t = delta_p / delta_vel

#				UI.vector(point, (delta_t + 13) * Vector3(0,1,0), Color(0,0,1,delta_t * 0.05 + 0.5), true)
#			UI.point(point + Vector3(0,-TESTS*0.1,0), Color(0,0,1,1))
			UI.vector(point, Vector3(0, closest_accuracy, 0), Color(0,1,0,1), false)
		3: # smart index caching
			var TESTS = 0
			var MAX_TESTS = 5

			# fetch cached index, or start from 0.
			var i = 0 # OLDEST point in the history. (0)
			if smart_index_cache_dict.has(point):
				var cc = smart_index_cache_dict[point]
				i = cc.index
				pass

			# temp container for the cache.
			var index_to_be_cached = i
			var rr_next
			var rr_curr
			var rr_prev

			### STEP 1: start from the **CACHED** or the **OLDEST** index (0) in the history.
			rr_curr = method_2_test_i(i, point, -1)

			# no need to check; the closest accuracy at this point will always be the initial 1000.0
			closest_accuracy = rr_curr
			index_to_be_cached = i

			if abs(closest_accuracy) <= 0.1: # already hit within min accuracy, return.
				pass # ...
			else:

				### STEP 2: check the **PREVIOUS** index in the history.
				if i > 0:
					rr_prev = method_2_test_i(i - 1, point, -1) # PREVIOUS point in the history. (-1)

					# better than the last one!!
					if abs(rr_prev) < abs(closest_accuracy):
						closest_accuracy = rr_prev
						index_to_be_cached = i - 1

				### STEP 3: check the **NEXT** index in the history.
				if i < velocity_history.size() - 1:
					rr_next = method_2_test_i(i + 1, point, -1) # NEXT point in the history. (+1)

					# the NEXT is the BEST of the triplet, so... we're done!
					if abs(rr_next) < abs(closest_accuracy):
						closest_accuracy = rr_next
						index_to_be_cached = i + 1


			smart_index_cache_dict[point] = {
				"index": index_to_be_cached,
				"accuracy": closest_accuracy
			}
#			UI.vector(point, Vector3(0, closest_accuracy, 0), Color(0,1,0,1), false)
			if velocity_history.size() > index_to_be_cached:
				closest_E = velocity_history[index_to_be_cached][2]
		4: # smart caching (array)
			var TESTS = 0
			var MAX_TESTS = 5

			# fetch cached index, or start from 0.
			var i = 0 # OLDEST point in the history. (0)
			if cache_id == null || cache_id > smart_index_cache_array.size() - 1: # new/invalid cache id?
				cache_id = smart_index_cache_array.size()
				smart_index_cache_array.push_back(null) # add a new (default empty) to the cache array
			else:
				var cc = smart_index_cache_array[cache_id]
				i = cc[0]
				pass

			# temp container for the cache.
			var index_to_be_cached = i
			var rr_next
			var rr_curr
			var rr_prev

			### STEP 1: start from the **CACHED** or the **OLDEST** index (0) in the history.
			rr_curr = method_2_test_i(i, point, -1)

			# no need to check; the closest accuracy at this point will always be the initial 1000.0
			closest_accuracy = rr_curr
			index_to_be_cached = i

			if abs(closest_accuracy) <= 0.1: # already hit within min accuracy, return.
				pass # ...
			else:

				### STEP 2: check the **PREVIOUS** index in the history.
				if i > 0:
					rr_prev = method_2_test_i(i - 1, point, -1) # PREVIOUS point in the history. (-1)

					# better than the last one!!
					if abs(rr_prev) < abs(closest_accuracy):
						closest_accuracy = rr_prev
						index_to_be_cached = i - 1

				### STEP 3: check the **NEXT** index in the history.
				if i < velocity_history.size() - 1:
					rr_next = method_2_test_i(i + 1, point, -1) # NEXT point in the history. (+1)

					# the NEXT is the BEST of the triplet, so... we're done!
					if abs(rr_next) < abs(closest_accuracy):
						closest_accuracy = rr_next
						index_to_be_cached = i + 1
		5: # smart caching (inline)
			# fetch cached index, or start from 0.
			var i = 0 # OLDEST point in the history. (0)
			if smart_index_cache_dict.has(point):
				var cc = smart_index_cache_dict[point]
				i = cc.index
				pass

			# temp container for the cache.
			var index_to_be_cached = i
			var rr_next
			var rr_curr
			var rr_prev

			### STEP 1: start from the **CACHED** or the **OLDEST** index (0) in the history.
#			rr_curr = method_2_test_i(i, point, -1)
			if i < 0 || i > velocity_history.size() - 1: # make sure it's within bounds!
				rr_curr = 1000
			else:
				var H = velocity_history[i]
				var t = H[0]
				var p = H[1]
				var VECTOR = (p - point) # vector pointing from the P ("point") probe to the H ("p") source
				var NORMAL = VECTOR.normalized()
				var DELTA_T = abs(life - t) + (0.03)
				var c = NORMAL * SPEED_OF_LIGHT * DELTA_T # vector scaled by the SPEED OF LIGHT - this is where the signal would have STARTED
				rr_curr = (SPEED_OF_LIGHT * DELTA_T) - VECTOR.length()


			# no need to check; the closest accuracy at this point will always be the initial 1000.0
			closest_accuracy = rr_curr
			index_to_be_cached = i

			if abs(closest_accuracy) <= 0.1: # already hit within min accuracy, return.
				pass # ...
			else:

				### STEP 2: check the **PREVIOUS** index in the history.
				if i > 0:
#					rr_prev = method_2_test_i(i - 1, point, -1) # PREVIOUS point in the history. (-1)
					if i - 1 < 0 || i - 1 > velocity_history.size() - 1: # make sure it's within bounds!
						rr_prev = 1000
					else:
						var H = velocity_history[i - 1]
						var t = H[0]
						var p = H[1]
						var VECTOR = (p - point) # vector pointing from the P ("point") probe to the H ("p") source
						var NORMAL = VECTOR.normalized()
						var DELTA_T = abs(life - t) + (0.03)
						var c = NORMAL * SPEED_OF_LIGHT * DELTA_T # vector scaled by the SPEED OF LIGHT - this is where the signal would have STARTED
						rr_prev = (SPEED_OF_LIGHT * DELTA_T) - VECTOR.length()

					# better than the last one!!
					if abs(rr_prev) < abs(closest_accuracy):
						closest_accuracy = rr_prev
						index_to_be_cached = i - 1

				### STEP 3: check the **NEXT** index in the history.
				if i < velocity_history.size() - 1:
#					rr_next = method_2_test_i(i + 1, point, -1) # NEXT point in the history. (+1)
					if i + 1 < 0 || i + 1 > velocity_history.size() - 1: # make sure it's within bounds!
						rr_next = 1000
					else:
						var H = velocity_history[i + 1]
						var t = H[0]
						var p = H[1]
						var VECTOR = (p - point) # vector pointing from the P ("point") probe to the H ("p") source
						var NORMAL = VECTOR.normalized()
						var DELTA_T = abs(life - t) + (0.03)
						var c = NORMAL * SPEED_OF_LIGHT * DELTA_T # vector scaled by the SPEED OF LIGHT - this is where the signal would have STARTED
						rr_next = (SPEED_OF_LIGHT * DELTA_T) - VECTOR.length()

					# the NEXT is the BEST of the triplet, so... we're done!
					if abs(rr_next) < abs(closest_accuracy):
						closest_accuracy = rr_next
						index_to_be_cached = i + 1


			smart_index_cache_dict[point] = {
				"index": index_to_be_cached,
				"accuracy": closest_accuracy
			}
#			UI.vector(point, Vector3(0, closest_accuracy, 0), Color(0,1,0,1), false)
			if velocity_history.size() > index_to_be_cached:
				closest_E = velocity_history[index_to_be_cached][2]
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

	if !visible:
		return
#	var E = Gdnative.GDNATIVE.get_electron_state(ELECTRON_ID, life)
#	var E = Gdnative.GDNATIVE.get_E_at_point(Vector3(), SPEED_OF_LIGHT())
#	print(E)
#	UI.vector(Vector3(), E * 15, Color(1,0,0,40 * E.length_squared()), true)

# Called when the node enters the scene tree for the first time.
func _ready():
	ELECTRON_ID = get_parent().add_as_source(self)
#	get_parent().ask_for_my_id(self)
	pass # Replace with function body.
