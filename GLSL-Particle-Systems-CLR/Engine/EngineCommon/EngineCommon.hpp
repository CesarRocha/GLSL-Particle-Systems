//================================================================
//                     EngineCommon.hpp
//================================================================
#ifndef _INCLUDED_ENGINECOMMON_
#define _INCLUDED_ENGINECOMMON_
#include <string>

#pragma warning( disable: 4127 ) // conditional expression is constant
#pragma warning( disable: 4244 ) //conversion from 'time_t' to 'unsigned int', possible loss of data
#define UNUSED(x) (void)(x)
#define CUSTOM_NEW_DELETE

extern std::string g_projectName;
extern bool		g_displayConsole;
extern bool		g_renderWorld;
extern double	g_secondsPerTick; 
extern float	g_currentSeconds;
extern bool		g_debugMode;
extern bool		g_displayFPS;
extern bool		g_appIsRunning;
extern bool		g_polygonLines;
extern bool		g_use3DCamera;
extern bool		g_displayMousePosition;

extern bool		g_displayInfoTable;
extern size_t	g_numberInfoTableLines;
extern bool		g_fullScreen;
extern bool		g_borderless;
extern bool		g_drawAxes;
extern bool		g_use3DCamera;
class Rgba;
extern Rgba		g_clearBufferColor;


extern std::string   g_globalUtilityString;
extern std::string   g_globalUtilityString2;


extern const double g_maxFramesPerSecond;
extern const double g_minFramesPerSecond;
extern const double g_mininumSecondsPerFrame;
extern const double g_maximumSecondsPerFrame;
extern		 int	g_frameCount;

extern const std::string g_masterFont;
extern const std::string g_masterFontShader;
extern const std::string g_masterFontImage;



#endif