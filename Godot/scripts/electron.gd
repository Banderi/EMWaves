extends Spatial

var ELECTRON_ID = null

var speed = 10.0
var intensity = Vector3(0, 0.1, 0)
var velocity = Vector3(0, -1, 0) # I guess we're going with this!
#var position = 0.0

func update_position():
	var r = Gdnative.GDNATIVE.load_electron_state(ELECTRON_ID, life, translation)

# Called every frame. 'delta' is the elapsed time since the previous frame.
var life = 0.0
func _process(delta):
	life += delta

	velocity = sin(life * speed) * intensity
	translate(velocity) # move particle!!
	update_position()

	if !visible:
		return

# Called when the node enters the scene tree for the first time.
func _ready():
	ELECTRON_ID = get_parent().add_as_source(self)
