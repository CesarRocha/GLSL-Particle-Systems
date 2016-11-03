#include "Engine/Renderer/Font/Font.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Clock/MasterClock.hpp"
#include <assert.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
std::vector<vert_t> myFontVector;
float backspaceTimeDelay	= .5f;
float backspaceDeleteDelay	= .2f;


//===================================================================================================
//               class Font                                                                        ==
//===================================================================================================
Font::Font()
	:m_cursorPosition(Vector2(5.0f, 15.0f))
	,m_cursorHeight(0.0f)
	,m_riseY(0.0f)
	,m_cursorBlink(1.4f)
	,m_decayTime(1.0)
	,m_accendingActiveLines(false)
	,m_activeTextLinesLimit(30)
{
}
Font::Font(const std::string& fontFileName)
	:m_cursorPosition(Vector2(5.0f, 15.0f))
	,m_cursorHeight(0.0f)
	,m_riseY(0.0f)
	,m_cursorBlink(1.4f)
	,m_decayTime(1.0)
	,m_accendingActiveLines(false)
	,m_activeTextLinesLimit(30)
{
	ReadFontFileAndLoadToList(fontFileName);
}


//===================================================================================================
void	Font::ReadFontFileAndLoadToList(const std::string& FontFileName)
{

	std::ifstream fontInfoFile;
	fontInfoFile.open(FontFileName); 
	if (!fontInfoFile)
	{
		std::string note = MakeComplexString("Font File Not Found!\n[%s]\n\nLook for\n_______WORKING IFSTEAM PATH.txt\n to find the working directory.", FontFileName.c_str());
		std::ofstream myfile;
		myfile.open("_______WORKING IFSTEAM PATH.txt");
		myfile << "This is the directory the 'ReadFontFileAndLoadToList' function failed to load from.";
		myfile.close();

		MessageBoxA(NULL, note.c_str(), "FontFileError", MB_OK);
	}
	


	unsigned int charcount = 0;
	if (fontInfoFile)  
	{
		std::string params;
		int i = 1;
		while (getline( fontInfoFile, params ))
		{
			if(i == 4)
			{
				params.erase( 0, params.find( "chars count=" ) + 12 );
				charcount = stoi( params.substr( 0, params.find_first_of( " " ) ) );
			}
			if( i > 4 &&  m_glyphInfoSheet.size() !=  charcount)
			{
				FaceData* faceData = new FaceData();
				int character;

				float x;
				float y;
				float height;
				float width;
				float xoffset;
				float yoffset;
				float xadvance;

				params.erase( 0, params.find( "id=" ) + 3 );
				character = stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "x=" ) + 2 );
				x = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "y=" ) + 2 );
				y = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "width=" ) + 6 );
				width = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "height=" ) + 7 );
				height = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "xoffset=" ) + 8 );
				xoffset = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "yoffset=" ) + 8 );
				yoffset = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				params.erase( 0, params.find( "xadvance=" ) + 9 );
				xadvance = (float)stoi( params.substr( 0, params.find_first_of( " " ) ) );

				

				faceData->character = (char)character;
				faceData->x = x;
				faceData->y = y;
				faceData->width = width;
				faceData->height = height;
				faceData->xoffset = xoffset;
				faceData->yoffset = yoffset;
				faceData->xadvance = xadvance;

				m_glyphInfoSheet[character] = faceData;

				if(height > m_cursorHeight)
					m_cursorHeight = height;
			}
			else
				++i;
		}
		fontInfoFile.close();
	}

}
void	Font::UpdateTextLinesLifetime(double deltaSeconds)
{

	if (m_decayTime <= 0.0 && m_activeTextLines.size() != 0)
	{
		InfoText infoText;
		for (std::vector<InfoText>::iterator iter = m_activeTextLines.begin(); iter != m_activeTextLines.end(); iter++)
		{
			infoText = *iter;
			if(infoText.m_autoDelete)
			{
				iter = m_activeTextLines.erase(iter);
				m_decayTime = .1; 
				break;
			}
		}
	}

	m_decayTime -= deltaSeconds;
}
bool	Font::UpdateTextString()
{
	if(g_displayConsole)
	{
		//Detect space bar
		if(s_theInputSystem->wasKeyReleased(VK_SPACE))
		{
			m_myText += ' ';
			return true;
		}

		//Detect backspace
		if(s_theInputSystem->isKeyDown(VK_BACK))
		{
			if(backspaceTimeDelay > 0.0)
			{
				backspaceTimeDelay -= MasterClock::GetMasterClock()->GetClockDeltaSeconds();
				return false;
			}
			
			if (m_myText.length() != 0)
			{
				if (backspaceDeleteDelay > 0.0f)
				{
					backspaceDeleteDelay -= MasterClock::GetMasterClock()->GetClockDeltaSeconds();
					return false;
				}

				backspaceDeleteDelay = 0.2f;
				m_myText.erase(m_myText.size() - 1);
				return true;
			}
		}
		if (s_theInputSystem->wasKeyReleased(VK_BACK))
		{
			if (m_myText.length() != 0)
			{
				m_myText.erase(m_myText.size() - 1);
				return true;
			}
			backspaceTimeDelay = .5f; 
			backspaceDeleteDelay = 0.2f;
		}




		if (s_theInputSystem->isKeyDown(VK_SHIFT))
		{
			for (int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; i++)
			{
				if(s_theInputSystem->wasKeyReleased(i))
				{
					if (i >= 65 && i <= 90)
					{
						m_myText += (char)i;
						return true;
					}

					if (i == 57) // (
					{
						m_myText += (char)i - 17;
						return true;
					}

					if (i == 48) // )
					{
						m_myText += (char)i - 7;
						return true;
					}
					if (i == 0xDE) // "
					{
						m_myText += 34;
						return true;
					}

					if (i == 0xBD) // _
					{
						m_myText += 95;
						return true;
					}
				}
			}
		}
		else
			//Detect A-Z/0-9
		for(int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; i++ )
		{
			//Lowercase
			if(s_theInputSystem->wasKeyReleased(i))
			{		
				if( i >= 65 && i <= 90)
				{
					m_myText += (char)(i + 32);
					return true;
				}
				if( i >= 188 &&  i<= 191)
				{
					m_myText += (char)(i - 144);
					return true;
				}
				if( i == 186 )
				{ 
					m_myText += 59;
					return true;
				}
				//Numbers
				if ( i >= 48 && i <= 57)
				{
					m_myText += (char)i;
					return true;
				}

				if (i == 0xDE) // '
				{
					m_myText += 39;
					return true;
				}
				//Special Chars

			}
		} 	
	}
	return false;
}
void	Font::CursorBlink(double deltaSeconds)
{
	if(m_cursorBlink > 0.7f)
		m_renderCursor = true;
	if(m_cursorBlink <= 0.7f)
		m_renderCursor = false;
	if(m_cursorBlink <= 0.0f)
		m_cursorBlink = 1.4f;

	m_cursorBlink -= (float)deltaSeconds;
}
void	Font::RenderCursor(OpenGLRenderer* renderer)
{
	if(m_renderCursor)
	{
		renderer->pushMatrix();
		renderer->SetOrthoGraphicView();
		renderer->SetModelColor(Rgba::GREEN);
		glDisable( GL_TEXTURE_2D );
		glBegin(GL_LINES);
		{
			glVertex3f(m_cursorPosition.x,  m_cursorPosition.y, 0.00f);
			glVertex3f(m_cursorPosition.x,  m_cursorPosition.y + m_cursorHeight, 0.00f);
		}
		glEnd();
		renderer->popMatrix();
	}
}



