extends KinematicProp

var ELECTRON_ID = null

var speed = 5.0
var intensity = Vector3(0, 1.0, 0)
#var velocity = Vector3(0, -1, 0) # I guess we're going with this!
#var position = 0.0

func update_position():
	if ELECTRON_ID == null:
		ELECTRON_ID = get_parent().add_as_source(self)
	var r = Gdnative.GDNATIVE.load_electron_state(ELECTRON_ID, life, body.translation)

# Called every frame. 'delta' is the elapsed time since the previous frame.
var life = 0.0
func _process(delta):
	life += delta
	if !visible:
		return

	var vel = sin(life * speed) * intensity
	move_to_point(vel)
	._process(delta)

	# GDNative...
	update_position()


# Called when the node enters the scene tree for the first time.
#func _ready():
#	if visible:
#		ELECTRON_ID = get_parent().add_as_source(self)
