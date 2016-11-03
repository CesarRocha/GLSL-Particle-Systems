#version 330 core

layout (points) in;
layout (line_strip) out;
layout (max_vertices = 256) out;    
   
uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel;  

void main()
{
	for(int XIndex = -25; XIndex <= 25; XIndex++)
	{
		gl_Position = gProj * gView * vec4(XIndex,  -25.0, 0.0, 1.0);
		EmitVertex();
	
		gl_Position = gProj * gView * vec4(XIndex,  25.0, 0.0, 1.0);
		EmitVertex();
	
		EndPrimitive();  
	}

	for(int YIndex = -25; YIndex <= 25; YIndex++)
	{
		gl_Position = gProj * gView * vec4(-25.0, YIndex, 0.0, 1.0);
		EmitVertex();

		gl_Position = gProj * gView * vec4( 25.0,  YIndex, 0.0, 1.0);
		EmitVertex();

		EndPrimitive();  
	}


}   