//===================================================================================================
vert_t* Font::CreateFontVertsPerChar(const std::string& text, const Vector2& position, const Rgba& color, int& bufferSize)
{
	myFontVector.clear();

	//Loop through string and find the data per char
	size_t inputLength = text.length();

	m_cursorPosition = position;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		char findThisChar = text[i];

		float x;
		float y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;
		float scale = 1.0f;

		std::map< int, FaceData* >::iterator found = m_glyphInfoSheet.find(findThisChar);

		if (found == m_glyphInfoSheet.end())
			continue;

		if (found->first == findThisChar)
		{
			x = found->second->x / 256;
			y = found->second->y / 256;
			width = found->second->width / 256;
			height = found->second->height / 256;
			xoffset = found->second->xoffset;
			yoffset = found->second->yoffset;
			xadvance = found->second->xadvance;
			float glyphW = found->second->width;
			float glyphH = found->second->height;


			//================================================================
			Vector2 mins(m_cursorPosition.x + xoffset * scale, m_cursorPosition.y + (32 - yoffset - glyphH) * scale);
			Vector2 maxs((mins.x + glyphW) * scale, (mins.y + glyphH) * scale);

			myFontVector.push_back(vert_t(Vector3(mins.x, mins.y + m_riseY, 0.0f), Vector2(x, y + height), color));
			myFontVector.push_back(vert_t(Vector3(maxs.x, mins.y + m_riseY, 0.0f), Vector2(x + width, y + height), color));
			myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y + m_riseY, 0.0f), Vector2(x + width, y), color));

			myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y + m_riseY, 0.0f), Vector2(x + width, y), color));
			myFontVector.push_back(vert_t(Vector3(mins.x, maxs.y + m_riseY, 0.0f), Vector2(x, y), color));
			myFontVector.push_back(vert_t(Vector3(mins.x, mins.y + m_riseY, 0.0f), Vector2(x, y + height), color));
			//================================================================

			m_cursorPosition.x += xadvance*scale;
		}
	}
	bufferSize = inputLength * 6 * sizeof(vert_t);
	m_vertCount = inputLength * 6;

	if(myFontVector.size() != 0)
		return &myFontVector[0]; 

	myFontVector.push_back(vert_t());
	return &myFontVector[0];
}
vert_t* Font::CreateFontVertsPerCharLines(const Vector2& position, int& bufferSize)
{
	myFontVector.clear();
	size_t inputLength = 0;
	char lineNumber = 0;

	InfoText infoText;
	std::string textLine;
	Rgba color;
	for (std::vector<InfoText>::iterator iter = m_activeTextLines.begin(); iter != m_activeTextLines.end(); iter++)
	{
		infoText = *iter;
		textLine = infoText.m_text;

		inputLength += textLine.length();
	}

	int totalchars = 0;
	for (std::vector<InfoText>::iterator iter = m_activeTextLines.begin(); iter != m_activeTextLines.end(); iter++)
	{
		infoText = *iter;
		textLine = infoText.m_text;
		color = infoText.m_textColor;
		
		m_cursorPosition = position;

		for (unsigned int charIndex = 0; charIndex < textLine.size(); charIndex++)
		{
			char findThisChar = textLine[charIndex];

			float x;
			float y;
			float width;
			float height;
			float xoffset;
			float yoffset;
			float xadvance;
			float scale = 1.0f;

			std::map< int, FaceData* >::iterator found = m_glyphInfoSheet.find(findThisChar);
			if (found == m_glyphInfoSheet.end())
				continue;

			if (found->first == findThisChar)
			{
				x = found->second->x / 256;
				y = found->second->y / 256;
				width = found->second->width / 256;
				height = found->second->height / 256;
				xoffset = found->second->xoffset;
				yoffset = found->second->yoffset;
				xadvance = found->second->xadvance;
				float glyphW = found->second->width;
				float glyphH = found->second->height;


				//================================================================
				Vector2 mins(m_cursorPosition.x + xoffset * scale, m_cursorPosition.y + (32 - yoffset - glyphH) * scale);
				Vector2 maxs((mins.x + glyphW) * scale, (mins.y + glyphH) * scale);
			
				if(m_accendingActiveLines)
				{
					//Goes Up
					myFontVector.push_back(vert_t(Vector3(mins.x, mins.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y + height), color));
					myFontVector.push_back(vert_t(Vector3(maxs.x, mins.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y + height), color));
					myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y), color));

					myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y), color));
					myFontVector.push_back(vert_t(Vector3(mins.x, maxs.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y), color));
					myFontVector.push_back(vert_t(Vector3(mins.x, mins.y + m_riseY + (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y + height), color));
				}
				else
				{
					//Goes down
					myFontVector.push_back(vert_t(Vector3(mins.x, mins.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y + height), color));
					myFontVector.push_back(vert_t(Vector3(maxs.x, mins.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y + height), color));
					myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y), color));

					myFontVector.push_back(vert_t(Vector3(maxs.x, maxs.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x + width, y), color));
					myFontVector.push_back(vert_t(Vector3(mins.x, maxs.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y), color));
					myFontVector.push_back(vert_t(Vector3(mins.x, mins.y - m_riseY - (lineNumber * m_cursorHeight), 0.0f), Vector2(x, y + height), color));
				}
				
				m_cursorPosition.x += xadvance*scale;

			}
		}
		lineNumber++;
		totalchars += textLine.size();
	}



	bufferSize = inputLength * 6 * sizeof(vert_t);
	m_vertCount = inputLength * 6;

	if (myFontVector.size() != 0)
		return &myFontVector[0];

	myFontVector.push_back(vert_t());
	return &myFontVector[0];
}



