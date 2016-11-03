#include "TheApp.hpp"
#include <iomanip>
#include "Engine\Console\Console.hpp"
#include "GameCommon.hpp"



//===================================================================================================
//					class TheApp																   ==
//===================================================================================================
TheApp::TheApp()
	: m_inputSystem(NULL)
	, m_renderer(NULL)
	, m_windowHandle(NULL)
{
}
TheApp::~TheApp()
{
}



//===================================================================================================
void TheApp::Startup(void* windowHandle) 
{
	LogClearf(); 



	//New Renderer
	m_windowHandle = windowHandle;
	m_renderer = new OpenGLRenderer();
	if(m_renderer)
		m_renderer->startup((HWND)windowHandle);

	float x = m_renderer->GetDisplayWidth();
	float y = m_renderer->GetDisplayHeight();
	Vector2 displaySize(x,y);

	

	//Console and FPS shaders
	DeveloperLog::CreateConsoleLogAndFPS();
	OpenGLRenderer::CreateDebugGridShader();
	OpenGLRenderer::CreateDebugCoordinateShader();



	//New Input
	m_inputSystem = new InputSystem();
	if(m_inputSystem)
		m_inputSystem->Startup(windowHandle);



	//Setup Camera
	g_masterCamera->InitializeCamera( 0.0f, 0.0f, 0.0f, m_inputSystem);
	g_masterCamera->CreateOrthoMatrix(displaySize.x, displaySize.y, 1.0f, -1.0f);



	//New World
	m_world = new World(displaySize);
	if(m_world)
		m_world->StartUp();
}
void TheApp::Run()
{
	while( g_appIsRunning )
	{
		ProcessInput();
		UpdateWorld();
		RenderWorld();
	}
}
void TheApp::Shutdown()
{
	if(m_renderer)
		m_renderer->shutdown();
	if(m_inputSystem)
		m_inputSystem->Shutdown();
}



//===================================================================================================
void TheApp::ProcessInput()
{
	if(m_inputSystem)
	{
		m_inputSystem->Update();

		m_world->GameInput(m_inputSystem);

		//Toggle Rendering
		if (m_inputSystem->isKeyDown(VK_CONTROL) && m_inputSystem->wasKeyReleased('R'))
			g_renderWorld = !g_renderWorld;

		//Toggle FPS
		if(m_inputSystem->isKeyDown(VK_CONTROL) &&  m_inputSystem->wasKeyReleased('F'))
			g_displayFPS = !g_displayFPS;

		//Toggle InfoTable
		if (m_inputSystem->isKeyDown(VK_CONTROL) && m_inputSystem->wasKeyReleased('I'))
			g_displayInfoTable = !g_displayInfoTable;

		//Toggle Console
		if (m_inputSystem->wasKeyReleased(VK_OEM_3))
			DeveloperLog::GetDeveloperConsole()->ToggleConsoleVisibility();

		//Toggle PolygonLines
		if (m_inputSystem->isKeyDown(VK_CONTROL) && m_inputSystem->wasKeyReleased('X'))
			g_polygonLines = !g_polygonLines;

		//Toggle Mouse-to-Window lock
		if (m_inputSystem->isKeyDown(VK_CONTROL) && m_inputSystem->wasKeyReleased('C'))		
		{
			g_use3DCamera = !g_use3DCamera;
			ShowCursor(!g_use3DCamera);
		}

		//Click to lock mouse to Window
		if (!g_use3DCamera && m_inputSystem->wasMouseReleased(VK_LBUTTON))
		{
			g_use3DCamera = !g_use3DCamera;
			ShowCursor(!g_use3DCamera);
		}


		//Exit App
		if(m_inputSystem->isKeyDown(VK_ESCAPE))
			g_appIsRunning = false;
		else if(m_inputSystem->isReadyToQuit())
			g_appIsRunning = false;
	}
}



