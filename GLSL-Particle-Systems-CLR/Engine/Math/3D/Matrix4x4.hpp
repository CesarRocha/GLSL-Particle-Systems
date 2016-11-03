//================================================================
//                     Matrix4x4.hpp
//================================================================
//YOU ARE IN ROW MAJOR
#pragma once
#ifndef  __included_Matrix4x4__
#define  __included_Matrix4x4__

#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/3D/Vector3.hpp"
#include "Engine/Math/3D/Vector4.hpp"
#include <cmath>

class Matrix4x4
{
public:
	Vector4 m_iBasis;
	Vector4 m_jBasis;
	Vector4 m_kBasis;
	Vector4 m_translation;

	//Constructors
	Matrix4x4();
	~Matrix4x4();
	Matrix4x4(const Matrix4x4& copy);
	explicit Matrix4x4( const Vector2& iBasis, const Vector2& jBasis);
	explicit Matrix4x4( const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation);
	explicit Matrix4x4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis);
	explicit Matrix4x4( const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation);
	explicit Matrix4x4( const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation);
	
	Matrix4x4 operator * (const Matrix4x4& matrix);
	void MakeIdentity();
	void Transform(Matrix4x4 tranform);
	Matrix4x4 GetTransformed(const Matrix4x4& transform);

	const float* GetAsFloatArray() const {};
	float* GetAsFloatArray();
	 
	Vector2 TransformPosition( const Vector2& position2D ) const; // Hard coded to assume z=0, w=1
	Vector3 TransformPosition( const Vector3& position3D ) const; // Hard coded to assume w=1
	Vector2 TransformDirection( const Vector2& direction2D ) const; // Hard coded to assume z=0, w=0
	Vector3 TransformDirection( const Vector3& direction3D ) const; // Hard coded to assume w=0
	Vector4 TransformVector( const Vector4& homogeneousVector ) const; // w is provided

	void Translate( const Vector2& translation2D ); // z translation assumed to be 0
	void Translate( const Vector3& translation3D );
	void Scale( float uniformScale ) ;
	void Scale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	void Scale( const Vector3& nonUniformScale3D );
	void RotateDegreesAboutX( float degrees );
	void RotateDegreesAboutY( float degrees );
	void RotateDegreesAboutZ( float degrees ); // Possibly also offered as RotateDegrees2D
	void RotateRadiansAboutX( float radians );
	void RotateRadiansAboutY( float radians );
	void RotateRadiansAboutZ( float radians ); // Possibly also offered as RotateRadians2D
	//Optimize each of the above methods my removing any unnecessary constructions and calculations; for example, Translate() need only calculate and modify the values at indexes 12, 13, and 14.

	static Matrix4x4 CreateTranslation( const Vector2& translation2D ); // z translation assumed to be 0
	static Matrix4x4 CreateTranslation( const Vector3& translation3D );
	static Matrix4x4 CreateScale( float uniformScale );
	static Matrix4x4 CreateScale( const Vector2& nonUniformScale2D ); // z scale assumed to be 1
	static Matrix4x4 CreateScale( const Vector3& nonUniformScale3D );
	static Matrix4x4 CreateRotationDegreesAboutX( float degrees );
	static Matrix4x4 CreateRotationDegreesAboutY( float degrees );
	static Matrix4x4 CreateRotationDegreesAboutZ( float degrees ); // a.k.a. CreateRotationDegrees2D
	static Matrix4x4 CreateRotationRadiansAboutX( float radians );
	static Matrix4x4 CreateRotationRadiansAboutY( float radians );
	static Matrix4x4 CreateRotationRadiansAboutZ( float radians ); // a.k.a. CreateRotationRadians2D
	//################################################################################################//
	//##                                                                                            ##//
	//################################################################################################//
	static void MultiplyTwoMatrixOutputNewResult( const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix, Matrix4x4& outgoing );
	void Transpose();
};
#endif