//===================================================================================================
void	Font::AddNewInfoLineText(std::string text)
{
	while (m_activeTextLines.size() >= m_activeTextLinesLimit)
		m_activeTextLines.erase(m_activeTextLines.begin());

	m_activeTextLines.push_back(InfoText(text, Rgba::RED, false));

	m_decayTime = 1.0;
}
void	Font::AddNewInfoLineText(std::string text, bool autoDelete)
{
	while (m_activeTextLines.size() >= m_activeTextLinesLimit)
		m_activeTextLines.erase(m_activeTextLines.begin());

	m_activeTextLines.push_back(InfoText(text, Rgba::RED, autoDelete));

	m_decayTime = 1.0;
}
void	Font::AddNewInfoLineText(std::string text, Rgba color)
{
	while (m_activeTextLines.size() >= m_activeTextLinesLimit)
		m_activeTextLines.erase(m_activeTextLines.begin());

	m_activeTextLines.push_back(InfoText(text, color, false));

	m_decayTime = 1.0;
}
void	Font::AddNewInfoLineText(std::string text, Rgba color, bool autoDelete)
{
	while (m_activeTextLines.size() >= m_activeTextLinesLimit)
		m_activeTextLines.erase(m_activeTextLines.begin());

	m_activeTextLines.push_back(InfoText(text, color, autoDelete));

	m_decayTime = 1.0;
}