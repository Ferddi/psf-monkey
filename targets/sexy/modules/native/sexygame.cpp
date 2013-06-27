
//***** sexygame.h *****

class BBSexyGame : public BBGame{
public:
	BBSexyGame();

	static BBSexyGame *SexyGame(){ return _sexyGame; }
	
	virtual void SetUpdateRate( int hertz );
	virtual int Millisecs();
	virtual bool PollJoystick( int port,Array<Float> joyx,Array<Float> joyy,Array<Float> joyz,Array<bool> buttons );
	virtual void OpenUrl( String url );
	virtual void SetMouseVisible( bool visible );

	virtual String PathToFilePath( String path );
	
	virtual void Run();
	
private:
	static BBSexyGame *_sexyGame;

	bool _iconified;
	
	double _nextUpdate;
	double _updatePeriod;

	int startTime;
		
protected:

	static void OnMouseButton( int button,int action );
	static void OnMousePos( int x,int y );
	static int  OnWindowClose();
};

//***** monkeygame.h *****

class BBMonkeyGame : public BBSexyGame{
public:

	static void Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
};

BBMonkeyGame *gPopCapGame = NULL;

//***** board.h *****

//////////////////////////////////////////////////////////////////////////
//						Board.h
//
//	This is the third class to look at in this particular demo
//	(after main.cpp and GameApp.h/.cpp). The Board class is where most of
//	your actual game programming will go. It is here that we will do
//	all our game drawing, updating, and input processing. Of course, in
//	a larger application, you would probably do drawing and updating in
//	multiple files, but you would still most likely use something similar
//	to a Board class as the master game logic class. 
//
//	The reason that the Board class is a widget is because when a widget
//	is added to the GameApp's WidgetManager, it will automatically have its
//	Update and Draw methods called, and it will automatically receive input
//	at the appropriate times. Furthermore, by making it a widget and adding
//	it to the WidgetManager, the game logic loop, Update(), will be guaranteed
//	to run at a standard 100FPS on all machines. This is extremely important
//	as you always want your logic code to run at the same speed, but want
//	the drawing code to run as fast as possible. That way on faster machines
//	your program doesn't run its logic faster than on a slower machine.
//
//	You can think of the Board as a canvas upon which we do all our
//	drawing, and a central hub where if we need to, we instruct other
//	classes where and when to draw to.
//////////////////////////////////////////////////////////////////////////

// We place all our classes inside the "Sexy" namespace to avoid name collisions
// with other libraries that might be added.
namespace Sexy
{


// Forward declare the graphics class. You will see the graphics class used
// and explained in Board.cpp: it is the main object used to draw all
// images, fonts, etc.
class Graphics;

// We maintain a pointer to the main game application in the Board class.
// The main game app contains functions that are often times needed
// by the Board class, such as registry reading/writing, file reading/writing,
// etc.
class GameApp;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class Board : public Widget
{

	private:

		GameApp*	mApp;		

	public:

		//////////////////////////////////////////////////////////////////////////
		//	Function: Board
		//	Parameters:
		//		theApp	- Pointer to the main application class
		//	
		//	Returns: none
		//////////////////////////////////////////////////////////////////////////
		Board(GameApp* theApp);

		virtual ~Board();


		//////////////////////////////////////////////////////////////////////////
		//	Function: Draw
		//	Parameters:
		//		g	- Graphics object used to draw all images and fonts to the screen.
		//	
		//	Returns: none
		//
		//	Purpose: Called automatically by GameApp's WidgetManager, this function
		//	is the main method that is responsible for all graphical and textual
		//	displaying.
		//////////////////////////////////////////////////////////////////////////
		virtual void Draw(Graphics* g);

		//////////////////////////////////////////////////////////////////////////
		//	Function: Update
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called automatically by GameApp's WidgetManager, this method
		//	is GUARANTEED to be called 100 times per second (100FPS) and is where
		//	all main game logic is performed. Of course, if you had a larger more
		//	complex game, you'd most likely divide your logic between several
		//	other files, but this is commonly the central place where all game
		//	logic begins and is executed.
		//////////////////////////////////////////////////////////////////////////
		virtual void Update();

