#version 330 core

layout (points) in;
layout (line_strip) out;
layout (max_vertices = 256) out;    
   
uniform mat4	gView;
uniform mat4	gProj;
uniform mat4	gModel;  

out vec4 passColor;

void main()
{
	//RED
	gl_Position = gProj * gView * vec4(0.0, 0.0, 0.01, 1.0);
	passColor = vec4(1.0, 0.0, 0.0, 1.0);
	EmitVertex();
	
	gl_Position = gProj * gView * vec4(1.0, 0.0, 0.01, 1.0);
	passColor = vec4(1.0, 0.0, 0.0, 1.0);
	EmitVertex();	
	EndPrimitive();  


	//Green
	gl_Position = gProj * gView * vec4(0.0, 0.0, 0.01, 1.0);
	passColor = vec4(0.0, 1.0, 0.0, 1.0);
	EmitVertex();
	
	gl_Position = gProj * gView * vec4(0.0, 1.0, 0.01, 1.0);
	passColor = vec4(0.0, 1.0, 0.0, 1.0);
	EmitVertex();	
	EndPrimitive();  


	//Blue
	gl_Position = gProj * gView * vec4(0.0, 0.0, 0.01, 1.0);
	passColor = vec4(0.0, 0.0, 1.0, 1.0);
	EmitVertex();
	
	gl_Position = gProj * gView * vec4(0.0, 0.0, 1.01, 1.0);
	passColor = vec4(0.0, 0.0, 1.0, 1.0);
	EmitVertex();	
	EndPrimitive();  



}   