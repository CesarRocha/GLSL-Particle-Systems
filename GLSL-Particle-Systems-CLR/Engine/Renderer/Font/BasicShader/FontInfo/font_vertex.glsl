#version 330 core

uniform float gTime;
uniform mat4 gView;
uniform mat4 gProj;
   
in vec3 inPosition;
in vec4 inColor;
in vec2 inUV;

out vec4 passColor;
out vec3 passPos;
out vec2 passUV;

void main( void )
{
	mat4 worldToViewTransform = gView;
	mat4 viewToClipTransform = gProj;
	vec4 worldPos = vec4( inPosition, 1.0 );
	vec4 viewPos = worldToViewTransform * worldPos;
	vec4 clipPos = viewToClipTransform * viewPos;


	passColor = inColor;
	passUV = inUV;
	passPos = clipPos.xyz;
	gl_Position = clipPos;
}