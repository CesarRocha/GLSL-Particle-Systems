//================================================================
//                     RGBA.cpp
//================================================================
#include "RGBA.hpp"
#include "Engine\Math\3D\Matrix4x4.hpp"
static const float percent = 1.0f / 255;
//===================================================================================================
Rgba::Rgba()
	: r(255)
	, g(255)
	, b(255)
	, a(255)
{
}
Rgba::Rgba(float r, float g, float b)
	: r((unsigned char)(r * 255.0f))
	, g((unsigned char)(g * 255.0f))
	, b((unsigned char)(b * 255.0f))
	, a(255)
{
}
Rgba::Rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	: r(r)
	, g(g)
	, b(b)
	, a(a) 
{
}
Rgba::Rgba(unsigned char r, unsigned char g, unsigned char b)
	: r(r)
	, g(g)
	, b(b)
	, a(255) 
{
}


//===================================================================================================
Vector4 Rgba::RgbaToVector4(Rgba rgbaToVector4)
{
	float outR = percent * rgbaToVector4.r;
	float outG = percent * rgbaToVector4.g;
	float outB = percent * rgbaToVector4.b;
	float outA = percent * rgbaToVector4.a;

	return Vector4(outR, outG, outB, outA);
}


//===================================================================================================
Vector3 Rgba::RgbaToVector3(Rgba rgbaToVector3)
{
	float outR = percent * rgbaToVector3.r;
	float outG = percent * rgbaToVector3.g;
	float outB = percent * rgbaToVector3.b;

	return Vector3(outR, outG, outB);
}


//===================================================================================================
Rgba Rgba::operator * (float scale)
{
	float rr = (float)r;
	float gg = (float)g;
	float bb = (float)b;
	float aa = (float)a;
	rr = (rr * (scale));
	gg = (gg * (scale));
	bb = (bb * (scale));
	aa = (aa * (scale));
	r = (unsigned char)(rr);
	g = (unsigned char)(gg);
	b = (unsigned char)(bb);
	a = (unsigned char)(aa);
	return Rgba(r, g, b, a);
}



//===================================================================================================
const Rgba Rgba::RED	((unsigned char)255,  0,  0,255);
const Rgba Rgba::GREEN	((unsigned char)  0,255,  0,255);
const Rgba Rgba::BLUE	((unsigned char)  0,  0,255,255);
const Rgba Rgba::YELLOW	((unsigned char)255,255,  0,255);
const Rgba Rgba::WHITE	((unsigned char)255,255,255,255);
const Rgba Rgba::BLACK	((unsigned char)  0,  0,  0,255);
const Rgba Rgba::ORANGE ((unsigned char)255, 40,  0,255);

const Rgba Rgba::DEFAULT_TEXTURE_COLOR((unsigned char)255,255,255,255);
const Rgba Rgba::GREY((unsigned char)64,64,64,255);
const Rgba Rgba::PINK((unsigned char)255,111,193,255);
const Rgba Rgba::BLOODRED((unsigned char)255,40,40,255);
const Rgba Rgba::HALFRED((unsigned char)128,  0,  0,255);
const Rgba Rgba::QUARTERRED((unsigned char)64,  0,  0,255);
const Rgba Rgba::LIGHTBLOODRED((unsigned char)255,128,128,255);
const Rgba Rgba::BRIGHTBLUE((unsigned char)123, 168, 255, 255);
const Rgba Rgba::HEALINGBLUE((unsigned char)0, 233, 218, 255);
const Rgba Rgba::LIGHTBROWN((unsigned char)160, 122, 92, 255);
const Rgba Rgba::BROWN((unsigned char)106, 66, 44, 255);
const Rgba Rgba::HALFGREEN((unsigned char)  0,128,  0,255);
const Rgba Rgba::DARKBLUE((unsigned char) 0, 70, 165, 255);
const Rgba Rgba::SKYBLUE((unsigned char)125, 206, 250, 255);
const Rgba Rgba::SUNLIGHTYELLOW((unsigned char)252, 212, 64);