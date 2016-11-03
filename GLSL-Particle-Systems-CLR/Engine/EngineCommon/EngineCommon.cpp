//================================================================
//                     EngineCommon.cpp
//================================================================
#include "Engine/EngineCommon/EngineCommon.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include <string>
std::string g_projectName = "Unnamed";

bool	g_appIsRunning = true;
double	g_secondsPerTick; 
float	g_currentSeconds;

bool	g_renderWorld = true;
bool	g_debugMode = false;
bool	g_displayConsole = false;
bool	g_displayFPS = true;
bool	g_polygonLines = false;
bool	g_use3DCamera = true;
bool	g_displayMousePosition = false;

bool	g_fullScreen = true;
bool	g_borderless = false;
bool	g_displayInfoTable = true;
size_t	g_numberInfoTableLines = 10;
bool	g_drawAxes = true;
Rgba	g_clearBufferColor = Rgba(0, 0, 0, 0);


std::string  g_globalUtilityString;
std::string  g_globalUtilityString2;


const double g_maxFramesPerSecond = 60.0;
const double g_minFramesPerSecond = 5.0;
const double g_mininumSecondsPerFrame = 1.0 / g_maxFramesPerSecond;
const double g_maximumSecondsPerFrame = 1.0 / g_minFramesPerSecond;
int			 g_frameCount = 0;

const std::string g_masterFont		 = "./data/fontInfo/ariel.txt";
const std::string g_masterFontShader = "./data/fontInfo/font";
const std::string g_masterFontImage	 = "./data/fontInfo/text.png";

