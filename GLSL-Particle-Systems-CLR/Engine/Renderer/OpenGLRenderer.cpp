#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Renderer/Camera3D.hpp"
#include "Engine/Renderer/Shader/Material.hpp"
#include "Engine/Renderer/Shader/Mesh.hpp"
#include "Engine/Renderer/Shader/MeshRenderer.hpp"
#include "Engine/Renderer/Shader/MaterialDependants/Sampler.hpp"
#include "Engine/Renderer/Shader/MaterialDependants/Shader.hpp"
bool g_makeAProjection = false;
Shader*			debugGridShader;
Sampler*		debugGridSampler;
Material*		debugGridMaterial;
Mesh*			debugGridMesh;
MeshRenderer*	debugGridMeshRenderer;

Shader*			debugCoordsShader;
Sampler*		debugCoordsSampler;
Material*		debugCoordsMaterial;
Mesh*			debugCoordsMesh;
MeshRenderer*	debugCoordsMeshRenderer;



//===================================================================================================
//				class OpenGlRenderer															   ==
//===================================================================================================
OpenGLRenderer::OpenGLRenderer()
	:m_windowHandle(NULL)
	,m_deviceContext(NULL)
	,m_renderingContext(NULL)
	,m_displayHeight(NULL)
	,m_displayWidth(NULL)
{
}
void OpenGLRenderer::startup(HWND windowHandle)
{
	m_windowHandle	=	windowHandle;
	m_deviceContext =	GetDC(windowHandle);
	CreateOpenGLRenderingContext();
	InitializeAdvancedOpenGLFunctions();
}
void OpenGLRenderer::shutdown()
{
}



