
//***** monkeygame.cpp *****

void BBMonkeyGame::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Make sure to set this. Some classes, like the exception handler and custom cursors
	// will need to use it.
	gHInstance = hInstance;

	BBMonkeyGame *game=new BBMonkeyGame();
	gPopCapGame = game;
	
	try
	{
		int argc = 0;
		char **argv = (char **)CommandLineToArgvW((LPWSTR)lpCmdLine, &argc);
		bb_std_main( argc, (const char **)argv );		
	}
	catch( ThrowableObject *ex )
	{
		game->Die( ex );
		return;
	}

	if( game->Delegate() ) game->Run();

	delete game;
	
	exit( 0 );
}
