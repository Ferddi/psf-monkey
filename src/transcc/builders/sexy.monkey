
Import builder

Class SexyBuilder Extends Builder

	Method New( tcc:TransCC )
		Super.New( tcc )
	End
	
	Method Config:String()
		Local config:=New StringStack
		For Local kv:=Eachin _cfgVars
			config.Push "#define CFG_"+kv.Key+" "+kv.Value
		Next
		Return config.Join( "~n" )
	End
	
	'***** Vc2010 *****
	Method MakeVc2010:Void()
		
		CreateDir "vc2010/"+casedConfig
		CreateDir "vc2010/"+casedConfig+"/internal"
		CreateDir "vc2010/"+casedConfig+"/external"
		
		CreateDataDir "vc2010/"+casedConfig+"/data"
		
		Local main:=LoadString( "main.cpp" )
		
		main=ReplaceBlock( main,"TRANSCODE",transCode )
		main=ReplaceBlock( main,"CONFIG",Config() )
		
		SaveString main,"main.cpp"
		
		If tcc.opt_build

			ChangeDir "vc2010"

			Execute tcc.MSBUILD_PATH+" /p:Configuration="+casedConfig+" /p:Platform=Win32 MonkeyGame.sln"
			
			If tcc.opt_run
			
				ChangeDir casedConfig

				Execute "MonkeyGame"
				
			Endif
		Endif
	End

	'***** Builder *****	
	Method IsValid:Bool()
		Select HostOS
		Case "winnt"
			If tcc.MSBUILD_PATH Return True
		End
		Return False
	End
	
	Method Begin:Void()
		ENV_LANG="cpp"
		_trans=New CppTranslator
	End
	
	Method MakeTarget:Void()
		Select HostOS
		Case "winnt"
			MakeVc2010
		End
	End
	
End
