#include "AnimatedTexture.hpp"
#include "Engine\Utility\Utility.hpp"
std::map< std::string, AnimatedTexture* >	AnimatedTexture::s_AnimatedTextureMap ; //Collection of all known animations loaded so far.  



//===================================================================================================
//			class AnimatedTexture																   ==
//===================================================================================================
AnimatedTexture::AnimatedTexture( const std::string& animatedTexturePath, int numberOfFrames, int cellsWide, int cellsHigh )
	: Texture(animatedTexturePath, cellsWide, cellsHigh)
	, m_totalNumberOfSprites(numberOfFrames)
	, m_cellHeight (cellsHigh)
	, m_cellWidth (cellsWide)
{
}



//===================================================================================================
AnimatedTexture* AnimatedTexture::CreateOrGetAnimatedTexture(const std::string& animatedTexturePath, int numberOfFrames, int cellsWide, int cellsHigh, bool allowFail)
{
	bool textureExists = DoesTextureFileExist(animatedTexturePath);
	if (!textureExists)
	{
		if (!allowFail)
			FATAL_ERROR(MakeComplexString("Texture path [%s] invalid. Texture not loaded.", animatedTexturePath.c_str()));
		return nullptr;
	}

	AnimatedTextureMap::iterator textureIter = s_AnimatedTextureMap.find( animatedTexturePath );
	if ( textureIter != s_AnimatedTextureMap.end() )
		return textureIter->second ;

	AnimatedTexture*	newTexture			= new AnimatedTexture( animatedTexturePath, numberOfFrames, cellsWide, cellsHigh );
	s_AnimatedTextureMap[ animatedTexturePath ]	= newTexture ;
	return newTexture ;
}
double  AnimatedTexture::CalcPercentCompleteTimeAndDuration(double animationAge, double timeElapsed)
{
	return timeElapsed / animationAge;
}
void AnimatedTexture::CalcTextureCoordinatesAtPercentComplete(double percentComplete, Vector2& minCoordinates_out, Vector2& maxCoordinates_out)
{
	float XsizeOfSprite = 1.0f / m_cellWidth;
	float YsizeOfSprite = 1.0f / m_cellHeight;

	int dpriteNumber = (int)(percentComplete*m_totalNumberOfSprites);
	int spriteColumn = dpriteNumber % m_cellWidth;
	int spriteRow = dpriteNumber / m_cellWidth;

	Vector2 minOut( (spriteColumn)     * XsizeOfSprite,      spriteRow      * YsizeOfSprite);
	Vector2 maxOut( (spriteColumn + 1) * XsizeOfSprite,     (spriteRow + 1) * YsizeOfSprite);

	minCoordinates_out = minOut;
	maxCoordinates_out = maxOut;


// 	float percentPerFrame = 1.0f / m_totalNumberOfSprites;
// 
// 	int   spriteNumber	= (int)percentComplete * m_totalNumberOfSprites;
// 	int	  spriteRow		= spriteNumber / m_cellWidth;
// 	int	  spriteColumn	= spriteNumber % m_cellWidth;
// 	float XsizeOfSprite	= 1.0f / m_cellWidth;
// 	float YsizeOfSprite	= 1.0f / m_cellHeight;
// 	minCoordinates_out	= Vector2(spriteColumn * XsizeOfSprite, spriteRow * YsizeOfSprite);
// 	maxCoordinates_out  = Vector2((spriteColumn + 1) * XsizeOfSprite, (spriteRow + 1) * YsizeOfSprite  );
}