#include "Texture.hpp"
#include "Engine\Utility\Utility.hpp"
#define STBI_HEADER_FILE_ONLY
#include "Engine/Renderer/stb_image.c"
#undef STBI_HEADER_FILE_ONLY
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <fstream>
#define GL_CLAMP_TO_EDGE 0x812F
TextureMap	Texture::s_globalTextures ;



//===================================================================================================
//               class Texture			                                                           ==
//===================================================================================================
Texture::Texture( const std::string& texturePath )
	:m_textureDimensions(IntVector2(0,0))
	,m_spriteDimensions(IntVector2(0, 0))
	,m_percentPerSprite(Vector2(0, 0))
{
	int				x ;
	int				y ; 
	int				numberOfColorComponents ;
	//int				requiredNumberOfColorComponents ;
	unsigned char*	pixelData	= stbi_load( texturePath.c_str(), &x, &y, &numberOfColorComponents, 0 );

	m_textureDimensions.x		= x ;
	m_textureDimensions.y		= y ;
	m_pathName		= texturePath ;

	// Enable texturing
	glEnable( GL_TEXTURE_2D );

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_platformHandle );

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, m_platformHandle );

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR


	GLenum bufferFormat = GL_RGBA ; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( numberOfColorComponents == 3 )
		bufferFormat = GL_RGB ;
	else 
		if( numberOfColorComponents == 1 )
			bufferFormat = GL_RED ;
	// TODO: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(						// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,					// Creating this as a 2d texture
		0,								// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,					// Type of texel format we want OpenGL to use for this texture internally on the video card
		(int)m_textureDimensions.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		(int)m_textureDimensions.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,								// Border size, in texels (must be 0 or 1)
		bufferFormat,					// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,				// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData );					// Location of the actual pixel data bytes/buffer

	stbi_image_free( pixelData );
}
Texture::Texture( const std::string& texturePath, int spriteCountX, int spriteCountY )
{
	int				x ;
	int				y ; 
	int				numberOfColorComponents ;
	//int				requiredNumberOfColorComponents ;
	unsigned char*	pixelData	= stbi_load( texturePath.c_str(), &x, &y, &numberOfColorComponents, 0 );

	m_textureDimensions.x		= x ;
	m_textureDimensions.y		= y ;
	m_pathName		= texturePath ;

	m_spriteDimensions.x = m_textureDimensions.x / spriteCountX;
	m_spriteDimensions.y = m_textureDimensions.y / spriteCountY;
	m_percentPerSprite.x = (float)m_spriteDimensions.x / (float)x;
	m_percentPerSprite.y = (float)m_spriteDimensions.y / (float)y;
	// Enable texturing
	glEnable( GL_TEXTURE_2D );

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_platformHandle );

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, m_platformHandle );

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR


	GLenum bufferFormat = GL_RGBA ; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( numberOfColorComponents == 3 )
		bufferFormat = GL_RGB ;

	// TODO: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(						// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,					// Creating this as a 2d texture
		0,								// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,					// Type of texel format we want OpenGL to use for this texture internally on the video card
		(int)m_textureDimensions.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		(int)m_textureDimensions.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,								// Border size, in texels (must be 0 or 1)
		bufferFormat,					// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,				// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData );					// Location of the actual pixel data bytes/buffer

	stbi_image_free( pixelData );
}
Texture::Texture( const std::string& texturePath, unsigned char* pixelData, IntVector2 textureSize, int numberOfColorComponents)
{
	//unsigned char*	pixelData	= stbi_load( defaultName.c_str(), &textureSize.x, &textureSize.y, &numberOfColorComponents, 0 );

	m_textureDimensions.x		= textureSize.x ;
	m_textureDimensions.y		= textureSize.y ;
	m_pathName					= texturePath;

	// Enable texturing
	glEnable( GL_TEXTURE_2D );

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures( 1, (GLuint*) &m_platformHandle );

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, m_platformHandle );

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); // one of: GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT, GL_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR


	GLenum bufferFormat = GL_RGBA ; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if( numberOfColorComponents == 3 )
		bufferFormat = GL_RGB ;
	else 
		if( numberOfColorComponents == 1 )
			bufferFormat = GL_RED ;
	//TODO: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(						// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,					// Creating this as a 2d texture
		0,								// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,					// Type of texel format we want OpenGL to use for this texture internally on the video card
		(int)m_textureDimensions.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		(int)m_textureDimensions.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,								// Border size, in texels (must be 0 or 1)
		bufferFormat,					// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,				// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		pixelData );					// Location of the actual pixel data bytes/buffer

//	stbi_image_free( pixelData );
}



//===================================================================================================
bool	Texture::DoesTextureFileExist(const std::string fileName)
{
	std::ifstream infile(fileName.c_str());
	return infile.good();
}



//===================================================================================================
Texture* Texture::CreateOrGetTexture( const std::string& texturePath, bool allowFail )
{
	bool textureExists = DoesTextureFileExist(texturePath);
	if (!textureExists)
	{
		if(!allowFail)
			FATAL_ERROR(MakeComplexString("Texture path [%s] invalid. Texture not loaded.", texturePath.c_str()));
		return nullptr;
	}
	TextureMap::iterator textureIter = s_globalTextures.find( texturePath );
	if ( textureIter != s_globalTextures.end() )
		return textureIter->second ;

	Texture*	newTexture			= new Texture( texturePath );
	s_globalTextures[ texturePath ]	= newTexture ;
	return newTexture ;

}
Texture* Texture::CreateOrGetTextureWithSprites( const std::string& texturePath, int spriteCountX, int spriteCountY, bool allowFail)
{
	bool textureExists = DoesTextureFileExist(texturePath);
	if (!textureExists)
	{
		if (!allowFail)
			FATAL_ERROR(MakeComplexString("Texture path [%s] invalid. Texture not loaded.", texturePath.c_str()));
		return nullptr;
	}
	TextureMap::iterator textureIter = s_globalTextures.find( texturePath );
	if ( textureIter != s_globalTextures.end() )
		return textureIter->second ;

	Texture*	newTexture			= new Texture( texturePath, spriteCountX, spriteCountY );
	s_globalTextures[ texturePath ]	= newTexture ;
	return newTexture ;
}
Texture* Texture::CreateMissingTexture( const std::string& defaultName, unsigned char* pixelData, IntVector2 textureSize, int numberOfColorComponents)
{
	TextureMap::iterator textureIter = s_globalTextures.find( defaultName );
	if ( textureIter != s_globalTextures.end() )
		return textureIter->second ;

	Texture*	newTexture			= new Texture( defaultName, pixelData, textureSize, numberOfColorComponents );
	s_globalTextures[ defaultName ]	= newTexture ;
	return newTexture ;
}



//===================================================================================================
Vector2 Texture::GetSpriteCoordsAtXYIndex(float percent, int x, int y)
{
	return Vector2(percent * x, percent * y);
}
Vector2 Texture::GetSpriteCoordsAtXYIndex(int x, int y)
{
	return Vector2(m_percentPerSprite.x * x, m_percentPerSprite.y * y);
}