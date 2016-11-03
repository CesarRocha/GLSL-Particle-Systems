#version 330 core

uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel;  
uniform float	gSize;
////////////////////////////////////////////////////////////////
//
in vec3			inPosition;
in vec3			inOrientation;
in vec3			inVelocity;
in vec4			inColor;
in float		inSize;
in vec2			inUV0;
in vec2			inUV0Max;
//
////////////////////////////////////////////////////////////////
//
out Vertex
{
	vec3  orientation;
	vec3  velocity;
	vec2  uv;
	vec2  uvMax;
	vec4  color;
	float size;
}   vertex;
//
////////////////////////////////////////////////////////////////

void main()
{

  
	gl_Position = gView * gModel * vec4(inPosition, 1.0);
	
	vertex.color		= inColor;
	vertex.velocity		= inVelocity;
	vertex.orientation	= inOrientation;
	vertex.uv			= inUV0;
	vertex.uvMax		= inUV0Max;
	vertex.size			= inSize;
}