//===================================================================================================
void OpenGLRenderer::SwapBuffers()
{	
	::SwapBuffers( m_deviceContext );
}
void OpenGLRenderer::ClearBuffer(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void OpenGLRenderer::EndOpenGL2DTexture()
{
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void OpenGLRenderer::BeginOpenGL2DTexture(const Texture& texture)
{
	glEnable( GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );
	glBegin( GL_QUADS );
}
void OpenGLRenderer::pushMatrix()
{
	glPushMatrix();
}
void OpenGLRenderer::popMatrix()
{
	glPopMatrix();
}
void OpenGLRenderer::RenderBasicRenderingFunctions2D(Rgba clearBufferColor)
{
	Vector4 color = Rgba::RgbaToVector4(clearBufferColor);
	ClearBuffer(color.x, color.y, color.z);
	SetOrthoGraphicView();
	GluPerspective3D(70.0f, (16.0f / 9.0f), .1f, 1000.0f);
	ApplyCameraTransform(g_masterCamera->m_worldPosition, g_masterCamera->m_orientation);
}
void OpenGLRenderer::RenderBasicRenderingFunctions3D(Rgba clearBufferColor)
{
	Vector4 color = Rgba::RgbaToVector4(clearBufferColor);
	ClearBuffer(color.x, color.y, color.z);
	SetModelViewMatrixMode();
	GluPerspective3D(70.0f, (16.0f / 9.0f), .1f, 1000.0f);
	ApplyCameraTransform(g_masterCamera->m_worldPosition, g_masterCamera->m_orientation);
}



//===================================================================================================
void OpenGLRenderer::SetModelViewMatrixMode()
{
	glMatrixMode(GL_MODELVIEW);
}
void OpenGLRenderer::SetModelViewTranslation( float x, float y, float z)
{
	glTranslatef(x,y,z);
}
void OpenGLRenderer::SetModelViewRotation(float angleDegrees, float amountAroundX, float amountAroundY, float amountAroundZ)
{
	glRotatef(angleDegrees, amountAroundX, amountAroundY, amountAroundZ);
}
void OpenGLRenderer::SetModelColor(float R, float G, float B, float A)
{
	glColor4f(R,G,B,A);
}
void OpenGLRenderer::SetModelColor(Rgba rgba)
{
	glColor4ub(rgba.r, rgba.g, rgba.b, rgba.a);
}




//===================================================================================================
void OpenGLRenderer::SetOrthoGraphicView()
{
	glLoadIdentity();
	glOrtho( 0.0f, m_displayWidth, 0.0f, m_displayHeight, 0.0f, 0.1f);
}
void OpenGLRenderer::GluPerspective3DPROJECTION(float fieldOfViewDegreesVertical, float aspect, float nearDepth, float farDepth)
{
	g_makeAProjection = true;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	gluPerspective(fieldOfViewDegreesVertical, aspect, nearDepth, farDepth);
}
void OpenGLRenderer::GluPerspective3D(float fieldOfViewDegreesVertical, float aspect, float nearDepth, float farDepth)
{
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_CULL_FACE);
	
	//glCullFace(GL_BACK);
	gluPerspective(fieldOfViewDegreesVertical, aspect, nearDepth, farDepth);
	glDisable(GL_CULL_FACE);
}
void OpenGLRenderer::ApplyCameraTransform( const Vector3& camPos, EulerAngles cameraOrientation  ) 
{
	if(g_makeAProjection)
		glLoadIdentity();

	glRotatef( -90.f, 1.0f, 0.0f, 0.0f); //Lean "forward" 90* to put +Z up (was +Y)
	glRotatef( 90.0f, 0.0f, 0.0f, 1.0f); //Spin "left" 90* to put +X forward (Was +Y)


	glRotatef( -cameraOrientation.rollDegreesAboutX,  1.0f, 0.0f, 0.0f);
	glRotatef( -cameraOrientation.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f);
	glRotatef( -cameraOrientation.yawDegreesAboutZ,   0.0f, 0.0f, 1.0f);

	glTranslatef( -camPos.x, -camPos.y, -camPos.z );
	glGetFloatv ( GL_MODELVIEW_MATRIX, m_viewMatrixSG );
}



//===================================================================================================
void OpenGLRenderer::DrawTriangle2D(const Vector2& v0, const Vector2& v1, const Vector2& v2)
{
	glBegin( GL_TRIANGLES );
		glVertex2f(  v0.x, v0.y); 
		glVertex2f(  v1.x, v1.y); 
		glVertex2f(  v2.x, v2.y);
	glEnd();
}
void OpenGLRenderer::DrawTriangle3D(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	glBegin( GL_TRIANGLES );
		glVertex3f(  v0.x, v0.y, v0.z); 
		glVertex3f(  v1.x, v1.y, v1.z); 
		glVertex3f(  v2.x, v2.y, v2.z);
	glEnd();
}
void OpenGLRenderer::DrawPolygon2D(const std::vector <Vector2>& verticies)
{
	glBegin(GL_POLYGON);
	for (std::vector <Vector2>::const_iterator vertexIter = verticies.begin(); vertexIter != verticies.end(); ++vertexIter)
	{
		const Vector2& vertex = (* vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}
void OpenGLRenderer::DrawPolygon3D(const std::vector <Vector3>& verticies)
{
	glBegin(GL_POLYGON);
	for (std::vector <Vector3>::const_iterator vertexIter = verticies.begin(); vertexIter != verticies.end(); ++vertexIter)
	{
		const Vector3& vertex = (* vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}
void OpenGLRenderer::DrawHollowPolygon2D(const std::vector <Vector2>& verticies, int thickness)
{
	glLineWidth((float)thickness);
	glBegin(GL_LINE_LOOP);
	for (std::vector <Vector2>::const_iterator vertexIter = verticies.begin(); vertexIter != verticies.end(); ++vertexIter)
	{

		const Vector2& vertex = (* vertexIter);
		glVertex2f(vertex.x, vertex.y);
	}
	glEnd();
}
void OpenGLRenderer::DrawHollowPolygon3D(const std::vector <Vector3>& verticies, int thickness)
{
	glLineWidth((float)thickness);
	glBegin(GL_LINE_LOOP);
	for (std::vector <Vector3>::const_iterator vertexIter = verticies.begin(); vertexIter != verticies.end(); ++vertexIter)
	{

		const Vector3& vertex = (* vertexIter);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
}
void OpenGLRenderer::DrawTexturedQuad2D(const Texture& texture, const std::vector<Vector2>& verticies, const std::vector<Vector2>& textureCoordinates)
{
	if ( verticies.size() != 4)
		return ;

	if( verticies.size() != textureCoordinates.size())
		return ;

	glEnable( GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );
	glBegin( GL_QUADS );
	std::vector <Vector2>::const_iterator vertexIter		= verticies.begin();
	std::vector <Vector2>::const_iterator textureCoordIter	= textureCoordinates.begin();

	for ( ; vertexIter != verticies.end(); ++vertexIter, ++textureCoordIter)
	{	
		const Vector2& vertex				=	( *vertexIter);
		const Vector2& textureCoordinate	=	( *textureCoordIter);
		glTexCoord2f( textureCoordinate.x, textureCoordinate.y);
		glVertex2f( vertex.x, vertex.y);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void OpenGLRenderer::DrawTexturedQuad3D(const Texture& texture, const std::vector<Vector3>& verticies, const std::vector<Vector2>& textureCoordinates, bool isReadyForVertexes)
{
	if ( verticies.size() != 4)
		return ;

	if( verticies.size() != textureCoordinates.size())
		return ;

	if(!isReadyForVertexes)
	{
		glEnable( GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, texture.GetPlatformHandle() );
		glBegin( GL_QUADS );
	}
	std::vector <Vector3>::const_iterator vertexIter		= verticies.begin();
	std::vector <Vector2>::const_iterator textureCoordIter	= textureCoordinates.begin();

	for ( ; vertexIter != verticies.end(); ++vertexIter, ++textureCoordIter)
	{	
		const Vector3& vertex				=	( *vertexIter);
		const Vector2& textureCoordinate	=	( *textureCoordIter);
		glTexCoord2f( textureCoordinate.x, textureCoordinate.y);
		glVertex3f( vertex.x, vertex.y, vertex.z);
	}
	if(!isReadyForVertexes)
	{
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}
void OpenGLRenderer::DrawCircularPolygon(float radius, int sections)
{
	float angle = 2.0f * 3.1415f/sections;
	glBegin(GL_POLYGON);
	{
		for (int i= 1; i <= sections; i++)
		{
			float x = radius*cos(i * angle);
			float y = radius*sin(i * angle);
			glVertex2f(x, y);
		}
	}
	glEnd();
}



//===================================================================================================
void OpenGLRenderer::DrawDebugCoodinateAxes(float length)
{
	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
	{
		////////////////////////////////////////////0,0,0
		//Red
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(length, 0.0f, 0.0f);

		//Green
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, length, 0.0f);

		//Blue
		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, length);
		// 		////////////////////////////////////////////16,16,0
		// 		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 0.0f);
		// 		glVertex3f(length, 16.0f, 0.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 0.0f);
		// 		glVertex3f(0.0f, 16-length, 0.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 0.0f);
		// 		glVertex3f(0.0f, 16.0f, length);
		// 		////////////////////////////////////////////16,0,0
		// 		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 0.0f);
		//  		glVertex3f(16-length, 0.0f, 0.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 0.0f);
		// 		glVertex3f(16.0f, length, 0.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 0.0f);
		// 		glVertex3f(16.0f, 0.0f, length);
		// 		////////////////////////////////////////////16,16,0
		// 		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 0.0f);
		// 		glVertex3f(16-length, 16.0f, 0.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 0.0f);
		// 		glVertex3f(16.0f, 16-length, 0.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 0.0f);
		// 		glVertex3f(16.0f, 16.0f, length);
		////////////////////////////////////////////GREY
		//RedGrey
		glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-length *.1f, 0.0f, 0.0f);

		//GreenGrey
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -length*.1f, 0.0f);

		//BlueGrey
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -length*.1f);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////0,0,16
		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 0.0f, 128.0f);
		// 		glVertex3f(length, 0.0f, 128.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 0.0f, 128.0f);
		// 		glVertex3f(0.0f, length, 128.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(0.0f, 0.0f, 128.0f);
		// 		glVertex3f(0.0f, 0.0f, 128-length);
		////////////////////////////////////////////16,16,0
		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 128.0f);
		// 		glVertex3f(length, 16.0f, 128.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 128.0f);
		// 		glVertex3f(0.0f, 16-length, 128.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(0.0f, 16.0f, 128.0f);
		// 		glVertex3f(0.0f, 16.0f, 128-length);
		////////////////////////////////////////////16,0,0
		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 128.0f);
		// 		glVertex3f(16-length, 0.0f, 128.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 128.0f);
		// 		glVertex3f(16.0f, length, 128.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(16.0f, 0.0f, 128.0f);
		// 		glVertex3f(16.0f, 0.0f, 128-length);
		////////////////////////////////////////////16,16,0
		//Red
		// 		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 128.0f);
		// 		glVertex3f(16-length, 16.0f, 128.0f);
		// 
		// 		//Green
		// 		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 128.0f);
		// 		glVertex3f(16.0f, 16-length, 128.0f);
		// 
		// 		//Blue
		// 		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		// 		glVertex3f(16.0f, 16.0f, 128.0f);
		// 		glVertex3f(16.0f, 16.0f, 128-length);

		//Color Reset
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	}
	glEnd();
}
void OpenGLRenderer::DrawDebugGridFloor()
{
	glDisable( GL_TEXTURE_2D );
	glLineWidth(2.0f);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
	glBegin(GL_LINES);
	{
		glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
		int size  = 30;
		for(int x = -size; x < size+1; x++)
		{
			glVertex3f((float)x, (float)-size, 0.0f);
			glVertex3f((float)x, (float) size, 0.0f);
		}

		for(int y = -size; y < size+1; y++)
		{
			glVertex3f((float)-size, (float)y, 0.0f);
			glVertex3f((float) size, (float)y, 0.0f);
		}

		//Color Reset
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	glEnd();
	glLineWidth(1.0f);
}
void OpenGLRenderer::DrawDebugPoint( Vector3 position, float length, Rgba color)
{

	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
	{
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex3f(position.x + -length, position.y + 0.0f,    position.z + 0.0f);
		glVertex3f(position.x + length,  position.y + 0.0f,    position.z + 0.0f);
		glVertex3f(position.x + 0.0f,    position.y + -length, position.z + 0.0f);
		glVertex3f(position.x + 0.0f,    position.y + length,  position.z + 0.0f);  
		glVertex3f(position.x + 0.0f,    position.y + 0.0f,    position.z + -length);
		glVertex3f(position.x + 0.0f,    position.y + 0.0f,    position.z + length);

		//Color Reset
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	}
	glEnd();


}
void OpenGLRenderer::DrawDebugArrows( Vector3 position, float length, Rgba color)
{

	glDisable( GL_TEXTURE_2D );
	glBegin(GL_LINES);
	{
		////////////////////////////////////////////
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex3f(        		  position.x, position.y, position.z);
		glVertex3f(        position.x+length, position.y, position.z);

		glVertex3f(        position.x+length,  position.y+          - .15f,           position.z);
		glVertex3f(        position.x+length,  position.y+            .15f,           position.z);

		glVertex3f(        position.x+length,  position.y+            .15f,           position.z);
		glVertex3f( position.x+length + .15f,        position.y,           position.z);

		glVertex3f(position.x+length,           position.y+          -.15f,           position.z);
		glVertex3f( position.x+length + .15f,        position.y,           position.z);

		glVertex3f(               position.x, position.y+length,     position.z+	         - .15f );
		glVertex3f(               position.x, position.y+length,     position.z+	           .15f );

		glVertex3f(               position.x, position.y+length,     position.z+	           .15f );
		glVertex3f(               position.x, position.y+length + .15f,   position.z );

		glVertex3f(               position.x, position.y+length,     position.z+	          -.15f );
		glVertex3f(               position.x, position.y+length + .15f,   position.z );
		///





		glVertex3f(				 position.x,                     position.y,		 position.z);
		glVertex3f(				 position.x,              position.y+length,		 position.z);

		glVertex3f(	position.x+			       .15f,              position.y+length,		 position.z);
		glVertex3f(	position.x+			      -.15f,              position.y+length,		 position.z);

		glVertex3f(	position.x+			       .15f,			  position.y+length,		 position.z);
		glVertex3f(				 position.x,       position.y+length + .15f,		 position.z);

		glVertex3f(	position.x+			      -.15f,			  position.y+length,		 position.z);
		glVertex3f(				 position.x,       position.y+length + .15f,		 position.z);

		glVertex3f(position.x+length       ,					 position.y,	position.z+		 .15f  );
		glVertex3f(position.x+length       ,					 position.y,	position.z+			-.15f  );

		glVertex3f(position.x+length       ,					 position.y,	position.z+			 .15f  );
		glVertex3f(position.x+length + .15f,					 position.y,	   position.z  );

		glVertex3f(position.x+length       ,					 position.y,	position.z+			-.15f  );
		glVertex3f(position.x+length + .15f,					 position.y,	   position.z  );



		///
		glVertex3f(position.x,   position.y,  position.z);
		glVertex3f(position.x,   position.y,  position.z+length);

		glVertex3f(position.x+      .15f,   position.y,  position.z+length );
		glVertex3f(position.x+     -.15f,   position.y,  position.z+length );

		glVertex3f(position.x+      .15f,	 position.y,  position.z+length );
		glVertex3f(position.x,   position.y,  position.z+length + .15f );

		glVertex3f(position.x+		-.15f,   position.y,  position.z+length );
		glVertex3f(position.x,   position.y,  position.z+length + .15f );

		glVertex3f(position.x,   position.y  +    .15f,  position.z+length );
		glVertex3f(position.x,   position.y +    -.15f,  position.z+length );

		glVertex3f(position.x,   position.y +     .15f,  position.z+length );
		glVertex3f(position.x,   position.y,  position.z+length + .15f );

		glVertex3f(position.x,	position.y	+  -.15f,  position.z+length );
		glVertex3f(position.x,   position.y,  position.z+length + .15f );



		//Color Reset
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	}
	glEnd();

}
void OpenGLRenderer::DrawDebugAABB3( Vector3 mins, Vector3 maxs, int size, Rgba color, Rgba lineColor)
{
	glDisable( GL_TEXTURE_2D );


	std::vector<Vector3> vert;

	vert.push_back(Vector3( mins.x, mins.y, mins.z ));
	vert.push_back(Vector3( maxs.x, mins.y, mins.z ));
	vert.push_back(Vector3( maxs.x, mins.y, maxs.z ));
	vert.push_back(Vector3( mins.x, mins.y, maxs.z ));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();

	vert.push_back(Vector3( mins.x, mins.y, mins.z ));
	vert.push_back(Vector3( mins.x, maxs.y, mins.z ));
	vert.push_back(Vector3( maxs.x, maxs.y, mins.z ));
	vert.push_back(Vector3( maxs.x, mins.y, mins.z ));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();

	vert.push_back(Vector3( mins.x, maxs.y, mins.z ));
	vert.push_back(Vector3( mins.x, mins.y, mins.z ));
	vert.push_back(Vector3( mins.x, mins.y, maxs.z ));
	vert.push_back(Vector3( mins.x, maxs.y, maxs.z ));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();


	vert.push_back(Vector3( maxs.x, mins.y, mins.z ));
	vert.push_back(Vector3( maxs.x, maxs.y, mins.z ));
	vert.push_back(Vector3( maxs.x, maxs.y, maxs.z ));
	vert.push_back(Vector3( maxs.x, mins.y, maxs.z ));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();

	vert.push_back(Vector3( maxs.x, maxs.y, mins.z ));
	vert.push_back(Vector3( mins.x, maxs.y, mins.z ));
	vert.push_back(Vector3( mins.x, maxs.y, maxs.z ));
	vert.push_back(Vector3( maxs.x, maxs.y, maxs.z ));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();

	vert.push_back(Vector3( mins.x, mins.y, maxs.z));
	vert.push_back(Vector3( maxs.x, mins.y, maxs.z));
	vert.push_back(Vector3( maxs.x, maxs.y, maxs.z));
	vert.push_back(Vector3( mins.x, maxs.y, maxs.z));
	glColor4f(color.r, color.g, color.b, color.a);
	DrawPolygon3D(vert);
	glColor4f(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
	DrawHollowPolygon3D(vert, size);
	vert.clear();

	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}
void OpenGLRenderer::DrawDebugSphere( Vector3 position, float radius, Rgba color)
{
	SetModelColor(color.r, color.g, color.b, color.a);
	renderWireSphere(radius, 20, 20);
	SetModelColor(1.0f, 1.0f, 1.0f, 1.0f);	

}


//===================================================================================================
void OpenGLRenderer::CreateDebugGridShader()
{
	vert_t  originVert[] =
	{
		vert_t(Vector3(0.0f, 0.0f, 0.0f), Rgba::WHITE),
	};

	debugGridMesh		  = new Mesh(originVert, 1);

	debugGridShader		  = Shader::CreateOrGetShader("debugGridShader", "../../../Engine/Renderer/Shader/DebugGridShader/DebugGrid", "../../../Engine/Renderer/Shader/DebugGridShader/DebugGrid", "../../../Engine/Renderer/Shader/DebugGridShader/DebugGrid", GEOPOINT);
	debugGridSampler	  = new Sampler();
	debugGridMaterial	  = Material::CreateOrGetMaterial("debugGridMaterial", debugGridSampler);
	debugGridMeshRenderer = MeshRenderer::CreateOrGetMeshRenderer("debugGridMeshRenderer", debugGridMesh, debugGridMaterial, debugGridShader);
}
void OpenGLRenderer::DrawDebugGridShader()
{
	debugGridMeshRenderer->Render(g_masterCamera->m_worldPosition);
}
void OpenGLRenderer::CreateDebugCoordinateShader()
{
	vert_t  originVert[] =
	{
		vert_t(Vector3(0.0f, 0.0f, 0.0f), Rgba::WHITE),
	};

	debugCoordsMesh = new Mesh(originVert, 1);

	debugCoordsShader		= Shader::CreateOrGetShader("debugCoordsShader", "../../../Engine/Renderer/Shader/DebugCoordinateShader/DebugCoords", "../../../Engine/Renderer/Shader/DebugCoordinateShader/DebugCoords", "../../../Engine/Renderer/Shader/DebugCoordinateShader/DebugCoords", GEOPOINT);
	debugCoordsSampler		= new Sampler();
	debugCoordsMaterial		= Material::CreateOrGetMaterial("debugCoordsMaterial", debugCoordsSampler);
	debugCoordsMeshRenderer = MeshRenderer::CreateOrGetMeshRenderer("debugCoordsMeshRenderer", debugCoordsMesh, debugCoordsMaterial, debugCoordsShader);
}
void OpenGLRenderer::DrawDebugCoordinateShader()
{
	debugCoordsMeshRenderer->Render(g_masterCamera->m_worldPosition);
}


//===================================================================================================
void OpenGLRenderer::CreateOpenGLRenderingContext()
{
	PIXELFORMATDESCRIPTOR pixelFormatDescription ;
	memset( &pixelFormatDescription, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

	pixelFormatDescription.nSize		=	sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormatDescription.cColorBits	=	32 ;
	pixelFormatDescription.nVersion		=	1;
	pixelFormatDescription.dwFlags		=	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ; 

	HWND	myWindowHandle	=	(HWND)(m_windowHandle );
	HDC		hdc				=	GetDC( myWindowHandle );
	UNUSED(hdc);
	int		pixelFormat		=	ChoosePixelFormat( m_deviceContext, &pixelFormatDescription );
	BOOL	didSetFormat	=	SetPixelFormat( m_deviceContext, pixelFormat, &pixelFormatDescription );	

	m_renderingContext		=	wglCreateContext( m_deviceContext );


	if (didSetFormat)
	{
		wglMakeCurrent( m_deviceContext, m_renderingContext ) ;
		float dimensions [ 4 ];
		glGetFloatv( GL_VIEWPORT, dimensions);
		m_displayWidth=		dimensions [2];
		m_displayHeight=	dimensions [3];
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
void OpenGLRenderer::InitializeAdvancedOpenGLFunctions()
{
	glGenBuffers				=	(PFNGLGENBUFFERSPROC)				wglGetProcAddress	( "glGenBuffers" );
	glDeleteBuffers				=   (PFNGLDELETEBUFFERSPROC)			wglGetProcAddress	( "glDeleteBuffers "); 
	glBindBuffer				=	(PFNGLBINDBUFFERPROC)				wglGetProcAddress	( "glBindBuffer" );
	glBufferData				=	(PFNGLBUFFERDATAPROC)				wglGetProcAddress	( "glBufferData" );
	glGenerateMipmap			=   (PFNGLGENERATEMIPMAPPROC)			wglGetProcAddress	( "glGenerateMipmap" );

	glCreateShader				=	(PFNGLCREATESHADERPROC)				wglGetProcAddress	( "glCreateShader" );
	glShaderSource				=	(PFNGLSHADERSOURCEPROC)				wglGetProcAddress	( "glShaderSource" );
	glCompileShader				=	(PFNGLCOMPILESHADERPROC)			wglGetProcAddress	( "glCompileShader" );
	glDeleteShader				=	(PFNGLDELETESHADERPROC)				wglGetProcAddress	( "glDeleteShader" );
	glGetShaderiv				=	(PFNGLGETSHADERIVPROC)				wglGetProcAddress	( "glGetShaderiv"  );
	glGetShaderInfoLog			=	(PFNGLGETSHADERINFOLOGPROC)			wglGetProcAddress	( "glGetShaderInfoLog" );

	glCreateProgram				=	(PFNGLCREATEPROGRAMPROC)			wglGetProcAddress	( "glCreateProgram" );
	glAttachShader				=	(PFNGLATTACHSHADERPROC)				wglGetProcAddress	( "glAttachShader" );
	glDetachShader				=	(PFNGLDETACHSHADERPROC)				wglGetProcAddress	( "glDetachShader" );
	glLinkProgram				=	(PFNGLLINKPROGRAMPROC)				wglGetProcAddress	( "glLinkProgram" );
	glDeleteProgram				=	(PFNGLDELETEPROGRAMPROC)			wglGetProcAddress	( "glDeleteProgram" );
	glProgramParameteri			=	(PFNGLPROGRAMPARAMETERIPROC)		wglGetProcAddress	( "glProgramParameteri" );
	glGetProgramiv				=	(PFNGLGETPROGRAMIVPROC)				wglGetProcAddress	( "glGetProgramiv" );
	glGetProgramInfoLog			=	(PFNGLGETPROGRAMINFOLOGPROC)		wglGetProcAddress	( "glGetProgramInfoLog" );

	glUseProgram				=	(PFNGLUSEPROGRAMPROC)				wglGetProcAddress	( "glUseProgram" );
	glGetUniformLocation		=	(PFNGLGETUNIFORMLOCATIONPROC)		wglGetProcAddress	( "glGetUniformLocation" );
	glGetUniformIndices			=	(PFNGLGETUNIFORMINDICESPROC)		wglGetProcAddress	( "glGetUniformIndices" );
	glGetActiveUniform			=	(PFNGLGETACTIVEUNIFORMPROC)			wglGetProcAddress	( "glGetActiveUniform" );

	glUniform1fv				=	(PFNGLUNIFORM1FVPROC)				wglGetProcAddress	( "glUniform1fv" );
	glUniform2f					=	(PFNGLUNIFORM2FPROC)				wglGetProcAddress	( "glUniform2f");
	glUniform3f					=	(PFNGLUNIFORM3FPROC)				wglGetProcAddress	( "glUniform3f" );
	glUniform4f					=	(PFNGLUNIFORM4FPROC)				wglGetProcAddress	( "glUniform4f" );

	glUniform1iv				=	(PFNGLUNIFORM1IVPROC)				wglGetProcAddress	( "glUniform1iv" );
	glUniformMatrix4fv			=	(PFNGLUNIFORMMATRIX4FVPROC)			wglGetProcAddress	( "glUniformMatrix4fv" );

	glGenVertexArrays			=	(PFNGLGENVERTEXARRAYSPROC)			wglGetProcAddress	( "glGenVertexArrays" );
	glDeleteVertexArrays		=	(PFNGLDELETEVERTEXARRAYSPROC)		wglGetProcAddress	( "glDeleteVertexArrays" );
	glBindVertexArray			=	(PFNGLBINDVERTEXARRAYPROC)			wglGetProcAddress	( "glBindVertexArray" );
	glGetAttribLocation			=	(PFNGLGETATTRIBLOCATIONPROC)		wglGetProcAddress	( "glGetAttribLocation" );
	glEnableVertexAttribArray	=	(PFNGLENABLEVERTEXATTRIBARRAYPROC)	wglGetProcAddress	( "glEnableVertexAttribArray" );
	glVertexAttribPointer		=	(PFNGLVERTEXATTRIBPOINTERPROC)		wglGetProcAddress	( "glVertexAttribPointer" );
	glActiveTexture				=	(PFNGLACTIVETEXTUREPROC)			wglGetProcAddress	( "glActiveTexture" );

	glBindSampler				=	(PFNGLBINDSAMPLERPROC)				wglGetProcAddress	( "glBindSampler" );
	glDeleteSamplers			=	(PFNGLDELETESAMPLERSPROC)			wglGetProcAddress	( "glDeleteSamplers" );
	glGenSamplers				=	(PFNGLGENSAMPLERSPROC)				wglGetProcAddress	( "glGenSamplers" );
	glSamplerParameteri			=	(PFNGLSAMPLERPARAMETERIPROC)		wglGetProcAddress	( "glSamplerParameteri" );

	glBindFramebuffer			=	(PFNGLBINDFRAMEBUFFERPROC)			wglGetProcAddress	( "glBindFramebuffer" );
	glBlitFramebuffer			=	(PFNGLBLITFRAMEBUFFERPROC)			wglGetProcAddress	( "glBlitFramebuffer" );
	glCheckFramebufferStatus	=	(PFNGLCHECKFRAMEBUFFERSTATUSPROC)	wglGetProcAddress	( "glCheckFramebufferStatus" );
	glDeleteFramebuffers		=	(PFNGLDELETEFRAMEBUFFERSPROC)		wglGetProcAddress	( "glDeleteFramebuffers" );
	glFramebufferTexture		=	(PFNGLFRAMEBUFFERTEXTUREPROC)		wglGetProcAddress	( "glFramebufferTexture" );
	glGenFramebuffers			=	(PFNGLGENFRAMEBUFFERSPROC)			wglGetProcAddress	( "glGenFramebuffers" );
	glBindFramebufferEXT		=	(PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress	( "glBindFramebufferEXT" );
	glCheckFramebufferStatusEXT	=	(PFNGLCHECKFRAMEBUFFERSTATUSPROC)	wglGetProcAddress	( "glCheckFramebufferStatusEXT" );

	glGetActiveAttrib			=    (PFNGLGETACTIVEATTRIBPROC)         wglGetProcAddress    (  "glGetActiveAttrib");
}
void OpenGLRenderer::DestroyOpenGLRenderingContext()
{
	wglDeleteContext(m_renderingContext);
}



//===================================================================================================
void OpenGLRenderer::circleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */
    const double angle = 2*PI/(double)n;

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
        //fgError("Failed to allocate memory in circleTable");
    }

    /* Compute cos and sin around the circle */

    for (i=0; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}
void OpenGLRenderer::renderSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks are drawn. */

    double z0,z1;
    double r0,r1;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;
    circleTable(&sint1,&cost1,-slices);
    circleTable(&sint2,&cost2,stacks*2);

    /* The top stack is covered with a triangle fan */

    z0 = 1.0;
    z1 = cost2[1];
    r0 = 0.0;
    r1 = sint2[1];

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,1);
        glVertex3d(0,0,radius);

        for (j=slices; j>=0; j--)
        {       
            glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
            glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
        }

    glEnd();

    /* Cover each stack with a quad strip, except the top and bottom stacks */

    for( i=1; i<stacks-1; i++ )
    {
        z0 = z1; z1 = cost2[i+1];
        r0 = r1; r1 = sint2[i+1];

        glBegin(GL_QUAD_STRIP);

            for(j=0; j<=slices; j++)
            {
                glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
                glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
                glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
                glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
            }

        glEnd();
    }

    /* The bottom stack is covered with a triangle fan */

    z0 = z1;
    r0 = r1;

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,-1);
        glVertex3d(0,0,-radius);

        for (j=0; j<=slices; j++)
        {
            glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
            glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
        }

    glEnd();

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}
void OpenGLRenderer::renderWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks and slices are drawn. */

    double r;
    double x,y,z;

    /* Pre-computed circle */
        
    double *sint1,*cost1;
    double *sint2,*cost2;
    circleTable(&sint1,&cost1,-slices  );
    circleTable(&sint2,&cost2, stacks*2);

    /* Draw a line loop for each stack */

    for (i=1; i<stacks; i++)
    {
        z = cost2[i];
        r = sint2[i];

        glBegin(GL_LINE_LOOP);

            for(j=0; j<=slices; j++)
            {
                x = cost1[j];
                y = sint1[j];

                glNormal3d(x,y,z);
                glVertex3d(x*r*radius,y*r*radius,z*radius);
            }

        glEnd();
    }

    /* Draw a line loop for each slice */

    for (i=0; i<slices; i++)
    {
        glBegin(GL_LINE_STRIP);

            for(j=0; j<=stacks; j++)
            {
                x = cost1[i]*sint2[j];
                y = sint1[i]*sint2[j];
                z = cost2[j];

                glNormal3d(x,y,z);
                glVertex3d(x*radius,y*radius,z*radius);
            }

        glEnd();
    }

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}



//===================================================================================================
PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation = nullptr;
PFNGLVALIDATEPROGRAMPROC			glValidateProgram = nullptr;
PFNGLGENBUFFERSPROC					glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC					glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC					glBufferData = nullptr;
PFNGLGENERATEMIPMAPPROC				glGenerateMipmap = nullptr;
PFNGLCREATESHADERPROC				glCreateShader = nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader = nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource = nullptr;
PFNGLDELETESHADERPROC				glDeleteShader = nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC				glAttachShader = nullptr;
PFNGLDETACHSHADERPROC				glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram = nullptr;
PFNGLDELETEPROGRAMPROC				glDeleteProgram = nullptr;
PFNGLPROGRAMPARAMETERIPROC			glProgramParameteri = nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram = nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation = nullptr;
PFNGLGETUNIFORMINDICESPROC			glGetUniformIndices = nullptr;
PFNGLGETACTIVEUNIFORMPROC 			glGetActiveUniform = nullptr;
PFNGLUNIFORM1FVPROC					glUniform1fv = nullptr;
PFNGLUNIFORM2FPROC					glUniform2f = nullptr;
PFNGLUNIFORM3FPROC					glUniform3f = nullptr;
PFNGLUNIFORM4FPROC					glUniform4f = nullptr;
PFNGLUNIFORM3UIPROC					glUniform3ui = nullptr;
PFNGLUNIFORM4UIPROC					glUniform4ui = nullptr;

PFNGLUNIFORM1IVPROC					glUniform1iv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv = nullptr;
PFNGLGENVERTEXARRAYSPROC			glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC			glBindVertexArray = nullptr;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer = nullptr;
PFNGLACTIVETEXTUREPROC				glActiveTexture = nullptr;
PFNGLBINDSAMPLERPROC				glBindSampler = nullptr;
PFNGLDELETESAMPLERSPROC				glDeleteSamplers = nullptr;
PFNGLGENSAMPLERSPROC				glGenSamplers = nullptr;
PFNGLSAMPLERPARAMETERIPROC			glSamplerParameteri = nullptr;

PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer = nullptr;
PFNGLBLITFRAMEBUFFERPROC			glBlitFramebuffer = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers = nullptr;
PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture = nullptr;
PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebufferEXT = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatusEXT = nullptr;

PFNGLGETACTIVEATTRIBPROC			glGetActiveAttrib = nullptr;

