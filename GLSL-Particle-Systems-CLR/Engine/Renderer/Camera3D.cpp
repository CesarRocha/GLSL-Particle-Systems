//================================================================
//                     Camera3D.cpp
//================================================================

#pragma once
#include "Engine/Renderer/Camera3D.hpp"
#include "Engine\EngineCommon\EngineCommon.hpp"
#include <Windows.h>
Camera3D* g_masterCamera = new Camera3D();

void Camera3D::InitializeCamera(float initialRoll_X, float initialPitch_Y, float intialYaw_Z, InputSystem* inputSystem)
{
	ShowCursor(FALSE);
	m_worldPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_orientation = EulerAngles(initialRoll_X, initialPitch_Y, intialYaw_Z);
	m_inputSystem = inputSystem;
}

void Camera3D::UpdateCamera(double deltaSeconds)
{
	if (m_inputSystem == nullptr)
	{
		MessageBoxA(NULL, "Camera is nullptr. Check if InitializeCamera() has a valid *. Closing Program.", "Camera Error!", MB_OK);
		g_appIsRunning = false;
		return;
	}
	const float degreesPerMouseDelta = 0.04f;
	float moveSpeedBlocksPerSecond = 3.4f;

	float cameraYawDegrees = m_orientation.yawDegreesAboutZ;
	float cameraYawRadians = ConvertDegreesToRadians(cameraYawDegrees);
	Vector3 cameraForwardXY(cos(cameraYawRadians), sin(cameraYawRadians), 0.0f);
	Vector3 cameraLeftXY(-cameraForwardXY.y, cameraForwardXY.x, 0.0f);
	Vector3 cameraMoveVector(0.0f, 0.0f, 0.0f);

	if (m_inputSystem->isKeyDown('W'))
		cameraMoveVector += cameraForwardXY;
	if (m_inputSystem->isKeyDown('S'))
		cameraMoveVector -= cameraForwardXY;
	if (m_inputSystem->isKeyDown('A'))
		cameraMoveVector += cameraLeftXY;
	if (m_inputSystem->isKeyDown('D'))
		cameraMoveVector -= cameraLeftXY;
	if (m_inputSystem->isKeyDown('E'))
		cameraMoveVector.z += 1.f;
	if (m_inputSystem->isKeyDown(VK_SPACE))
		cameraMoveVector.z -= 1.f;
	if (m_inputSystem->isKeyDown(VK_SHIFT))
		moveSpeedBlocksPerSecond = 35.2f;

	cameraMoveVector.Normalize();
	cameraMoveVector *= (moveSpeedBlocksPerSecond * (float)deltaSeconds);
	m_worldPosition += cameraMoveVector;

	POINT centerCursorPos = { 400, 300 };
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	SetCursorPos(centerCursorPos.x, centerCursorPos.y);
	Vector2 mouseDelta((float)cursorPos.x - (float)centerCursorPos.x, (float)cursorPos.y - (float)centerCursorPos.y);

	m_orientation.yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDelta.x);
	m_orientation.pitchDegreesAboutY += (degreesPerMouseDelta * mouseDelta.y);

	cameraMoveVector *= (moveSpeedBlocksPerSecond * (float)deltaSeconds);
	g_masterCamera->m_worldPosition += cameraMoveVector;

	if (m_orientation.pitchDegreesAboutY < -89.f)
		m_orientation.pitchDegreesAboutY = -89.f;
	if (m_orientation.pitchDegreesAboutY >  89.f)
		m_orientation.pitchDegreesAboutY = 89.f;
}

