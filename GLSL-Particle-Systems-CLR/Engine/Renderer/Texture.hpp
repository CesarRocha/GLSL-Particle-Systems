#pragma once
#ifndef __included_Texture__
#define __included_Texture__
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include <string>
#include <map>



class Texture;
typedef std::map< std::string, Texture* >	TextureMap ;
//===================================================================================================
//				class Texture																	   ==
//===================================================================================================
class Texture
{
public:
	Texture( const std::string& pathName );
	Texture( const std::string& pathName, int spriteCountX, int spriteCountY );
	Texture( const std::string& texturePath, unsigned char* pixelData, IntVector2 textureSize, int numberOfColorComponents);
	virtual ~Texture() {}
	static bool			DoesTextureFileExist(const std::string fileName);


	static Texture*		CreateOrGetTexture(const std::string& texturePath, bool allowFail = false);
	static Texture*		CreateOrGetTextureWithSprites(const std::string& texturePath, int spriteCountX, int spriteCountY, bool allowFail = false);
	static Texture*		CreateMissingTexture(const std::string& defaultName, unsigned char* pixelData, IntVector2 textureSize, int numberOfColorComponents);
	
	
	static Vector2		GetSpriteCoordsAtXYIndex(float percent, int x, int y);
	Vector2				GetSpriteCoordsAtXYIndex(int x, int y);


	Vector2				GetPercentPerSprite()		{ return m_percentPerSprite; }
	IntVector2			GetSize() 					{ return m_textureDimensions ; }
	const std::string&	GetName() const				{ return m_pathName ; }
	unsigned int		GetPlatformHandle() const	{ return m_platformHandle ; }



private:
	std::string			m_pathName ;
	IntVector2			m_textureDimensions;
	IntVector2			m_spriteDimensions;
	Vector2				m_percentPerSprite;
	unsigned int		m_platformHandle;
	static TextureMap	s_globalTextures ;
};
#endif


