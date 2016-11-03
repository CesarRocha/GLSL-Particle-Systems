//================================================================
//                     ModelLoader.hpp
//================================================================
#pragma once
#ifndef _INCLUDE_MODEL_LOADER
#define _INCLUDE_MODEL_LOADER_
#include "Engine/Utility/Utility.hpp"
#include "Engine/Renderer/Shader/Material.hpp"
#include "Engine/Renderer/Shader/Mesh.hpp"
#include "Engine/Renderer/Shader/MaterialDependants/Shader.hpp"
#include "Engine/Renderer/Shader/MeshRenderer.hpp"


//===================================================================================================
//			class ModelLoader																	   ==
//===================================================================================================
class ModelLoader
{
public:
	ModelLoader();
	ModelLoader(const std::string& fileName, const std::string& specialShaderName = "", const BufferType& bufferType = VBO);

	void Render(const Matrix4x4& view, const Matrix4x4& projection, const Matrix4x4& model, const Vector3& cameraPosition, const double& deltaSeconds = 0);

	void SetNumberOfLights(char number);
	void SetMaterialShine(float shine);
	void SetMaterialAmbientColor(Rgba color);
	void SetMaterialAmbientPower(float power);

	void SetLightPositionAtIndex(char index, Vector3 position);

	void SetLightPropertyFloat(char index, LightPropertyFloat property, float value);
	void SetLightPropertyRgba(char index, LightPropertyRgba property, Rgba value);
	void SetLightPropertyVector3(char index, LightPropertyVector3 property, Vector3 value);

	void SetFogColorAndIntensity(Rgba CandI);
	void SetFogStart(float start);
	void SetFogEnd(float end);

	Vector3 GetLightPositionAtIndex(char index);


	MeshRenderer	m_modelRenderer;
	Mesh			m_modelMesh;
private:
	void CreateModelShader(const std::string & specialShaderName, const BufferType & bufferType);
	void LoadTexturesIntoModelSampler(const std::string& fileName);
	void CreateModelMesh(const std::string& fileName, const BufferType& bufferType);
	void CreateModelRenderer();
	void GLCheckError(const std::string& file, int line);
	Sampler			m_modelSampler;
	Shader			m_modelShader;
	Material		m_modelMaterial;
};



#endif
	

