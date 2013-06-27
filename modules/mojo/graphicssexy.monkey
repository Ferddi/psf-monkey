Strict

Import mojo

#If TARGET = "sexy"

Extern Private

	Function SetGraphicsNative:Void(width:Int, height:Int) = "SetGraphicsNative"
	Function SwitchScreenNative:Void(fullScreen:Bool) = "SwitchScreenNative"
	Function SetLinearBlendNative:Void(fullScreen:Bool) = "SetLinearBlendNative"

Public

#End

'functions
Function SetGraphics:Void(width:Int, height:Int)
	' --- this will change the resolution of the running app ---
	#If TARGET = "sexy"
	SetGraphicsNative(width, height)
	#End
End Function

Function SwitchScreen:Void(fullScreen:Bool)
	' --- this will change the resolution of the running app ---
	#If TARGET = "sexy"
	SwitchScreenNative(fullScreen)
	#End
End Function

Function SetLinearBlend:Void(blend:Bool)
	#If TARGET = "sexy"
	SetLinearBlendNative(blend)
	#End
End Function