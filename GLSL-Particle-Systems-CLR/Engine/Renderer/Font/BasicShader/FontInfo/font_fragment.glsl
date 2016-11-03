#version 330 core

uniform float gTime;
uniform sampler2D gTexture;

in vec4 passColor;
in vec3 passPos;
in vec2 passUV;

out vec4 outColor;

const float cPI = 3.1456;

void main( void )
{
	vec4 texColor = texture( gTexture, passUV );
	vec4 color =  vec4(texColor.r); 
	color *= passColor;
	outColor = color;
}