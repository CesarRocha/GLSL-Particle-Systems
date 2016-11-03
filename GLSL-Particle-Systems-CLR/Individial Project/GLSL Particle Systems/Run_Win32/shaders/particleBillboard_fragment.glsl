#version 330 core

//Textures
uniform sampler2D gTexDiffuse;
uniform sampler2D gTexNormal; 
uniform int		  gPolygonMode;

//From Geometry
in vec3 passPosition;
in vec2 passUV0;
in vec4 passColor;


//Out
out vec4 outColor;



//////////////////////////////////////////////////////////////////////////
void main (void)
{
	if(gPolygonMode == 1)
	{ 
		outColor = passColor;
		return;
	}
	vec4 diffuseTexel = texture( gTexDiffuse, passUV0);
	vec4 normalTexel  = texture(  gTexNormal, passUV0 );


	vec3  finalColor = (diffuseTexel.rgb * passColor.rgb);
	float finalAlpha = (diffuseTexel.a * passColor.a); 
	
	outColor = vec4( finalColor, finalAlpha );
}