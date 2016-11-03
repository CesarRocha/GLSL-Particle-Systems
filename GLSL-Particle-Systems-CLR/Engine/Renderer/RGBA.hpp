//================================================================
//                     RGBA.hpp
//================================================================
#pragma once
#ifndef __INCLUDE_RGBA__
#define __INCLUDE_RGBA__

class Vector4;
class Vector3;
class Rgba
{
public:
	Rgba();
	Rgba(float r, float g, float b);
	Rgba(unsigned char r, unsigned char g, unsigned char  b);
	Rgba(unsigned char r, unsigned char g, unsigned char  b, unsigned char a);


	static Vector4 RgbaToVector4(Rgba rgbaToVector4);
	static Vector3 RgbaToVector3(Rgba rgbaToVector3);
	Rgba	operator * (float scale);


	static const Rgba DEFAULT_TEXTURE_COLOR;
	static const Rgba RED;
	static const Rgba GREEN;
	static const Rgba BLUE;
	static const Rgba YELLOW;
	static const Rgba WHITE;
	static const Rgba BLACK;
	static const Rgba ORANGE;


	//Black
	static const Rgba GREY;

	//Red
	static const Rgba BLOODRED;
	static const Rgba HALFRED;
	static const Rgba QUARTERRED;
	static const Rgba LIGHTBLOODRED;
	static const Rgba PINK;

	//Blue
	static const Rgba BRIGHTBLUE;
	static const Rgba HEALINGBLUE;
	static const Rgba DARKBLUE;
	static const Rgba SKYBLUE;

	//Brown
	static const Rgba LIGHTBROWN;
	static const Rgba BROWN;

	//Green
	static const Rgba HALFGREEN;

	//Yellow
	static const Rgba SUNLIGHTYELLOW;

	unsigned char r; 
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

#endif
