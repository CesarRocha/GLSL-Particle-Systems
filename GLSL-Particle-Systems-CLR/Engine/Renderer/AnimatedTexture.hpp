#pragma once
#ifndef  __included_AnimatedTexture__
#define  __included_AnimatedTexture__
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vector2.hpp"
#include <string>
#include <map>



class AnimatedTexture ;
typedef std::map< std::string, AnimatedTexture* >	AnimatedTextureMap ;
//===================================================================================================
//					class Animated Texture														   ==
//===================================================================================================
class AnimatedTexture : public Texture
{
public:
	AnimatedTexture(const std::string& animatedTexturePath, int numberOfFrames, int cellsWide, int cellsHigh);
	virtual ~AnimatedTexture() {}

	
	static AnimatedTexture* CreateOrGetAnimatedTexture(const std::string& animatedTexturePath, int numOfSprites, int cellWidth, int cellHeight, bool allowFail = false);
	double CalcPercentCompleteTimeAndDuration(double animationAge, double timeElapsed);
	void   CalcTextureCoordinatesAtPercentComplete(double percentComplete, Vector2& minCoodinates_out, Vector2& maxCoordinates_out);

	int GetTotalNumberOfSprites()	{ return m_totalNumberOfSprites; }
	int GetNumSpritesX()			{ return m_cellWidth; }
	int GetNumSpritesY()			{ return m_cellHeight; }
private:
	static AnimatedTextureMap	s_AnimatedTextureMap;
	int m_totalNumberOfSprites;
	int m_cellHeight;
	int m_cellWidth;
};

#endif