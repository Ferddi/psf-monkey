
//Lang/OS...
#include <ctime>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <typeinfo>
#include <signal.h>

#if _WIN32
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#undef LoadString

#elif __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <dirent.h>
#include <copyfile.h>

#elif __linux
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>
#endif

// Graphics/Audio stuff

//OpenGL...
#include <GL/glfw.h>

//OpenAL...
//#include <al.h>
//#include <alc.h>

//STB_image lib...
//#define STBI_HEADER_FILE_ONLY
//#include <stb_image.c>

//stb_vorbis lib
//#define STB_VORBIS_HEADER_ONLY
//#include <stb_vorbis.c>

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/DDInterface.h"
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/SoundInstance.h"

// See the Draw method for more information on using the Color class.
#include "SexyAppFramework/Color.h"

// Why are we including ImageFont.h and not Font.h? Font.h is just a generic
// base class. ImageFont creates fonts from an image that contains all the
// text characters as well as a text file that indicates character widths
// and kerning information, as well as some more advanced features not used
// in this tutorial such as font layers, etc.
#include "SexyAppFramework/ImageFont.h"

// The Image.h file just declares basic functions. All images are either of 
// the DDImage or MemoryImage type. For this demo, we will use DDImage
// types, as they are the type returned by the image loading code.
// A DDImage is actually derived from MemoryImage, so where an Image or
// MemoryImage is required, a DDImage will suffice as well. A DDImage
// contains optimized code for use with DirectX 7+.
#include "SexyAppFramework/DDImage.h"

// This will let us load sounds
#include "SexyAppFramework/SoundManager.h"

// The Rectangle template, used to specify X, Y, Width, Height
#include "SexyAppFramework/Rect.h"
#include "SexyAppFramework/SexyMatrix.h"

#include "SexyAppFramework/SWTri.h"

// The SexyAppFramework resides in the "Sexy" namespace. As a convenience,
// you'll see in all the .cpp files "using namespace Sexy" to avoid
// having to prefix everything with Sexy::
using namespace Sexy;

