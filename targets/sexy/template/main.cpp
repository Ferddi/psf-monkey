
#include "main.h"

//${CONFIG_BEGIN}
//${CONFIG_END}

// This routine place comments at the head of a section of debug output
void OutputHeading( const char * explanation )
{
   _RPT1( _CRT_WARN, "\n\n%s:\n**************************************\
************************************\n", explanation );
}

// The following macros set and clear, respectively, given bits
// of the C runtime library debug flag, as specified by a bitmask.
#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
            _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

//${TRANSCODE_BEGIN}
//${TRANSCODE_END}

//int main( int argc,const char *argv[] ){
//
//	BBMonkeyGame::Main( argc,argv );
//}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	BBMonkeyGame::Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return 0;
}