//===================================================================================================
void TheApp::UpdateGameTime()
{
	static double lastTimeSeconds = MasterClock::GetMasterClock()->GetAbsoluteTimeSeconds();
	double currentTimeSeconds = MasterClock::GetMasterClock()->GetAbsoluteTimeSeconds();
	m_deltaSeconds = currentTimeSeconds - lastTimeSeconds;
	while(m_deltaSeconds < g_mininumSecondsPerFrame)
	{
		currentTimeSeconds = MasterClock::GetMasterClock()->GetAbsoluteTimeSeconds();
		m_deltaSeconds = currentTimeSeconds - lastTimeSeconds;
		double timeRemaining = g_mininumSecondsPerFrame - m_deltaSeconds;
		if(timeRemaining > .002)
			Sleep(1);
		g_frameCount++;
	}  
	if(m_deltaSeconds > g_maximumSecondsPerFrame)
		m_deltaSeconds = g_maximumSecondsPerFrame;
	lastTimeSeconds = currentTimeSeconds;
	
	MasterClock::GetMasterClock()->UpdateClock(m_deltaSeconds);
	

	std::stringstream text;
	text << "FPS: " << std::fixed << std::setprecision(2) << (1.0 / m_deltaSeconds);
	std::string fpsString = text.str();
	DeveloperLog::GetFPSFont()->m_myText = fpsString;


}
void TheApp::UpdateWorld()
{	 
	//Update Game Time
	UpdateGameTime();

	
	//Console Display
	if(!g_displayConsole)	
	{
		//Update Camera
		if (g_use3DCamera)
		{
			g_masterCamera->UpdateCamera(m_deltaSeconds);
			g_masterCamera->CreateProjPers(g_masterCamera->m_projPersMatrix, 60.0f, (16.0f / 9.0f), 0.1f, 15000.0f);
			g_masterCamera->CreateViewMatrix(g_masterCamera->m_viewMatrix);
		}
	}
	else
	{
		DeveloperLog::GetDeveloperLog()->CursorBlink(m_deltaSeconds);
		DeveloperLog::DetectInputForConsole();
	}


	//FPS Display
	if (g_displayFPS)
		DeveloperLog::UpdateDeveloperLogShader(DeveloperLog::GetFPSShader(), DeveloperLog::GetFPSFont(), Vector2(5.0f, m_renderer->GetDisplayHeight() * .96f), Rgba::HEALINGBLUE);


	//Info Table Display
	if (g_displayInfoTable)
	{
		DeveloperLog::UpdateInfoTextShader(DeveloperLog::GetInfoTableShader(), DeveloperLog::GetInfoTableFont(), Vector2(5.0f, m_renderer->GetDisplayHeight() * .90f));
		DeveloperLog::GetInfoTableFont()->UpdateTextLinesLifetime(m_deltaSeconds);
	}


	//World Display
	if(m_world)
		m_world->Update(m_deltaSeconds);	
}



//===================================================================================================
void TheApp::RenderWorld()
{
	m_renderer->RenderBasicRenderingFunctions3D(g_clearBufferColor);

	//Draw Axes
	if (g_drawAxes)
	{
		OpenGLRenderer::DrawDebugGridShader();
		OpenGLRenderer::DrawDebugCoordinateShader();
	}

	//Render World
	if (g_renderWorld)
		m_world->Render(m_renderer);

	//Render Console, Log, Cursor
	if (g_displayConsole)
	{
		DeveloperLog::GetDeveloperConsole()->RenderCursor(m_renderer);
		DeveloperLog::GetDeveloperConsoleShader()->RenderBasicShader(Matrix4x4(), g_masterCamera->m_orthoMatrix, DeveloperLog::GetDeveloperConsole()->m_vertCount);
		DeveloperLog::GetDeveloperLogShader()->RenderBasicShader(Matrix4x4(), g_masterCamera->m_orthoMatrix, DeveloperLog::GetDeveloperLog()->m_vertCount);
	}

	//Render FPS
	if (g_displayFPS)
		DeveloperLog::GetFPSShader()->RenderBasicShader(Matrix4x4(), g_masterCamera->m_orthoMatrix, DeveloperLog::GetFPSFont()->m_vertCount);
	
	//Render InfoTable
	if(g_displayInfoTable)
		DeveloperLog::GetInfoTableShader()->RenderBasicShader(Matrix4x4(), g_masterCamera->m_orthoMatrix, DeveloperLog::GetInfoTableFont()->m_vertCount);

	//Render PolyonLiens
	if (g_polygonLines)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_renderer->SwapBuffers();
}