//================================================================
//                     sphere.cpp
//================================================================
#include "Engine/Renderer/Shapes/Sphere.hpp"

XIcoSphere::XIcoSphere()
{
}

void XIcoSphere::normalize(GLfloat *a) {
	GLfloat d=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
	a[0]/=d; a[1]/=d; a[2]/=d;
}

void XIcoSphere::drawtri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r) {
	if (div<=0) {
		glNormal3fv(a); glVertex3f(a[0]*r, a[1]*r, a[2]*r);
		glNormal3fv(b); glVertex3f(b[0]*r, b[1]*r, b[2]*r);
		glNormal3fv(c); glVertex3f(c[0]*r, c[1]*r, c[2]*r);
	} else {
		GLfloat ab[3], ac[3], bc[3];
		for (int i=0;i<3;i++) {
			ab[i]=(a[i]+b[i])/2;
			ac[i]=(a[i]+c[i])/2;
			bc[i]=(b[i]+c[i])/2;
		}
		normalize(ab); normalize(ac); normalize(bc);
		drawtri(a, ab, ac, div-1, r);
		drawtri(b, bc, ab, div-1, r);
		drawtri(c, ac, bc, div-1, r);
		drawtri(ab, bc, ac, div-1, r);  //<--Comment this line and sphere looks really cool!
	}  
}

void XIcoSphere::drawsphere(int ndiv, float radius) {
	glBegin(GL_LINES);
	for (int i=0;i<20;i++)
		drawtri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius);
	glEnd();
}


//================================================================
//                     Sphere.cpp
//================================================================

// void Sphere::CreateSphere(int slices, int wedges, int radius)
// {
// 	vert_t sphereVerts;
// 
// 	const double SliceAngle = 2*PI/(double)slices;
// 	const double WedgeAngle = 2*PI/(double)wedges;
// 	for(int s = 0; s < slices; ++s)
// 	{
// 		for(int w = 0; w < wedges; ++w)
// 		{
// 
// 		}
// 
// 	}
// 	m_sphereVerts = sphereVerts;
// }