const Vector3 Camera3D::GetForwardVector() const
{
	float cosPitch = cos(m_orientation.pitchDegreesAboutY * DEG2RAD );
	float sinPitch = sin(m_orientation.pitchDegreesAboutY * DEG2RAD );
	float cosYaw   = cos(m_orientation.yawDegreesAboutZ   * DEG2RAD );
	float sinYaw   = sin(m_orientation.yawDegreesAboutZ   * DEG2RAD );
	Vector3 direction(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
	direction.Normalize();
	return direction;
}


const Vector3 Camera3D::GetRightVector() const
{
	Vector3 forwardVector = GetForwardVector();
	Vector3 rightVector = forwardVector.CrossProduct(Vector3::ZUP);
	rightVector.Normalize();
	return rightVector;
}

const Vector3 Camera3D::GetUpVector() const
{
	Vector3 forwardVector = GetForwardVector();
	Vector3 rightVector = GetRightVector();
	Vector3 upVector = rightVector.CrossProduct(forwardVector);
	
	upVector.Normalize();
	return upVector;
}

//################################################################################################//
//##                                                                                            ##//
//################################################################################################//
#include "OpenGLRenderer.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <GL/Gl.h>
void Camera3D::CreateViewMatrix(Matrix4x4& mOut)
{
	Matrix4x4 currentOpenGLMatrix;
	glLoadIdentity();
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );

	mOut.MakeIdentity();
	mOut.RotateDegreesAboutX(-90.0f); //Lean "forward" 90* to put +Z up (was +Y)
	glRotatef( -90.f, 1.0f, 0.0f, 0.0f); //Lean "forward" 90* to put +Z up (was +Y)
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );

	mOut.RotateDegreesAboutZ(90.0f);	//Spin "left" 90* to put +X forward (Was +Y)
	glRotatef( 90.0f, 0.0f, 0.0f, 1.0f); //Spin "left" 90* to put +X forward (Was +Y)
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );
	
	mOut.RotateDegreesAboutX( -m_orientation.rollDegreesAboutX );
	glRotatef( -m_orientation.rollDegreesAboutX,  1.0f, 0.0f, 0.0f);
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );
	
	mOut.RotateDegreesAboutY( -m_orientation.pitchDegreesAboutY );
	glRotatef( -m_orientation.pitchDegreesAboutY, 0.0f, 1.0f, 0.0f);
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );
	
	mOut.RotateDegreesAboutZ( -m_orientation.yawDegreesAboutZ );
	glRotatef( -m_orientation.yawDegreesAboutZ,   0.0f, 0.0f, 1.0f);
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );

	mOut.Translate( -1.0 * m_worldPosition);
	glTranslatef( -m_worldPosition.x, -m_worldPosition.y, -m_worldPosition.z );
	glGetFloatv( GL_MODELVIEW_MATRIX, &currentOpenGLMatrix.m_iBasis.x );

}

//################################################################################################//
//##                                                                                            ##//
//################################################################################################//
//Working Proj as per Eiserloh
void Camera3D::CreateProjPers(Matrix4x4& m, float fovDegreesVertical, float aspect, float zNearDist, float zFarDist)
{
	Matrix4x4 perspectiveProjection;
	float f = 1.0f / tan( .5f * ConvertDegreesToRadians(fovDegreesVertical)) ;
	float s= 1.0f / (zNearDist - zFarDist);

	perspectiveProjection.m_iBasis.x = f / aspect;
	perspectiveProjection.m_jBasis.y = f;
	perspectiveProjection.m_kBasis.z = (zNearDist + zFarDist) * s;
	perspectiveProjection.m_kBasis.w = -1.0f;
	perspectiveProjection.m_translation.z = 2.0f * zNearDist * zFarDist * s;
	perspectiveProjection.m_translation.w = 0.0f;
	m = perspectiveProjection;
}

//Working as per Fonts project
Matrix4x4 Camera3D::CreateOrthoMatrix(float screenWidth, float screenHeight, float zFar, float zNear)
{
	Matrix4x4 out(
		Vector4( 2.0f/screenWidth,              0.0f,                             0.0f,    0.0f ),
		Vector4(             0.0f, 2.0f/screenHeight,                             0.0f,    0.0f ),
		Vector4(             0.0f,              0.0f,              2.0f/(zFar - zNear),    0.0f),
		Vector4(            -1.0f,             -1.0f,  -(zFar + zNear)/ (zFar - zNear),    1.0f));

	m_orthoMatrix = out;
	return out;
}
