#version 330 core

uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel;  


////////////////////////////////////////////////////////////////
in vec3		inPosition;
in vec2		inUV0;
in vec2		inUV02;
in vec2		inUV03;
in vec3		inNormal; 
in vec3		inTangent; 
in vec3		inBiTangent; 
in vec4		inColor;
////////////////////////////////////////////////////////////////
out BaseVoxel
{
	vec3  VPosition;
	vec2  VUV0;
	vec2  VUV02;
	vec2  VUV03;
	vec4  VColor;
}	baseVoxel;

out vec3 passPosition;
out vec2 passUV0;		
out vec2 passUV02;	
out vec2 passuv03;	
out vec4 passColor;	
////////////////////////////////////////////////////////////////
void main()
{

	passPosition = inPosition;
	//For Geometry Shader
	baseVoxel.VPosition		= inPosition;
	baseVoxel.VUV0			= inUV0;
	baseVoxel.VUV02			= inUV02;
    baseVoxel.VUV03			= inUV03;
	baseVoxel.VColor		= inColor;

	passUV0			= inUV0;
	passUV02		= inUV02;
	passuv03		= inUV03;
	passColor		= inColor;

	gl_Position				= gProj * gView * gModel * vec4(inPosition, 1.0 );

}


	//Orginal voxels distance to camera
	//passDistToCamera		= gl_Position.w;
    //baseVoxel.VNormal		= (gModel * vec4(inNormal, 0.0)).xyz;
    //baseVoxel.VTangent	= (gModel * vec4(inTangent, 0.0)).xyz;
	//baseVoxel.VBiTangent	= (gModel * vec4(inBiTangent, 0.0)).xyz;
	//baseVoxel.VDistToCamera = gl_Position.w;