		//////////////////////////////////////////////////////////////////////////
		//	Function: MouseMove
		//	Parameters:
		//		x - X coordinate relative to the application of the mouse 
		//		y - Y coordinate relative to the application of the mouse
		//
		//	Returns: none
		//
		//	Purpose: Called by the WidgetManager automatically anytime the
		//	mouse moves. The topmost widget (i.e. the widget most immediately
		//	under the cursor) is the one who gets the function call, and any
		//	widgets underneath by default are ignored.
		//////////////////////////////////////////////////////////////////////////		
		virtual void	MouseMove(int x, int y);
		virtual void	MouseDrag(int x, int y);

		//////////////////////////////////////////////////////////////////////////
		//	Function: MouseDown
		//	Parameters:
		//		x - X coordinate relative to the application of the mouse 
		//		y - Y coordinate relative to the application of the mouse
		//		theClickCount - An integer indicating which mouse button
		//						was pressed. One of the following:
		//			1:  Left button
		//			2:  Double-left-click
		//			3:  Middle button
		//			-1: Right button
		//			-2: Double-right-click
		//
		//		IMPORTANT: Because you can't have a double click with a
		//		single click, you will receive BOTH a left (or right) button as well
		//		as a double left (or right) button message whenever you double-click.
		//
		//	Returns: none
		//
		//	Purpose: Called by the WidgetManager automatically anytime 
		//	a mouse button is in the down state.
		//	The topmost widget (i.e. the widget most immediately
		//	under the cursor) is the one who gets the function call, and any
		//	widgets underneath by default are ignored.
		//////////////////////////////////////////////////////////////////////////
		virtual void	MouseDown(int x, int y, int theBtnNum, int theClickCount);	// theBtnNum = 0 left, 1 right, 2 middle
		virtual void	MouseUp(int x, int y, int theBtnNum, int theClickCount);
		virtual void	MouseWheel(int theDelta);

		virtual void	KeyChar(SexyChar theChar);
		virtual void	KeyDown(KeyCode theKey);
		virtual void	KeyUp(KeyCode theKey);	
		
};

}

//***** gameapp.h *****

//////////////////////////////////////////////////////////////////////////
//							GameApp.h
//
//	This is what drives the whole game. In here, you derive your class
//	from SexyAppBase and implement common game tasks, such as 
//	responding to widgets (covered later), initializing and loading
//	resources, setting up the various game screens, etc.
//	All applications at minimum must have a class that derives from
//	SexyAppBase.
//
//	The GameApp class is used to do such things as create the main 
//	menu screen, create the main game class (where all drawing/updating/
//	interaction takes place), etc.
//////////////////////////////////////////////////////////////////////////

// We place all our classes inside the "Sexy" namespace to avoid name collisions
// with other libraries that might be added.
namespace Sexy
{

	// The GameApp class will be responsible for creating a class by the name
	// of "Board", which we will use to do all the game's drawing, input processing,
	// etc. Board is the second most important class and is where almost all of your
	// game logic code will originate from. It is a widget, which allows for
	// easy and automatic invocation of its update, drawing, and input processing
	// functions. See the "Board" class for more details. 
	class Board;

	// Alright, fonts and images! You'll learn more about these in GameApp.cpp
	// so check there for more info. 
	class ImageFont;
	class Image;

class GameApp : public SexyAppBase
{

	private:

		Board*		mBoard;	

	public:

		GameApp();
		virtual ~GameApp();

		//////////////////////////////////////////////////////////////////////////
		//	Function: Init
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Initializes the application. Sets the resolution, overrides
		//	any default settings, and if there is a loader/intro screen (not in this demo)
		//	creates it and displays it. The framework will then automatically
		//	call the LoadingThreadProc() method after this method returns.
		//////////////////////////////////////////////////////////////////////////		
		virtual void	Init();

		//////////////////////////////////////////////////////////////////////////
		//	Function: LoadingThreadProc
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Loads all resources in a separate thread. If there is a 
		//	loader/intro screen (not in this demo), would also update the
		//	loader progress indicator. When the function returns, the
		//	LoadingThreadCompleted() method is automatically called.
		//////////////////////////////////////////////////////////////////////////		
		virtual void	LoadingThreadProc();

		//////////////////////////////////////////////////////////////////////////
		//	Function: LoadingThreadCompleted
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: Called when LoadingThreadProc is complete and all resources
		//	have been loaded. It is in this function that you would then set up
		//	your main menu or similar screen. For this particular demo however,
		//	we will go straight to the main game class, "Board".
		//////////////////////////////////////////////////////////////////////////		
		virtual void	LoadingThreadCompleted();

