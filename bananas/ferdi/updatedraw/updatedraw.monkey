
Import mojo
Import mojo.graphicssexy

Function Main:Int()

	New Demo
	Return 0

End Function

Class Demo Extends App

	Field image:Image
	
	Method OnCreate:Int()

		' --- app is created ---
		SetUpdateRate(60)
		
		image = LoadImage("player.png")
		
		Return 0

	End Method
	
	Method OnUpdate:Int()

		' --- app is updated ---

		BeginRenderSexy()

		Cls(0,0,0)
		
		SetColor(255,0,0)
		DrawCircle(100, 100, 10)

		SetColor(0,255,0)
		DrawRect(0, 0, 100, 100)

		SetColor(255, 255, 255)
		DrawImage(image, 100, 100)

		EndRenderSexy()

		Return 0

	End Method
	
	Method OnRender:Int()

		' Nothing needs to be here.
		
		Return 0

	End Method

End Class 
