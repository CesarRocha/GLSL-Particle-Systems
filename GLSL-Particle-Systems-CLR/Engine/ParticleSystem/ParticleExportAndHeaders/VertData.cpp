#include "VertData.hpp"


//================================================================
//              class VertData                               == 
//================================================================
VertData::VertData()
	: size(1.0f)
	, size2(Vector2(1.0f, 1.0f))
	, color(Rgba::WHITE)
	, positionX(Vector2::ZERO)
	, positionY(Vector2::ZERO)
	, positionZ(Vector2::ZERO)
	, velocityX(Vector2::ZERO)
	, velocityY(Vector2::ZERO)
	, velocityZ(Vector2::ZERO)
	, orientationX(Vector2::ZERO)
	, orientationY(Vector2::ZERO)
	, orientationZ(Vector2::ZERO)
	, normalizeVelocity(false)
{ }
void VertData::PopulateXMLWithVertData(XMLNode& newVertDataFile)
{
	std::string string;
	//"size");
	//string = MakeComplexString("%f", size);
	string = MakeComplexString("%f ~ %f", size2.x, size2.y);
	newVertDataFile.addAttribute("size", string.c_str());

	//"color");
	Vector4 converted = Rgba::RgbaToVector4(color);
	string = MakeComplexString("%i, %i, %i, %i", color.r, color.g, color.b, color.a);
	newVertDataFile.addAttribute("color", string.c_str());

	//"positionX");
	string = MakeComplexString("%f ~ %f", positionX.x, positionX.y);
	newVertDataFile.addAttribute("positionX", string.c_str());

	//"positionY");
	string = MakeComplexString("%f ~ %f", positionY.x, positionY.y);
	newVertDataFile.addAttribute("positionY", string.c_str());

	//"positionZ");
	string = MakeComplexString("%f ~ %f", positionZ.x, positionZ.y);
	newVertDataFile.addAttribute("positionZ", string.c_str());

	//"velocityX");
	string = MakeComplexString("%f ~ %f", velocityX.x, velocityX.y);
	newVertDataFile.addAttribute("velocityX", string.c_str());

	//"velocityY");
	string = MakeComplexString("%f ~ %f", velocityY.x, velocityY.y);
	newVertDataFile.addAttribute("velocityY", string.c_str());

	//"velocityZ");
	string = MakeComplexString("%f ~ %f", velocityZ.x, velocityZ.y);
	newVertDataFile.addAttribute("velocityZ", string.c_str());
	//
	//"orientationX");
	string = MakeComplexString("%f ~ %f", orientationX.x, orientationX.y);
	newVertDataFile.addAttribute("orientationX", string.c_str());

	//"orientationY");
	string = MakeComplexString("%f ~ %f", orientationY.x, orientationY.y);
	newVertDataFile.addAttribute("orientationY", string.c_str());

	//"orientationZ");
	string = MakeComplexString("%f ~ %f", orientationZ.x, orientationZ.y);
	newVertDataFile.addAttribute("orientationZ", string.c_str());

	//"normalizeVelocity"
	string = MakeComplexString("%s", normalizeVelocity);
	if (strcmp("true", string.c_str()) == 0)
		newVertDataFile.addAttribute("normalizeVelocity", "true");
	else
		newVertDataFile.addAttribute("normalizeVelocity", "false");



}