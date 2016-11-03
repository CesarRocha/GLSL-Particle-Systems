#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 36) out;    
   
uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel; 

float particleSize = 1.0;
vec3 plane; 
float offset = 0.03125; 

////////////////////////////////////////////////////////////////
in BaseVoxel
{
	vec3  VPosition;
	vec2  VUV0;
	vec2  VUV02;
	vec2  VUV03;
	vec4  VColor;
}	baseVoxel[];
 
 out vec3 passPosition;
 out vec2 passUV0;
 out vec4 passColor;
 out float passDistToCamera;

////////////////////////////////////////////////////////////////
void main()
{
	//passPosition = baseVoxel[0].VPosition;
	vec3 voxelOrigin = baseVoxel[0].VPosition;
	

	////Top Plane
	plane = voxelOrigin.xyz + vec3(0.0, 0.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV02 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV02 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 0.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV02 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV02 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive(); 


	////Bottom Plane
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV03 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(0.0, 0.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV03 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV03 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 0.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV03 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive(); 


	////East Plane
	plane = voxelOrigin.xyz + vec3(1.0, 0.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(1.0, 0.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive();


	/////West Plane
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(0.0, 0.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(0.0, 0.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive();



	
	////North
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(1.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(0.0, 1.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive();



	//South PLane
	plane = voxelOrigin.xyz + vec3(0.0, 0.0,  0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, offset);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	  
	plane = voxelOrigin.xyz + vec3(0.0,  0.0, 1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(0.0, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 0.0, 0.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, offset);
	passColor = baseVoxel[0].VColor;
	passPosition = gl_Position.xyz;
	passDistToCamera = gl_Position.w;
	EmitVertex();  
	
	plane = voxelOrigin.xyz + vec3(1.0, 0.0,  1.0) * particleSize;
	gl_Position = gProj * gView * vec4(plane, 1.0);
	passUV0 = baseVoxel[0].VUV0 + vec2(offset, 0.0);
	passColor = baseVoxel[0].VColor;
	passDistToCamera = gl_Position.w;
	passPosition = gl_Position.xyz;
	EmitVertex();  
	EndPrimitive();


	EndPrimitive(); 
}   