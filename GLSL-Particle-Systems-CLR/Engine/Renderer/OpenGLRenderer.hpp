//================================================================
//                     OpenGLRenderer.hpp
//================================================================
#pragma once
#ifndef _included_OpenGLRenderer_
#define _included_OpenGLRenderer_
#define OPENGL_CHECK_ERRORS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "glext.h"
#include <math.h>
#include <assert.h>
#include <vector>

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Math/3D/Matrix4x4.hpp"
#include "Engine/Math/LineSegment2.hpp"

#include "Engine/Renderer/AnimatedTexture.hpp"
#include "Engine/Math/3D/EulerAngles.hpp"
#include "Engine/Renderer/RGBA.hpp"
#include "Engine/Renderer/vert_t.hpp"


//===================================================================================================
//							class OpenGLRenderer												   ==
//===================================================================================================
class OpenGLRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void	startup(HWND windowHandle);
	void	shutdown();

	//================================================================================================
	//																							  
	//================================================================================================
	void	SwapBuffers();
	void	ClearBuffer(float r = 0.0f, float g = 0.0f, float b = 0.0f);
	void	BeginOpenGL2DTexture(const Texture& texture);
	void	EndOpenGL2DTexture();
	void	pushMatrix();
	void	popMatrix();


	//================================================================================================
	//Accessors																								  
	//================================================================================================
	float	GetDisplayWidth()						const {return m_displayWidth;}
	float	GetDisplayHeight()						const {return m_displayHeight;}

	//================================================================================================
	//Mutators																								  
	//================================================================================================	
	void	SetModelViewMatrixMode();
	void	SetModelViewTranslation(float x, float y, float z=0.0f);
	void	SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ);
	void	SetModelColor(float R, float G, float B, float A);
	void	SetModelColor(Rgba rgba);


	//================================================================================================
	//Camera Methods																								  
	//================================================================================================
	void	SetOrthoGraphicView();
	void	GluPerspective3DPROJECTION(float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth);
	void	GluPerspective3D(float fieldOfViewDegreesVertical, float aspectRatio, float nearDepth, float farDepth);
	void	ApplyCameraTransform( const Vector3& cameraPosition, EulerAngles cameraOrientation );


	//================================================================================================
	//Draw Methods																								  
	//================================================================================================
	void	DrawTriangle2D(const Vector2& v0, const Vector2& v1, const Vector2& v2);
	void	DrawTriangle3D(const Vector3& v0, const Vector3& v1, const Vector3& v2);
	void	DrawPolygon2D(const std::vector <Vector2>& verticies);
	void	DrawPolygon3D(const std::vector <Vector3>& verticies);
	void	DrawHollowPolygon2D(const std::vector <Vector2>& verticies, int thickness);
	void	DrawHollowPolygon3D(const std::vector <Vector3>& verticies, int thickness);
	void	DrawTexturedQuad2D(const Texture& texture, const std::vector<Vector2>& verticies, const std::vector<Vector2>& textureCoordinates);
	void	DrawTexturedQuad3D(const Texture& texture, const std::vector<Vector3>& verticies, const std::vector<Vector2>& textureCoordinates, bool isReadyForVertexes = false);
	void	DrawCircularPolygon(float radius, int sections);

	void	RenderBasicRenderingFunctions2D(Rgba clearBufferColor = Rgba::BLACK);
	void	RenderBasicRenderingFunctions3D(Rgba clearBufferColor = Rgba::BLACK);


	void	DrawDebugCoodinateAxes(float length);
	void	DrawDebugGridFloor();
	void	DrawDebugPoint( Vector3 position, float length, Rgba color);
	void	DrawDebugArrows( Vector3 position, float length, Rgba color);
	void	DrawDebugAABB3( Vector3 mins, Vector3 maxs, int length, Rgba color, Rgba lineColor);
	void    DrawDebugSphere( Vector3 position, float radius, Rgba color);

	static void	CreateDebugGridShader();
	static void	DrawDebugGridShader();
	static void	CreateDebugCoordinateShader();
	static void	DrawDebugCoordinateShader();


	void	circleTable(double **sint,double **cost,const int n);
	void	renderSolidSphere(GLdouble radius, GLint slices, GLint stacks);
	void	renderWireSphere(GLdouble radius, GLint slices, GLint stacks);



