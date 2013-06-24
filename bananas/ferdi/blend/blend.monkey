
Import mojo

Function DrawRectLine:Void(x:Int, y:Int, w:Int, h:Int)

	DrawLine(x,y,x+w,y)
	DrawLine(x,y+h,x+w,y+h)
	DrawLine(x,y,x,y+h)
	DrawLine(x+w,y,x+w,y+h)

End Function

Class MojoGame Extends App

	Field sparkImage:Image

	' Stuff to do on startup...
	Method OnCreate()
		' 60 frames per second, please!
		SetUpdateRate 60
		
		sparkImage=LoadImage( "bluspark.png",1,Image.MidHandle )
		
	End Method

	' Stuff to do while running...
	Method OnUpdate()

	End Method

	Method TestFillRect(x:Int, y:Int)

		DrawRect(x, y, 20, 20)
		DrawRect(x+10, y+10, 20, 20)
	
	End Method
	
	Method TestDrawLine(x:Int, y:Int)
	
		DrawLine(x, y, x+20, y+20)
		DrawLine(x+10, y+10, x+20, y+20)

	End Method

	Method TestDrawPoint(x:Int, y:Int)
	
		For Local j = 0 Until 20
			For Local i = 0 Until 20
				DrawPoint(x+i, y+j)
				DrawPoint(x+i+10, y+j+10)
			End For
		End For 
	
	End Method
		
	Method TestPolyFillAA(x:Int, y:Int)

		DrawCircle(x, y, 10)
		DrawCircle(x+5, y+5, 10)
	
	End Method

	Method TestDrawImageF(x:Int, y:Int)
		
		DrawImage(sparkImage, x, y)
		DrawImage(sparkImage, x+10, y+10)
	
	End Method

	Method TestDrawImage(x:Int, y:Int)
	
		DrawImage(sparkImage, x, y)
		DrawImage(sparkImage, x+10, y+10)

	End Method

	Method TestDrawTriangleTex(x:Int, y:Int)

		DrawImage(sparkImage, x, y, 10, 1.0, 1.0)
		DrawImage(sparkImage, x+10, y+10, 10, 1.0, 1.0)

	End Method

	' Drawing code...
	Method OnRender ()

		Cls 0, 0, 0						' Clear screen
		
		For Local i:Int = 0 Until 4
		
			If i = 0 Then
				SetColor(128, 128, 128)
				SetAlpha(1.0)
				SetBlend(AlphaBlend)
			Else If i = 1 Then
				SetColor(128, 128, 128)
				SetAlpha(0.9)
				SetBlend(AdditiveBlend)
			Else If i = 2 Then 
				SetColor(255, 255, 255)
				SetAlpha(0.3)
				SetBlend(AlphaBlend)
			Else If i = 3 Then 
				SetColor(255, 255, 255)
				SetAlpha(0.3)
				SetBlend(AdditiveBlend)
			End If
		
			TestFillRect(50 + i * 100, 50)
			TestDrawLine(50 + i * 100, 100)
			TestDrawPoint(50 + i * 100, 150)
			TestPolyFillAA(50 + i * 100, 200)
			TestDrawImageF(50 + i * 100, 250)
			TestDrawImage(50 + i * 100, 300)
			TestDrawTriangleTex(50 + i * 100, 350)
			
		End For 
		
		
	End Method

End Class

Function Main ()
	New MojoGame
End Function
