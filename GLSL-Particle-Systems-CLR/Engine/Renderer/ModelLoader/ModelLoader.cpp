#include "Engine/Renderer/ModelLoader/ModelLoader.hpp"
#include "Engine/Utility/Parser/BinaryFileParser.hpp"



//===================================================================================================
//				class ModelLoader																   ==
//===================================================================================================
ModelLoader::ModelLoader()
{
}
ModelLoader::ModelLoader(const std::string& fileName, const std::string& specialShaderName, const BufferType& bufferType)
{
	CreateModelShader(specialShaderName, bufferType);
	
	LoadTexturesIntoModelSampler(fileName);
	
	CreateModelMesh("../Run_Win32/data/model/" + fileName + "/" + fileName + ".c23", bufferType);

	CreateModelRenderer();
}



//===================================================================================================
//																								   ==
//===================================================================================================
void ModelLoader::Render(const Matrix4x4& view, const Matrix4x4& projection, const Matrix4x4& model, const Vector3& cameraPosition, const double& deltaSeconds)
{
	m_modelRenderer.Render(view, projection, model, cameraPosition, deltaSeconds);
}



//===================================================================================================
//																								   ==
//===================================================================================================
void ModelLoader::SetLightPropertyFloat(char index, LightPropertyFloat property, float value)
{
	switch (property)
	{
	case LIGHT_PROPERTY_AMBIENT_STRENGTH: m_modelRenderer.m_material.m_lightArray[index].m_lightAmbientStrength = value;
		break;
	case LIGHT_PROPERTY_INTENSITY: m_modelRenderer.m_material.m_lightArray[index].m_lightIntensity = value;
		break;
	case LIGHT_PROPERTY_WEIGHT: m_modelRenderer.m_material.m_lightArray[index].m_lightWeight = value;
		break;
	case LIGHT_PROPERTY_ANGLE: m_modelRenderer.m_material.m_lightArray[index].m_lightAngle = value;
		break;
	case LIGHT_PROPERTY_SPOT_POWER: m_modelRenderer.m_material.m_lightArray[index].m_lightSpotPower = value;
		break;
	case LIGHT_PROPERTY_LIGHT_POWER: m_modelRenderer.m_material.m_lightArray[index].m_lightPower = value;
		break;
	case LIGHT_PROPERTY_MINSF: m_modelRenderer.m_material.m_lightArray[index].m_lightMinSf = value;
		break;
	case LIGHT_PROPERTY_MIN_INTENSITY: m_modelRenderer.m_material.m_lightArray[index].m_lightMinIntensity = value;
		break;
	case LIGHT_PROPERTY_MAX_INTENSITY: m_modelRenderer.m_material.m_lightArray[index].m_lightMaxIntensity = value;
		break;

	default:
		break;
	}

}
void ModelLoader::SetLightPropertyRgba(char index, LightPropertyRgba property, Rgba value)
{
	switch (property)
	{
	case	LIGHT_PROPERTY_COLOR: m_modelRenderer.m_material.m_lightArray[index].m_lightColor = value;
		break;

	default:
		break;
	}
}
void ModelLoader::SetLightPropertyVector3(char index, LightPropertyVector3 property, Vector3 value)
{
	switch (property)
	{
	case LIGHT_PROPERTY_POSITION: m_modelRenderer.m_material.m_lightArray[index].m_lightPosition = value;
		break;
	case LIGHT_PROPERTY_DIRECTION: m_modelRenderer.m_material.m_lightArray[index].m_lightDirection = value;
		break;

	default:
		break;
	}
}
void ModelLoader::SetFogColorAndIntensity(Rgba CandI)
{
	m_modelRenderer.SetFogColorAndIntensity(CandI);
}
void ModelLoader::SetFogStart(float start)
{
	m_modelRenderer.SetFogStart(start);
}
void ModelLoader::SetFogEnd(float end)
{
	m_modelRenderer.SetFogEnd(end);
}
void ModelLoader::SetNumberOfLights(char number)
{
	if(number >= 0 && number <= 16)
		m_modelRenderer.m_material.m_lightCount = number;
}
void ModelLoader::SetMaterialShine(float shine)
{ 
	m_modelRenderer.m_material.m_shine = shine;
}
void ModelLoader::SetMaterialAmbientColor(Rgba color)
{
	m_modelRenderer.m_material.m_materialAmbientColor = color;
}
void ModelLoader::SetMaterialAmbientPower(float power)
{
	m_modelRenderer.m_material.m_materialAmbientPower = power;
}
void ModelLoader::SetLightPositionAtIndex(char index, Vector3 position)
{
	if (index >= 0 && index <= 16)
		m_modelRenderer.m_material.m_lightArray[index].m_lightPosition = position;

}
Vector3 ModelLoader::GetLightPositionAtIndex(char index)
{

	if (index >= 0 && index <= 16)
		return m_modelRenderer.m_material.m_lightArray[index].m_lightPosition;

	return Vector3::ZERO;
}



