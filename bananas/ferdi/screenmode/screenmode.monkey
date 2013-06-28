
Import mojo
Import mojo.graphicssexy

Function Main:Int()

	New Demo
	Return 0

End Function

Class Demo Extends App

	Field fullScreen:= False
	Field image:Image
	
	Method OnCreate:Int()

		' --- app is created ---
		SetUpdateRate(60)
		
		image = LoadImage("player.png")
		
		Return 0

	End Method
	
	Method OnUpdate:Int()

		' --- app is updated ---

		If KeyHit(KEY_SPACE)

			fullScreen = Not fullScreen
			SwitchScreen(fullScreen)

		Endif
		
		If KeyHit(KEY_F1)

			SetGraphics(640, 480)

		Endif

		If KeyHit(KEY_F2)

			SetGraphics(800, 600)

		Endif

		If KeyHit(KEY_F3)

			SetGraphics(1024, 768)

		Endif

		Return 0

	End Method
	
	Method OnRender:Int()

		' --- app is rendered ---

		Cls(0, 0, 0)
		
		DrawImage(image, 300, 50)
		DrawImage(image, 640 - 128, 400)
		
		' Testing new DrawPoly overload!
		DrawPoly([100.0, 100.0, 128.0, 0.0, 200.0, 100.0, 256.0, 128.0, 200.0, 200.0, 128.0, 256.0, 100.0, 200.0, 0.0, 128.0], image)
		
		DrawText("F1 - 640x480", 15, 15)
		DrawText("F2 - 800x600", 15, 30)
		DrawText("Space - Toggle Window / Fullscreen", 15, 45)
		DrawText("Alt + Enter - Toggle Window / Fullscreen", 15, 60)
		DrawText("Alt + F4 - Close", 15, 75)
		
		DrawText("MouseX: " + MouseX(), 15, 105)
		DrawText("MouseY: " + MouseY(), 15, 120)
		
		Return 0

	End Method

End Class 