protected:
	void	CreateOpenGLRenderingContext();
	void	InitializeAdvancedOpenGLFunctions();
	void	DestroyOpenGLRenderingContext();
	

private:
	HWND	m_windowHandle;
	HDC		m_deviceContext;
	HGLRC   m_renderingContext;
	float   m_displayWidth; 
	float   m_displayHeight;

	float   m_viewMatrixSG[16];
};


extern PFNGLGENBUFFERSPROC				glGenBuffers;
extern PFNGLDELETEBUFFERSPROC			glDeleteBuffers;
extern PFNGLBINDBUFFERPROC				glBindBuffer;
extern PFNGLBUFFERDATAPROC				glBufferData;
extern PFNGLGENERATEMIPMAPPROC			glGenerateMipmap;
extern PFNGLCREATESHADERPROC			glCreateShader;
extern PFNGLSHADERSOURCEPROC			glShaderSource;
extern PFNGLVALIDATEPROGRAMPROC			glValidateProgram;
extern PFNGLCOMPILESHADERPROC			glCompileShader;
extern PFNGLDELETESHADERPROC			glDeleteShader;
extern PFNGLGETSHADERIVPROC				glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;

extern PFNGLCREATEPROGRAMPROC			glCreateProgram;
extern PFNGLATTACHSHADERPROC			glAttachShader;
extern PFNGLDETACHSHADERPROC			glDetachShader;
extern PFNGLLINKPROGRAMPROC				glLinkProgram;
extern PFNGLDELETEPROGRAMPROC			glDeleteProgram;
extern PFNGLPROGRAMPARAMETERIPROC		glProgramParameteri;
extern PFNGLGETPROGRAMIVPROC			glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog;

extern PFNGLUSEPROGRAMPROC				glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC		glGetUniformLocation;
extern PFNGLGETUNIFORMINDICESPROC		glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMPROC 		glGetActiveUniform;

extern PFNGLUNIFORM1FVPROC				glUniform1fv;
extern PFNGLUNIFORM2FPROC				glUniform2f;
extern PFNGLUNIFORM3FPROC				glUniform3f;
extern PFNGLUNIFORM4FPROC				glUniform4f;
extern PFNGLUNIFORM3UIPROC				glUniform3ui;
extern PFNGLUNIFORM4UIPROC				glUniform4ui;
extern PFNGLUNIFORM1IVPROC				glUniform1iv;
extern PFNGLUNIFORMMATRIX4FVPROC		glUniformMatrix4fv;

extern PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC		glDeleteVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC			glBindVertexArray;
extern PFNGLGETATTRIBLOCATIONPROC		glGetAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer;
extern PFNGLBINDATTRIBLOCATIONPROC		glBindAttribLocation;
extern PFNGLACTIVETEXTUREPROC			glActiveTexture;

extern PFNGLBINDSAMPLERPROC				glBindSampler;
extern PFNGLDELETESAMPLERSPROC			glDeleteSamplers;
extern PFNGLGENSAMPLERSPROC				glGenSamplers;
extern PFNGLSAMPLERPARAMETERIPROC		glSamplerParameteri;

extern PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer;
extern PFNGLBLITFRAMEBUFFERPROC			glBlitFramebuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatus;
extern PFNGLDELETEFRAMEBUFFERSPROC		glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTUREPROC		glFramebufferTexture;
extern PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC		glBindFramebufferEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC	glCheckFramebufferStatusEXT;
extern PFNGLGETACTIVEATTRIBPROC			glGetActiveAttrib;
#endif
