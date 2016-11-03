#version 330 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 8) out;    
   
uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel;  
uniform float	gSize;
uniform int		  gPolygonMode;
////////////////////////////////////////////////////////////////
//
in Vertex
{
	vec3  orientation;
	vec3  velocity;
	vec2  uv;
	vec2  uvMax;
	vec4  color;
	float size;
}   vertex[];
//
////////////////////////////////////////////////////////////////
//
out vec3		passPosition;
out vec2		passUV0;
out vec4		passColor;
//
////////////////////////////////////////////////////////////////
void main()
{
	vec4  center       = gl_in[0].gl_Position;
	float particleSize = vertex[0].size;









    vec2 BL = vec2(-1, -1);
    vec2 BR = vec2( 1, -1);
    vec2 TR = vec2( 1,  1);
    vec2 TL = vec2(-1,  1);
	    
	
	vec2 BLu = normalize(BL);
	vec2 BRu = normalize(BR);
	vec2 TRu = normalize(TR);
	vec2 TLu = normalize(TL);

    
	vec2  incomingVelocity = vec2(vertex[0].velocity.x, vertex[0].velocity.y);
	//vec2  incomingVelocity = vec2( 3.0, 1.0);

	vec2 BLo = BL + abs( dot( BLu, incomingVelocity)) * BLu;
	vec2 BRo = BR + abs( dot( BRu, incomingVelocity)) * BRu;
	vec2 TRo = TR + abs( dot( TRu, incomingVelocity)) * TRu;
	vec2 TLo = TL + abs( dot( TLu, incomingVelocity)) * TLu;



//Initial Values
	vec2 vUp    = vec2(0.0, 0.5) * particleSize;
	vec2 vRight = vec2(0.5, 0.0) * particleSize;
////Make Rotation Matrix from angle
	float angle = atan(vertex[0].orientation.y, vertex[0].orientation.x);//  0.0;
	mat2 rotationMatrix = mat2
	(
		cos(angle), -sin(angle), 
		sin(angle),  cos(angle)    
	);
//Rotate the adjustment points
	vec2 vRightNew = vRight * rotationMatrix;
	vec2 vUpNew    = vUp    * rotationMatrix;
//ORIGINAL WOKRING QUAD WITH ROTATION
	vec2 BLVert  = center.xy - vRightNew - vUpNew; // Get bottom left vertex
	vec2 TLVert  = center.xy - vRightNew + vUpNew; // Get top left vertex
	vec2 BRVert  = center.xy + vRightNew - vUpNew; // Get bottom right vertex
	vec2 TRVert  = center.xy + vRightNew + vUpNew; // Get top right vertex	


	//BLVert  = center.xy + BLo; // Offsetting verts based on velocity
	//BRVert  = center.xy + BRo; // Offsetting verts based on velocity
	//TRVert  = center.xy + TRo; // Offsetting verts based on velocity
	//TLVert  = center.xy + TLo; // Offsetting verts based on velocity

//Bottom Left Vert to Fragment
	gl_Position = gProj * vec4(BLVert, center.zw); 
	passUV0.x	= vertex[0].uv.x;
	passUV0.y	= vertex[0].uvMax.y;
	passColor	= vertex[0].color;
	EmitVertex();  
//Top Left Vert to Fragment
	gl_Position = gProj * vec4(TLVert, center.zw); 
	passUV0.x	= vertex[0].uv.x;
	passUV0.y	= vertex[0].uv.y;
	passColor	= vertex[0].color;
	EmitVertex();  
//Bottom Right Vert to Fragment
	gl_Position = gProj * vec4(BRVert, center.zw); 
	passUV0.x	= vertex[0].uvMax.x;
	passUV0.y	= vertex[0].uvMax.y;
	passColor	= vertex[0].color;
	EmitVertex();  
//Top Right Vert to Fragment
	gl_Position = gProj * vec4(TRVert, center.zw); 
	passUV0.x	= vertex[0].uvMax.x;
	passUV0.y	= vertex[0].uv.y;
	passColor	= vertex[0].color;
	EmitVertex();  
	EndPrimitive(); 



//For Debug Purposes
	if(gPolygonMode == 1)
	{
		gl_Position = gProj * vec4(BLVert, center.zw); 
		passUV0.x	= 0;
		passUV0.y	= 0;
		passColor	= vec4(1, 0, 0, 1);
		EmitVertex();  	
	//Top Left Vert to Fragment
		gl_Position = gProj * vec4(TLVert, center.zw); 
		passUV0.x	= 0;
		passUV0.y	= 0;
		passColor	= vec4(0, 0, 1, 1);
		EmitVertex();  
	//Bottom Right Vert to Fragment
		gl_Position = gProj * vec4(BRVert, center.zw); 
		passUV0.x	= 0;
		passUV0.y	= 0;
		passColor	= vec4(0, 1, 0, 1);
		EmitVertex();  
	//Top Right Vert to Fragment
		gl_Position = gProj * vec4(TRVert, center.zw); 
		passUV0.x	= 0;
		passUV0.y	= 0;
		passColor	= vec4(1, 1, 1, 1);
		EmitVertex();  
		EndPrimitive(); 
	}
   
}   