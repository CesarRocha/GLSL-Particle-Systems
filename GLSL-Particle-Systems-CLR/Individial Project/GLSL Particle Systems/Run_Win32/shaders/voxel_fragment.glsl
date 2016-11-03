#version 330 core

//Textures
uniform sampler2D gTexDiffuse;
uniform sampler2D gTexNormal;


//From Geometry Shader
in vec3 passPosition;
in vec2 passUV0;
in vec4 passColor;


//Material
uniform vec4		gMaterialAmbientColor;
uniform float		gMaterialAmbientPower;
uniform float		gMaterialShininess;

//Fog
uniform float		gFogStart;
uniform float		gFogEnd;
uniform vec3		gFogColor;


//Camera
uniform vec3		gCameraPosition;


//Verts
in float	passDistToCamera;


//Light
uniform int gLightCount;
const	int numberOfLights = 16;
struct LightOutput
{
	vec3	LightIntensity;
	vec3	LightVector;
};
uniform struct Light
{
	vec3	gLightPosition;
	vec4	gLightColor;
	float   gLightIntensity;
	vec3	gLightDirection;
	
	float	gLightAmbient;
	float	gLightWeight;
	float	gLightAngle;
	float	gLightSpotPower;
	float	gLightMinSf;
	float	gLightMinIntensity;
	float	gLightMaxIntensity;
}			lights[numberOfLights];


//Out
out vec4 outColor;



//////////////////////////////////////////////////////////////////////////
LightOutput ComputeALight(const Light light, const vec3 normal)
{
		LightOutput outLight;
		
		vec3  Lvector = light.gLightPosition - passPosition;
		float dist = length(Lvector);

		Lvector = light.gLightWeight * (Lvector / dist) - (1-light.gLightWeight) * light.gLightDirection;

		float Sa = dot( -Lvector, light.gLightDirection);
		float Sc = step(light.gLightAngle, Sa);
		float Sf = Sc * max( Sa, light.gLightMinSf);
		float power =  light.gLightIntensity;
		vec3  Lcolor = light.gLightColor.xyz;
		float Li = clamp(power/ dist * Sf, light.gLightMinIntensity, light.gLightMaxIntensity);
		
		outLight.LightIntensity = Li * Lcolor;
		outLight.LightVector = Lvector;
		return outLight;
}

////////////////////////////////////////////////////////////////////
void main (void)
{
	//vec3 normal		= vec3(.5, .5, 1.0);
	vec4 diffuseTexel = texture( gTexDiffuse, passUV0);
	vec4 normalTexel  = texture( gTexNormal,  passUV0 );
	//outColor = vec4( normalTexel.xyz, 1.0 );
	//return;

//Ambient
   	vec3  ambient		= (gMaterialAmbientColor * gMaterialAmbientPower).rgb;
	vec3  final_color	= (ambient * diffuseTexel.rgb * passColor.rgb);
	float final_alpha	= (diffuseTexel.a * passColor.a); 


//Lights
	vec3 diffuse_power;
	for( int i = 0; i < gLightCount; i++)
	{
		vec3 surfaceToLight  = lights[i].gLightPosition  - passPosition;
	
		LightOutput outLight = ComputeALight( lights[i], normalTexel.xyz );
	
		diffuse_power += outLight.LightIntensity * dot(normalTexel.rgb, outLight.LightVector);
		//spec_power += ComputeSpecularEnhanced(surfaceToLight, surfaceToCamera, lights[i], normalTexel.rgb);
	}
	
	vec3 surfaceToLight  = lights[0].gLightPosition  - passPosition;
	surfaceToLight = normalize(surfaceToLight);

	//outColor = vec4(surfaceToLight, 1.0);
	//return;

//Clamp and add  
	diffuse_power = clamp( diffuse_power, vec3(0.0f), vec3(1.0f) );
	final_color += (diffuse_power * diffuseTexel.rgb);
	// + (spec_power * specular);


//Fog
	float fogFactor = (gFogEnd - passDistToCamera)/(gFogEnd - gFogStart);
	fogFactor		= clamp( fogFactor, 0.0, 1.0 );
	final_color		= mix(gFogColor, final_color, fogFactor); 
	final_color		= clamp(final_color, 0.0f, 1.0f);
	


//Return
	//outColor = vec4(1);
	outColor = vec4( final_color, final_alpha );
	return;
}