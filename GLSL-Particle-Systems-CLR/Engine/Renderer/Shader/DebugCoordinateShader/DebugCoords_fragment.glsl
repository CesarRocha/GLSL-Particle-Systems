#version 330 core

out vec4 outColor;
in  vec4 passColor;

void main (void)
{
	outColor = vec4( passColor.rgb, 1.0 );
}