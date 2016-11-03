//================================================================
//                     Font.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_Font_
#define _INCLUDE_Font_
#include "Engine/EngineCommon/EngineCommon.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include <string>
#include <vector>
#include <algorithm>

//===================================================================================================
//               class FaceData                                                                    ==
//===================================================================================================
class FaceData
{
public:
	FaceData() {};
	~FaceData();

	char  character;
	float x;
	float y;
	float width;
	float height;
	float xoffset;
	float yoffset;
	float xadvance;
private:
};



//===================================================================================================
//               class InfoText                                                                    ==
//===================================================================================================
struct InfoText
{
public:
	InfoText() {};
	InfoText(std::string text, Rgba color, bool autoDelete)
		:m_text(text)
		,m_textColor(color)
		,m_autoDelete(autoDelete){}

	std::string m_text;
	Rgba		m_textColor;
	bool		m_autoDelete;
};



//===================================================================================================
//               class Font                                                                        ==
//===================================================================================================
class Font
{
public:
	Font();
	Font(const std::string& fontFileName);
	

	void    ReadFontFileAndLoadToList(const std::string& FontFileName);
	vert_t* CreateFontVertsPerChar(const std::string& input, const Vector2& position, const Rgba& color, int& bufferSize);
	vert_t* CreateFontVertsPerCharLines(const Vector2& position, int& bufferSize);
	bool    UpdateTextString();


	void    RenderCursor(OpenGLRenderer* renderer);
	void    CursorBlink(double deltaSeconds);
	


	void	AddNewInfoLineText(std::string text);
	void	AddNewInfoLineText(std::string text, bool autoDelete);
	void	AddNewInfoLineText(std::string text, Rgba color);
	void	AddNewInfoLineText(std::string text, Rgba color, bool autoDelete);
	void	UpdateTextLinesLifetime(double deltaSeconds);
	void	ClearInfoTextLines() { m_activeTextLines.clear(); }


	std::string					m_myText;
	Vector2						m_cursorPosition;
	float						m_cursorHeight;
	float						m_riseY;
	int							m_vertCount;
	double						m_decayTime;
	bool						m_accendingActiveLines;
	size_t						m_activeTextLinesLimit;

private:
	friend class DeveloperLog;
	std::vector<InfoText>		m_activeTextLines;
	std::map <int, FaceData* >	m_glyphInfoSheet;
	InputSystem*				m_inputSystem;
	float						m_cursorBlink;
	bool						m_renderCursor;
	vert_t*						m_myVerts;
	GLuint						m_buff;
	GLuint						m_VAO;
};

#endif