//===================================================================================================
//				class ModelLoader Private														   ==
//===================================================================================================
void ModelLoader::CreateModelShader(const std::string& specialShaderName, const BufferType& bufferType)
{
	if(specialShaderName != "")
		m_modelShader = *Shader::CreateOrGetShader("../Run_Win32/shaders/model", "../Run_Win32/shaders/model", "../Run_Win32/shaders/" + specialShaderName, "../Run_Win32/shaders/model", GEOTRIANGLE);
	else
		m_modelShader = *Shader::CreateOrGetShader("../Run_Win32/shaders/model", "../Run_Win32/shaders/model",  "../Run_Win32/shaders/model", bufferType);
}
void ModelLoader::LoadTexturesIntoModelSampler(const std::string& fileName)
{
 	Texture* diffuse = Texture::CreateOrGetTexture("../Run_Win32/data/model/" + fileName + "/" + fileName + "_diffuse.png", true);
	if(diffuse != nullptr)
		m_modelSampler.SetDiffuse( diffuse );

	Texture* specular = Texture::CreateOrGetTexture("../Run_Win32/data/model/" + fileName + "/" + fileName + "_normal.png", true);
	if(specular !=  nullptr)
		m_modelSampler.SetNormal( specular );

	m_modelMaterial = Material(m_modelSampler);
}
void ModelLoader::CreateModelMesh(const std::string& filePath, const BufferType& bufferType)
{
	BinaryFileParser modelParser( filePath );


	std::string GC23;
	GC23.resize(4);
	GC23[0] = modelParser.ReadNextUnsignedChar();
	GC23[1] = modelParser.ReadNextUnsignedChar();
	GC23[2] = modelParser.ReadNextUnsignedChar();
	GC23[3] = modelParser.ReadNextUnsignedChar();
	unsigned char subtype = modelParser.ReadNextUnsignedChar();
	unsigned char version = modelParser.ReadNextUnsignedChar();
	std::string comment = modelParser.ReadNextString();
	ConsolePrintf("Magic Number: %c%c%c%c \n", GC23[0], GC23[1], GC23[2], GC23[3]);
	ConsolePrintf("Subtype: %u \n", subtype);
	ConsolePrintf("Version: %u \n", version);
	ConsolePrintf("Comment: %s \n", comment);


	//===================================================================================================
	if (GC23 != "GC23")	{
		vert_t verts[] = {vert_t( Vector3( 0.0f,  0.0f,  0.0f    ), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f,  0.0f ))};
		m_modelMesh = Mesh(verts, 1);
		ConsolePrintf("Wrong Magic Number \n", NULL);
		MessageBoxA( NULL, "Imported Model Incompatible (.C23)", "Magic Number Error", MB_OK);
		return;
	}


	//===================================================================================================
	if(subtype == 2)
	{
		vert_t* verts = nullptr;

		//Get number of vertex's
		unsigned int numVerts = modelParser.ReadNextUnsignedInt();
		verts = new vert_t[numVerts];

		//Get vertex
		for(unsigned int i = 0; i < numVerts; i++)
			verts[i] = modelParser.ReadNextTriangleVertex();

		//Get VBO Data
		if(bufferType == VBO || bufferType == GEOPOINT || bufferType == GEOTRIANGLE)
			m_modelMesh = Mesh(verts, numVerts, 0, 0); 
		GLCheckError(__FILE__,  __LINE__ -1);

		//Get Index Data
		if(bufferType == IBO)
		{
			unsigned int totalNumberOfIndexs = modelParser.ReadNextUnsignedInt();
			unsigned int* indexNumber;
			indexNumber = new unsigned int[totalNumberOfIndexs];
			for(unsigned int i = 0; i < totalNumberOfIndexs; i++)
				indexNumber[i] = modelParser.ReadNextUnsignedInt();

			m_modelMesh = Mesh(verts, numVerts, indexNumber, totalNumberOfIndexs  ); 
			GLCheckError(__FILE__,  __LINE__ -1);
		}
	}

	//===================================================================================================
	if(subtype == 4)
	{
		vert_s* verts = nullptr;

		//Get number of bones
		unsigned char numBones = modelParser.ReadNextUnsignedChar();

		//Get bone transforms
		Matrix4x4* boneTransforms;
		boneTransforms = new Matrix4x4[numBones];
		for(unsigned int index = 0; index < numBones; index++)
 			boneTransforms[index] = modelParser.ReadNextMatrix4x4();

		//Get number of vertex's
		unsigned int numVerts = modelParser.ReadNextUnsignedInt();
		verts = new vert_s[numVerts];

		//Get vertex
		for(unsigned int i = 0; i < numVerts; i++)
			verts[i] = modelParser.ReadNextSkeletalVertex();

		//Get VBO Data
		if(bufferType == VBO)
			m_modelMesh = Mesh(verts, numVerts, 0, 0); 
		GLCheckError(__FILE__,  __LINE__ -1);
 
		//Get Index Data
		if(bufferType == IBO)
		{
			unsigned int totalNumberOfIndexs = modelParser.ReadNextUnsignedInt();
			unsigned int* indexNumber;
			indexNumber = new unsigned int[totalNumberOfIndexs];
			for(unsigned int i = 0; i < totalNumberOfIndexs; i++)
				indexNumber[i] = modelParser.ReadNextUnsignedInt();

 		//	m_modelMesh = Mesh(verts, numVerts, indexNumber, totalNumberOfIndexs, numBones, boneTransforms ); 
 			GLCheckError(__FILE__,  __LINE__ -1);
 		}
	}
}
void ModelLoader::CreateModelRenderer()
{
	m_modelRenderer = MeshRenderer(m_modelMesh, m_modelMaterial, m_modelShader);
}
void ModelLoader::GLCheckError(const std::string& file, int line)
{
#if defined(OPENGL_CHECK_ERRORS)
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{

		std::stringstream out;
		out << line;
		std::string header = "Error: " + ConsolePrintf("0x%04x\n", error) + " at " + out.str();
		std::string message1 = file;
		std::string message3 = message1;
		MessageBoxA(NULL, message3.c_str(), header.c_str(), MB_OK);
	}
#endif
}
