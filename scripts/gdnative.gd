extends Node



const WAVES = preload("res://gdnative/waves.gdns")
onready var GDNATIVE = WAVES.new()

#func _on_Button_pressed():
#	$Label.text = "Data = " + data.get_data()





# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	print(GDNATIVE.get_heartbeat("test"))
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
