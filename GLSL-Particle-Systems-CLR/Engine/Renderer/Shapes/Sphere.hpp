//================================================================
//                     Sphere.hpp
//================================================================
#pragma once
#ifndef INCLUDE_SPHERE
#define INCLUDE_SPHERE
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include <math.h>

#define X .525731112119133606f 
#define Z .850650808352039932f

static GLfloat vdata[12][3] = {    
	{-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},    
	{0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0, -Z, -X},    
	{Z, X, 0.0f}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f} 
};
static GLuint tindices[20][3] = { 
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
	{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };


class XIcoSphere
{
public: 
	XIcoSphere();
	void Normalize(GLfloat *a);
	void DrawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r);
	void DrawIcoSphere(int ndiv, float radius=1.0);
	void drawsphere(int ndiv, float radius=1.0);
	void drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r);
	void normalize(GLfloat *a);
};

class Sphere
{
public:
	Sphere();
	void CreateSphere(int slices, int wedges, int radius);
	vert_t m_sphereVerts;
};

#endif