		//////////////////////////////////////////////////////////////////////////
		//	Function: SetFocusToBoard
		//
		//	Parameters: none
		//	Returns: none
		//
		//	Purpose: When a dialog box closes, this is called to set keyboard focus
		//	to the board widget.
		//////////////////////////////////////////////////////////////////////////		
		void SetFocusToBoard();
};

}

//***** board.cpp *****

GameApp *gPopCapApp = NULL;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Board::Board(GameApp* theApp)
{
	mApp = theApp;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Board::~Board()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void Board::Update()
{
	// Let the parent class update as well. This will increment
	// the variable mUpdateCnt which is an integer that indicates 
	// how many times the Update() method has been called. Since our
	// Board class is updated 100 times per second, this variable will
	// increment 100 times per second. As you will see in later demos,
	// we will use this variable for animation since its value represents
	// hundredths of a second, which is for almost all games a good
	// enough timer value and doesn't rely on the system clock function
	// call.
	Widget::Update();

	if (gPopCapGame != NULL)
	{
		gPopCapGame->UpdateGame();
	}

	// For this and most of the other demos, you will see the function
	// below called every Update() call. MarkDirty() tells the widget
	// manager that something has changed graphically in the widget and
	// that it needs to be repainted. All widgets follow this convention.
	//		In general, if you don't need
	// to update your drawing every time you call the Update method
	// (the most common case is when the game is paused) you should
	// NOT mark dirty. Why? If you aren't marking dirty every frame,
	// then you aren't drawing every frame and thus you use less CPU
	// time. Because people like to multitask, or they may be on a laptop
	// with limited battery life, using less CPU time lets people do
	// other things besides play your game. Of course, everyone
	// will want to play your game at all times, but it's good to be
	// nice to those rare people that might want to read email or
	// do other things at the same time. 
	//		In this particular demo, we
	// won't be nice, as the purpose is to bring you up to speed as
	// quickly as possible, and so we'll dispense with optimizations
	// for now, so you can concentrate on other core issues first.
	//		In general, this is the last method called in the Update
	// function, but that is not necessary. In fact, the MarkDirty
	// function can be called anywhere, in any method (although
	// calling it in the Draw method doesn't make sense since it is
	// already drawing) and even multiple times. Calling it multiple
	// times does not do anything: only the first call makes a difference.
	MarkDirty();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Graphics *gPopCapGraphics = NULL;
bool gIs3D;

void Board::Draw(Graphics* g)
{
	gPopCapGraphics = g;
	gIs3D = g->mIs3D;

	if (gPopCapGame != NULL)
	{
		gPopCapGame->RenderGame();
	}

	gPopCapGraphics = NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::MouseMove(int x, int y)
{
	if (gPopCapGame != NULL)
	{
		gPopCapGame->MouseEvent( BBGameEvent::MouseMove,-1,(float)x,(float)y );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::MouseDrag(int x, int y)
{
	if (gPopCapGame != NULL)
	{
		gPopCapGame->MouseEvent( BBGameEvent::MouseMove,-1,(float)x,(float)y );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	if (gPopCapGame != NULL)
	{
		gPopCapGame->MouseEvent( BBGameEvent::MouseDown, theBtnNum, (float)x, (float)y );	
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
	if (gPopCapGame != NULL)
	{
		gPopCapGame->MouseEvent( BBGameEvent::MouseUp, theBtnNum, (float)x, (float)y );	
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int gSexyMouseWheel;

void Board::MouseWheel(int theDelta)
{
	gSexyMouseWheel = theDelta;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum{
	VKEY_BACKSPACE=8,VKEY_TAB,
	VKEY_ENTER=13,
	VKEY_SHIFT=16,
	VKEY_CONTROL=17,
	VKEY_ESC=27,
	VKEY_SPACE=32,
	VKEY_PAGEUP=33,VKEY_PAGEDOWN,VKEY_END,VKEY_HOME,
	VKEY_LEFT=37,VKEY_UP,VKEY_RIGHT,VKEY_DOWN,
	VKEY_INSERT=45,VKEY_DELETE,
	VKEY_0=48,VKEY_1,VKEY_2,VKEY_3,VKEY_4,VKEY_5,VKEY_6,VKEY_7,VKEY_8,VKEY_9,
	VKEY_A=65,VKEY_B,VKEY_C,VKEY_D,VKEY_E,VKEY_F,VKEY_G,VKEY_H,VKEY_I,VKEY_J,
	VKEY_K,VKEY_L,VKEY_M,VKEY_N,VKEY_O,VKEY_P,VKEY_Q,VKEY_R,VKEY_S,VKEY_T,
	VKEY_U,VKEY_V,VKEY_W,VKEY_X,VKEY_Y,VKEY_Z,
	
	VKEY_LSYS=91,VKEY_RSYS,
	
	VKEY_NUM0=96,VKEY_NUM1,VKEY_NUM2,VKEY_NUM3,VKEY_NUM4,
	VKEY_NUM5,VKEY_NUM6,VKEY_NUM7,VKEY_NUM8,VKEY_NUM9,
	VKEY_NUMMULTIPLY=106,VKEY_NUMADD,VKEY_NUMSLASH,
	VKEY_NUMSUBTRACT,VKEY_NUMDECIMAL,VKEY_NUMDIVIDE,

	VKEY_F1=112,VKEY_F2,VKEY_F3,VKEY_F4,VKEY_F5,VKEY_F6,
	VKEY_F7,VKEY_F8,VKEY_F9,VKEY_F10,VKEY_F11,VKEY_F12,

	VKEY_LSHIFT=160,VKEY_RSHIFT,
	VKEY_LCONTROL=162,VKEY_RCONTROL,
	VKEY_LALT=164,VKEY_RALT,

	VKEY_TILDE=192,VKEY_MINUS=189,VKEY_EQUALS=187,
	VKEY_OPENBRACKET=219,VKEY_BACKSLASH=220,VKEY_CLOSEBRACKET=221,
	VKEY_SEMICOLON=186,VKEY_QUOTES=222,
	VKEY_COMMA=188,VKEY_PERIOD=190,VKEY_SLASH=191
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::KeyChar(SexyChar theChar)
{
	int chr = theChar;

	if (gPopCapGame != NULL)
	{
		gPopCapGame->KeyEvent( BBGameEvent::KeyChar, chr );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::KeyDown(KeyCode theKey)
{
	int key = theKey;

	if (gPopCapGame != NULL)
	{
		gPopCapGame->KeyEvent( BBGameEvent::KeyDown, key );
	}

	switch( key )
	{
		case VKEY_BACKSPACE:
		case VKEY_TAB:
		case VKEY_ENTER:
		case VKEY_ESC:
			break;

		case VKEY_PAGEUP:
		case VKEY_PAGEDOWN:
		case VKEY_END:
		case VKEY_HOME:
		case VKEY_LEFT:
		case VKEY_UP:
		case VKEY_RIGHT:
		case VKEY_DOWN:
		case VKEY_INSERT:
			key |= 0x10000;
			break;

		case VKEY_DELETE:
			key = 127;
			break;

		default:
			key = 0;
			break;
	}

	if( key != 0 )
	{
		if (gPopCapGame != NULL)
		{
			gPopCapGame->KeyEvent( BBGameEvent::KeyChar,key );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void Board::KeyUp(KeyCode theKey)
{
	int key = theKey;

	if (gPopCapGame != NULL)
	{
		gPopCapGame->KeyEvent( BBGameEvent::KeyUp,key );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//***** gameapp.cpp *****

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
GameApp::GameApp()
{
	// mProdName is used for internal purposes to indicate the game that we're working on
	mProdName = "MonkeyGame";

	// For internal uses, indicates the current product version
	mProductVersion = "0.1";

	// This is the text that appears in the title bar of the application window
	mTitle = StringToSexyStringFast("SexyAppFramework: " + mProdName + " - " + mProductVersion);

	// Indicates the registry location where all registry keys will be read from
	// and written to. This is stored under the HKEY_CURRENT_USER tree on 
	// Windows systems.
	mRegKey = "PopCap\\SexyAppFramework\\MonkeyGame";

	// Set the application width/height in terms of pixels here. Let's
	// use a different resolution from Demo 1 just for fun.
	mWidth = CFG_GLFW_WINDOW_WIDTH;
	mHeight = CFG_GLFW_WINDOW_HEIGHT;

	mBoard = NULL;

	// By setting this to true, the framework will automatically check to see
	// if hardware acceleration can be turned on. This doesn't guarantee that it
	// WILL be turned on, however. Some cards just aren't compatible or have
	// known issues. Also, cards with less than 8MB of video RAM aren't supported.
	// There are ways to override the 3D enabled settings, which we will discuss
	// in a later demo. As a side note, if you want to see if you app is
	// running with 3D acceleration, first enable debug keys by pressing
	// CTRL-ALT-D and then press F9. To toggle 3D on/off, press F8. That is just
	// for testing purposes.
	mAutoEnable3D = true;
	SWTri_AddAllDrawTriFuncs();

	// Enable smooth motion via UpdateF
	mVSyncUpdates = true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

GameApp::~GameApp()
{
	// Remove our "Board" class which was, in this particular demo,
	// responsible for all our game drawing and updating.
	// All widgets MUST be removed from the widget manager before deletion.
	// More information on the basics of widgets can be found in the Board
	// class file. If you tried to delete the Board widget before removing
	// it, you will get an assert.
	mWidgetManager->RemoveWidget(mBoard);
	delete mBoard;

	// We need to release the memory allocated to our sounds too.
	// This call frees up the memory for ALL sound effects.
	mSoundManager->ReleaseSounds();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void GameApp::Init()
{
	// Let the parent class perform any needed initializations first.
	// This should always be done.
	SexyAppBase::Init();

	// In later demos, you will see more done with this function.
	// For now, we have nothing else to initialize, so we are done.
	// Once complete, the LoadingThreadProc function will automatically
	// start and we will begin loading all our needed resources.
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void GameApp::LoadingThreadProc()
{
	if (gPopCapGame != NULL)
	{
		gPopCapGame->StartGame();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void GameApp::LoadingThreadCompleted()
{
	// Let the base app class also know that we have completed
	SexyAppBase::LoadingThreadCompleted();

	// When we're actually loading resources, we'll set the
	// mLoadingFailed variable to "true" if there were any problems
	// encountered along the way. If that is the case, just return
	// because we won't want the user to get to the main menu or any
	// other part of the game. We will want them to exit out.
	if (mLoadingFailed)
		return;

	// Now that we're done loading everything we need (which wasn't
	// anything in this particular demo), we need to get the main
	// game screen up and running: That is our "Board" class, and
	// it will handle all the drawing, updating, and input processing
	// for most of the game.
	mBoard = new Board(this);

	// This is a very important step: Because the Board class is a widget
	// (see Board.h/.cpp for more details) we need to tell it what
	// dimensions it has and where to place it. 
	// By default a widget is invisible because its
	// width/height are 0, 0. Since the Board class is our main
	// drawing area and game logic class, we want to make it the
	// same size as the application. For this particular demo, that means
	// 800x600. We will use mWidth and mHeight though, as those were
	// already set to the proper resolution in GameApp::Init().
	mBoard->Resize(0, 0, mWidth, mHeight);

	// Also an important step is to add the newly created Board widget to
	// the widget manager so that it will automatically have its update, draw,
	// and input processing methods called.
	mWidgetManager->AddWidget(mBoard);

	// This is a new step: We're going to tell the WidgetManager
	// that keyboard input and mouse wheel notifications should go to
	// the board object. This way, we'll be able to respond to keypresses:
	mWidgetManager->SetFocus(mBoard);

	// And just to test out our sound playing abilities, let's play the
	// mutator sound to indicate that we're done loading.
	//  We do that by calling GameApp's PlaySample()
	// method and specifying the integral ID of the sound to play. This
	// ID is the same as we used when loading the sound in GameApp::LoadingThreadProc()
//	PlaySample(2);

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void GameApp::SetFocusToBoard()
{
	if (mBoard != NULL)
		mWidgetManager->SetFocus(mBoard);
}

//***** sexygame.cpp *****

#define _QUOTE(X) #X
#define _STRINGIZE( X ) _QUOTE(X)

BBSexyGame *BBSexyGame::_sexyGame;

BBSexyGame::BBSexyGame():
_iconified( false ){
	_sexyGame=this;
	startTime = timeGetTime();
}

//***** BBGame *****

void BBSexyGame::SetUpdateRate( int updateRate )
{
	BBGame::SetUpdateRate( updateRate );

	if (gPopCapApp != NULL)
	{
		gPopCapApp->mSyncRefreshRate = updateRate;
		gPopCapApp->mFrameTime = (1000.0 / updateRate);
		gPopCapApp->mMinUpdateFTimeAcc = 200.0;

		if (updateRate < 5)
		{
			gPopCapApp->mMinUpdateFTimeAcc = 1000.0;
		}

		if( _updateRate )
		{
			_updatePeriod=1.0/_updateRate;
//			_nextUpdate=sexyGetTime()+_updatePeriod;
			_nextUpdate=timeGetTime()+_updatePeriod;
		}
	}
}

int BBSexyGame::Millisecs(){
//	return int( sexyGetTime()*1000.0 );
	return (timeGetTime() - startTime);
}

bool BBSexyGame::PollJoystick( int port,Array<Float> joyx,Array<Float> joyy,Array<Float> joyz,Array<bool> buttons ){

	int joy=GLFW_JOYSTICK_1+port;
	if( !glfwGetJoystickParam( joy,GLFW_PRESENT ) ) return false;

	int n_axes=glfwGetJoystickParam( joy,GLFW_AXES );
	int n_buttons=glfwGetJoystickParam( joy,GLFW_BUTTONS );

	float pos[6];
	memset( pos,0,sizeof(pos) );	
	glfwGetJoystickPos( joy,pos,n_axes );
	
	float t;
	switch( n_axes ){
	case 4:	//my saitek...axes=4, buttons=14
		pos[4]=pos[2];
		pos[3]=pos[3];
		pos[2]=0;
		break;
	case 5:	//xbox360...axes=5, buttons=10
		t=pos[3];
		pos[3]=pos[4];
		pos[4]=t;
		break;
	}
	
	joyx[0]=pos[0];joyx[1]=pos[3];
	joyy[0]=pos[1];joyy[1]=pos[4];
	joyz[0]=pos[2];joyz[1]=pos[5];

	//Buttons...
	//	
	unsigned char buts[32];
	memset( buts,0,sizeof(buts) );
	glfwGetJoystickButtons( port,buts,n_buttons );

	for( int i=0;i<n_buttons;++i ) buttons[i]=(buts[i]==GLFW_PRESS);

	return true;
}

void BBSexyGame::OpenUrl( String url ){
#if _WIN32
	char *cstr=url.ToCString<char>();
	ShellExecute( HWND_DESKTOP,"open",cstr,0,0,SW_SHOWNORMAL );
#elif __APPLE__
	UInt8 *cstr=url.ToCString<UInt8>();
	if( CFURLRef cfurl=CFURLCreateWithBytes( 0,cstr,url.Length(),kCFStringEncodingASCII,0 ) ){
		LSOpenCFURLRef( cfurl,0 );
		CFRelease( cfurl );
	}
#elif __linux
	char *cstr=( String( "xdg-open \"" )+url+"\"" ).ToCString<char>();
	system( cstr );
#endif
}

void BBSexyGame::SetMouseVisible( bool visible )
{
	if( visible )
	{
		if (gPopCapApp != NULL)
		{
			gPopCapApp->SetCursor(CURSOR_POINTER);
		}
	}
	else
	{
		if (gPopCapApp != NULL)
		{
			gPopCapApp->SetCursor(CURSOR_NONE);
		}
	}
}

String BBSexyGame::PathToFilePath( String path )
{
	if( !path.StartsWith( "monkey:" ) )
  {
		return path;
	}
  else if( path.StartsWith( "monkey://data/" ) )
  {
		return String("./data/")+path.Slice(14);
	}
  else if( path.StartsWith( "monkey://internal/" ) )
  {
		return String("./internal/")+path.Slice(18);
	}
  else if( path.StartsWith( "monkey://external/" ) )
  {
		return String("./external/")+path.Slice(18);
	}

	return "";
}

int BBSexyGame::OnWindowClose()
{
//	_game->KeyEvent( BBGameEvent::KeyDown,0x1b0 );
//	_game->KeyEvent( BBGameEvent::KeyUp,0x1b0 );
//	return GL_FALSE;

	return 0;
}

void BBSexyGame::Run()
{
	// Hack initialised GLFW just enough stuff for joystick to function.
	glfwHackInit();

	// Create and initialize our game application.
	gPopCapApp = new GameApp();

	if (gPopCapApp != NULL)
	{
		gPopCapApp->Init();

		// Starts the entire application: sets up the resource loading thread and 
		// custom cursor thread, and enters the game loop where the application
		// will remain until it is shut down. You will most likely not need to
		// override this function.
		gPopCapApp->Start();

		delete gPopCapApp;
	}
	
	OutputHeading("Memory dump of possible memory leaks:" );
	_CrtMemDumpAllObjectsSince( NULL );
}
