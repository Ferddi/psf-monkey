Strict

Import mojo

#If TARGET = "sexy"

Extern Private

	Function SetGraphicsNative:Void(width:Int, height:Int) = "SetGraphicsNative"
	Function SwitchScreenNative:Void(fullScreen:Bool) = "SwitchScreenNative"

Public

#End

'functions
Function SetGraphics:Void(width:Int, height:Int)
	' --- this will change the resolution of the running app ---
	#If TARGET = "sexy"
	SetGraphicsNative(width, height)
	#End
End

Function SwitchScreen:Void(fullScreen:Bool)
	' --- this will change the resolution of the running app ---
	#If TARGET = "sexy"
	SwitchScreenNative(fullScreen)
	#End
End
