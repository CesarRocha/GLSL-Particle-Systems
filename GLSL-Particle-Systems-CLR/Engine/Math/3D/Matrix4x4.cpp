//================================================================
//                     Matrix4x4.cpp
//================================================================
#include "Matrix4x4.hpp"

Matrix4x4::Matrix4x4()
{
	m_iBasis = Vector4     (1.0f, 0.0f, 0.0f, 0.0f);
	m_jBasis = Vector4     (0.0f, 1.0f, 0.0f, 0.0f);
	m_kBasis = Vector4     (0.0f, 0.0f, 1.0f, 0.0f);
	m_translation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4::~Matrix4x4()
{
}

Matrix4x4::Matrix4x4(const Matrix4x4& copy)
{
	m_iBasis = copy.m_iBasis;
	m_jBasis = copy.m_jBasis;
	m_kBasis = copy.m_kBasis;
	m_translation = copy.m_translation;
}

Matrix4x4::Matrix4x4(const Vector2& iBasis, const Vector2& jBasis)
	: m_iBasis     (iBasis.x,			iBasis.y,			0.0f,				0.0f)
	, m_jBasis     (jBasis.x,			jBasis.y,			0.0f,				0.0f)
	, m_kBasis     (0.0f,				0.0f,				1.0f,				0.0f)
	, m_translation(0.0f,				0.0f,				0.0f,				1.0f)
{
}

Matrix4x4::Matrix4x4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation)
	: m_iBasis     (iBasis.x,			iBasis.y,			0.0f,				0.0f)
	, m_jBasis     (jBasis.x,			jBasis.y,			0.0f,				0.0f)
	, m_kBasis     (0.0f,				0.0f,				1.0f,				0.0f)
	, m_translation(translation.x,		translation.y,		0.0f,				1.0f)
{
}

Matrix4x4::Matrix4x4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis)
	: m_iBasis     (iBasis.x,			iBasis.y,			iBasis.z,			 0.0f)
	, m_jBasis     (jBasis.x,			jBasis.y,			jBasis.z,			 0.0f)
	, m_kBasis     (kBasis.x,			kBasis.y,			kBasis.z,			 0.0f)
	, m_translation(0.0f,				0.0f,				0.0f,				 1.0f)
{
}

Matrix4x4::Matrix4x4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation)
	: m_iBasis     (iBasis.x,			iBasis.y,			iBasis.z,			0.0f)
	, m_jBasis     (jBasis.x,			jBasis.y,			jBasis.z,			0.0f)
	, m_kBasis     (kBasis.x,			kBasis.y,			kBasis.z,			0.0f)
	, m_translation(translation.x,		translation.y,		translation.z,      1.0f)
{
}

Matrix4x4::Matrix4x4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation)
	: m_iBasis     (iBasis.x,			iBasis.y,			iBasis.z,			iBasis.w)
	, m_jBasis     (jBasis.x,			jBasis.y,			jBasis.z,			jBasis.w)
	, m_kBasis     (kBasis.x,			kBasis.y,			kBasis.z,			kBasis.w)
	, m_translation(translation.x,      translation.y,      translation.z,      translation.w)
{
}

void Matrix4x4::MakeIdentity()
{
	m_iBasis = Vector4     (1.0f, 0.0f, 0.0f, 0.0f);
	m_jBasis = Vector4     (0.0f, 1.0f, 0.0f, 0.0f);
	m_kBasis = Vector4     (0.0f, 0.0f, 1.0f, 0.0f);
	m_translation = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix4x4::Transform(Matrix4x4 transform)
{
	m_iBasis.x = (m_iBasis.x * transform.m_iBasis.x) + (m_jBasis.x * transform.m_iBasis.y) + (m_kBasis.x * transform.m_iBasis.z) + (m_translation.x * transform.m_iBasis.w);
	m_iBasis.y = (m_iBasis.y * transform.m_iBasis.x) + (m_jBasis.y * transform.m_iBasis.y) + (m_kBasis.y * transform.m_iBasis.z) + (m_translation.y * transform.m_iBasis.w);
	m_iBasis.z = (m_iBasis.z * transform.m_iBasis.x) + (m_jBasis.z * transform.m_iBasis.y) + (m_kBasis.z * transform.m_iBasis.z) + (m_translation.z * transform.m_iBasis.w);
	m_iBasis.w = (m_iBasis.w * transform.m_iBasis.x) + (m_jBasis.w * transform.m_iBasis.y) + (m_kBasis.w * transform.m_iBasis.z) + (m_translation.w * transform.m_iBasis.w);

	m_jBasis.x = (m_iBasis.x * transform.m_jBasis.x) + (m_jBasis.x * transform.m_jBasis.y) + (m_kBasis.x * transform.m_jBasis.z) + (m_translation.x * transform.m_jBasis.w);
	m_jBasis.y = (m_iBasis.y * transform.m_jBasis.x) + (m_jBasis.y * transform.m_jBasis.y) + (m_kBasis.y * transform.m_jBasis.z) + (m_translation.y * transform.m_jBasis.w);
	m_jBasis.z = (m_iBasis.z * transform.m_jBasis.x) + (m_jBasis.z * transform.m_jBasis.y) + (m_kBasis.z * transform.m_jBasis.z) + (m_translation.z * transform.m_jBasis.w);
	m_jBasis.w = (m_iBasis.w * transform.m_jBasis.x) + (m_jBasis.w * transform.m_jBasis.y) + (m_kBasis.w * transform.m_jBasis.z) + (m_translation.w * transform.m_jBasis.w);

	m_kBasis.x = (m_iBasis.x * transform.m_kBasis.x) + (m_jBasis.x * transform.m_kBasis.y) + (m_kBasis.x * transform.m_kBasis.z) + (m_translation.x * transform.m_kBasis.w);
	m_kBasis.y = (m_iBasis.y * transform.m_kBasis.x) + (m_jBasis.y * transform.m_kBasis.y) + (m_kBasis.y * transform.m_kBasis.z) + (m_translation.y * transform.m_kBasis.w);
	m_kBasis.z = (m_iBasis.z * transform.m_kBasis.x) + (m_jBasis.z * transform.m_kBasis.y) + (m_kBasis.z * transform.m_kBasis.z) + (m_translation.z * transform.m_kBasis.w);
	m_kBasis.w = (m_iBasis.w * transform.m_kBasis.x) + (m_jBasis.w * transform.m_kBasis.y) + (m_kBasis.w * transform.m_kBasis.z) + (m_translation.w * transform.m_kBasis.w);

	m_translation.x = (m_iBasis.x * transform.m_translation.x) + (m_jBasis.x * transform.m_translation.y) + (m_kBasis.x * transform.m_translation.z) + (m_translation.x * transform.m_translation.w);
	m_translation.y = (m_iBasis.y * transform.m_translation.x) + (m_jBasis.y * transform.m_translation.y) + (m_kBasis.y * transform.m_translation.z) + (m_translation.y * transform.m_translation.w);
	m_translation.z = (m_iBasis.z * transform.m_translation.x) + (m_jBasis.z * transform.m_translation.y) + (m_kBasis.z * transform.m_translation.z) + (m_translation.z * transform.m_translation.w);
	m_translation.w = (m_iBasis.w * transform.m_translation.x) + (m_jBasis.w * transform.m_translation.y) + (m_kBasis.w * transform.m_translation.z) + (m_translation.w * transform.m_translation.w);

}

Matrix4x4 Matrix4x4::GetTransformed(const Matrix4x4& transform)
{
	Matrix4x4 temp;

	temp.m_iBasis.x = (m_iBasis.x * transform.m_iBasis.x) + (m_jBasis.x * transform.m_iBasis.y) + (m_kBasis.x * transform.m_iBasis.z) + (m_translation.x * transform.m_iBasis.w);
	temp.m_iBasis.y = (m_iBasis.y * transform.m_iBasis.x) + (m_jBasis.y * transform.m_iBasis.y) + (m_kBasis.y * transform.m_iBasis.z) + (m_translation.y * transform.m_iBasis.w);
	temp.m_iBasis.z = (m_iBasis.z * transform.m_iBasis.x) + (m_jBasis.z * transform.m_iBasis.y) + (m_kBasis.z * transform.m_iBasis.z) + (m_translation.z * transform.m_iBasis.w);
	temp.m_iBasis.w = (m_iBasis.w * transform.m_iBasis.x) + (m_jBasis.w * transform.m_iBasis.y) + (m_kBasis.w * transform.m_iBasis.z) + (m_translation.w * transform.m_iBasis.w);

	temp.m_jBasis.x = (m_iBasis.x * transform.m_jBasis.x) + (m_jBasis.x * transform.m_jBasis.y) + (m_kBasis.x * transform.m_jBasis.z) + (m_translation.x * transform.m_jBasis.w);
	temp.m_jBasis.y = (m_iBasis.y * transform.m_jBasis.x) + (m_jBasis.y * transform.m_jBasis.y) + (m_kBasis.y * transform.m_jBasis.z) + (m_translation.y * transform.m_jBasis.w);
	temp.m_jBasis.z = (m_iBasis.z * transform.m_jBasis.x) + (m_jBasis.z * transform.m_jBasis.y) + (m_kBasis.z * transform.m_jBasis.z) + (m_translation.z * transform.m_jBasis.w);
	temp.m_jBasis.w = (m_iBasis.w * transform.m_jBasis.x) + (m_jBasis.w * transform.m_jBasis.y) + (m_kBasis.w * transform.m_jBasis.z) + (m_translation.w * transform.m_jBasis.w);

	temp.m_kBasis.x = (m_iBasis.x * transform.m_kBasis.x) + (m_jBasis.x * transform.m_kBasis.y) + (m_kBasis.x * transform.m_kBasis.z) + (m_translation.x * transform.m_kBasis.w);
	temp.m_kBasis.y = (m_iBasis.y * transform.m_kBasis.x) + (m_jBasis.y * transform.m_kBasis.y) + (m_kBasis.y * transform.m_kBasis.z) + (m_translation.y * transform.m_kBasis.w);
	temp.m_kBasis.z = (m_iBasis.z * transform.m_kBasis.x) + (m_jBasis.z * transform.m_kBasis.y) + (m_kBasis.z * transform.m_kBasis.z) + (m_translation.z * transform.m_kBasis.w);
	temp.m_kBasis.w = (m_iBasis.w * transform.m_kBasis.x) + (m_jBasis.w * transform.m_kBasis.y) + (m_kBasis.w * transform.m_kBasis.z) + (m_translation.w * transform.m_kBasis.w);

	temp.m_translation.x = (m_iBasis.x * transform.m_translation.x) + (m_jBasis.x * transform.m_translation.y) + (m_kBasis.x * transform.m_translation.z) + (m_translation.x * transform.m_translation.w);
	temp.m_translation.y = (m_iBasis.y * transform.m_translation.x) + (m_jBasis.y * transform.m_translation.y) + (m_kBasis.y * transform.m_translation.z) + (m_translation.y * transform.m_translation.w);
	temp.m_translation.z = (m_iBasis.z * transform.m_translation.x) + (m_jBasis.z * transform.m_translation.y) + (m_kBasis.z * transform.m_translation.z) + (m_translation.z * transform.m_translation.w);
	temp.m_translation.w = (m_iBasis.w * transform.m_translation.x) + (m_jBasis.w * transform.m_translation.y) + (m_kBasis.w * transform.m_translation.z) + (m_translation.w * transform.m_translation.w);
	return temp;
}

// const float* GetAsFloatArray() const {};
// float* GetAsFloatArray();



Vector2 Matrix4x4::TransformPosition(const Vector2& position2D) const
{
	Matrix4x4 temp;
	temp.MakeIdentity();
	Vector4 tempVec4;
	tempVec4.x = position2D.x;
	tempVec4.y = position2D.y;
	tempVec4.z = 0.0f;
	tempVec4.w = 1.0f;

	return Vector2(temp.m_iBasis.x+position2D.x, temp.m_jBasis.y+position2D.y);
}

Vector3 Matrix4x4::TransformPosition(const Vector3& position3D) const
{
	Matrix4x4 temp;
	temp.MakeIdentity();
	Vector4 tempVec4;
	tempVec4.x = position3D.x;
	tempVec4.y = position3D.y;
	tempVec4.z = position3D.z;
	tempVec4.w = 1.0f;

	return Vector3(temp.m_iBasis.x+position3D.x, temp.m_jBasis.y+position3D.y, temp.m_kBasis.z+position3D.z);
}

Vector2 Matrix4x4::TransformDirection(const Vector2& direction2D) const
{
	Matrix4x4 temp;
	temp.MakeIdentity();
	Vector4 tempVec4;
	tempVec4.x = direction2D.x;
	tempVec4.y = direction2D.y;
	tempVec4.z = 0.0f;
	tempVec4.w = 0.0f;

	return Vector2(temp.m_iBasis.x+direction2D.x, temp.m_jBasis.y+direction2D.y);
}

Vector3 Matrix4x4::TransformDirection(const Vector3& direction3D) const
{
	Matrix4x4 temp;
	temp.MakeIdentity();
	Vector4 tempVec4;
	tempVec4.x = direction3D.x;
	tempVec4.y = direction3D.y;
	tempVec4.z = direction3D.z;
	tempVec4.w = 0.0f;

	return Vector3(temp.m_iBasis.x+direction3D.x, temp.m_jBasis.y+direction3D.y, temp.m_kBasis.z+direction3D.z);
}

Vector4 Matrix4x4::TransformVector(const Vector4& homogeneousVector) const
{
	Matrix4x4 temp;
	temp.MakeIdentity();
	return Vector4(temp.m_iBasis.x+homogeneousVector.x, temp.m_jBasis.y+homogeneousVector.y, temp.m_kBasis.z+homogeneousVector.z, temp.m_translation.w+homogeneousVector.w);
}

void Matrix4x4::Translate(const Vector2& translation2D)
{
	m_iBasis.x += translation2D.x;
	m_jBasis.y += translation2D.y;
}

void Matrix4x4::Translate(const Vector3& translation3D)
{
	Matrix4x4 translation;
	translation.m_translation.setXYZ( translation3D.x, translation3D.y, translation3D.z, 1.0f );
	
	Matrix4x4 out;
	Matrix4x4::MultiplyTwoMatrixOutputNewResult( *this, translation, out );

	m_translation = out.m_translation;
}

void Matrix4x4::Scale(float uniformScale)
{
	m_iBasis.x *= uniformScale;
	m_jBasis.y *= uniformScale;
	m_kBasis.z *= uniformScale;
}

void Matrix4x4::Scale(const Vector2& nonUniformScale2D)
{
	m_iBasis.x *= nonUniformScale2D.x;
	m_jBasis.y *= nonUniformScale2D.y;
}

void Matrix4x4::Scale(const Vector3& nonUniformScale3D)
{
	m_iBasis.x *= nonUniformScale3D.x;
	m_jBasis.y *= nonUniformScale3D.y;
	m_kBasis.z *= nonUniformScale3D.z;
}

//Correct 
void Matrix4x4::RotateDegreesAboutX( float degrees )
{
	Matrix4x4 newXRotation;

	float radians = degrees * DEG2RAD;
	float cosine = cos( radians );
	float sine = sin( radians );

	newXRotation.m_jBasis.y = cosine;
	newXRotation.m_jBasis.z = sine;
	newXRotation.m_kBasis.y = -sine;
	newXRotation.m_kBasis.z = cosine;

	Matrix4x4 myOldTransform = *this;
	MultiplyTwoMatrixOutputNewResult( myOldTransform, newXRotation, *this );
}
//Correct
void Matrix4x4::RotateDegreesAboutY( float degrees )
{
	Matrix4x4 newYRotation;

	float radians = degrees * DEG2RAD;
	newYRotation.m_iBasis.x = cos(radians);
	newYRotation.m_iBasis.z = -sin(radians);
	newYRotation.m_kBasis.x = sin(radians);
	newYRotation.m_kBasis.z = cos(radians);

	Matrix4x4 myOldTransform = *this;
	MultiplyTwoMatrixOutputNewResult( myOldTransform, newYRotation, *this );
}


//Correct
void Matrix4x4::RotateDegreesAboutZ( float degrees )
{
	Matrix4x4 newZRotation;
	float radians = degrees * DEG2RAD;
	float cosine = cos( radians );
	float sine = sin( radians );

	newZRotation.m_iBasis.x = cosine;
	newZRotation.m_iBasis.y = sine;
	newZRotation.m_jBasis.x = -sine;
	newZRotation.m_jBasis.y = cosine;

	Matrix4x4 myOldTransform = *this;
	MultiplyTwoMatrixOutputNewResult( myOldTransform, newZRotation, *this );
}


void Matrix4x4::RotateRadiansAboutX(float radians)
{
	m_jBasis.y =  cos(-1*ConvertRadiansToDegrees(radians));
	m_jBasis.z =  sin(-1*ConvertRadiansToDegrees(radians));

	m_kBasis.y =  -m_jBasis.z;
	m_kBasis.z =   m_jBasis.y;
}

void Matrix4x4::RotateRadiansAboutY(float radians)
{
	m_kBasis.x =  sin(-1*ConvertRadiansToDegrees(radians));
	m_kBasis.z =  cos(-1*ConvertRadiansToDegrees(radians));

	m_iBasis.x =  m_kBasis.z;
	m_iBasis.z = -m_kBasis.x;
}

void Matrix4x4::RotateRadiansAboutZ(float radians)
{
	m_iBasis.x =  cos(-1*ConvertRadiansToDegrees(radians));
	m_iBasis.y =  sin(-1*ConvertRadiansToDegrees(radians));

	m_jBasis.x = -m_iBasis.y;
	m_jBasis.y =  m_iBasis.x;
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector2& translation2D)
{
	Matrix4x4 temp;
	temp.m_translation.x = translation2D.x;
	temp.m_translation.y = translation2D.y;
	return temp;
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& translation3D)
{
	Matrix4x4 temp;
	temp.m_translation.x = translation3D.x;
	temp.m_translation.y = translation3D.y;
	temp.m_translation.z = translation3D.z;
	return temp;
}


Matrix4x4 Matrix4x4::CreateScale(float uniformScale)
{
	Matrix4x4 temp;
	temp.m_iBasis.x = uniformScale;
	temp.m_jBasis.y = uniformScale;
	temp.m_kBasis.z = uniformScale;
	return temp;
}


Matrix4x4 Matrix4x4::CreateScale(const Vector2& nonUniformScale2D)
{
	Matrix4x4 temp;
	temp.m_iBasis.x = nonUniformScale2D.x;
	temp.m_jBasis.y = nonUniformScale2D.y;
	return temp;
}


Matrix4x4 Matrix4x4::CreateScale(const Vector3& nonUniformScale3D)
{
	Matrix4x4 temp;
	temp.m_iBasis.x = nonUniformScale3D.x;
	temp.m_jBasis.y = nonUniformScale3D.y;
	temp.m_kBasis.z = nonUniformScale3D.y;
	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationDegreesAboutX(float degrees)
{
	Matrix4x4 temp;

	temp.m_jBasis.y =  cos(-1*degrees);
	temp.m_jBasis.z =  sin(-1*degrees);

	temp.m_kBasis.y =  -temp.m_jBasis.z;
	temp.m_kBasis.z =   temp.m_jBasis.y;

	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationDegreesAboutY(float degrees)
{
	Matrix4x4 temp;

	temp.m_kBasis.x =  sin(-1*degrees);
	temp.m_kBasis.z =  cos(-1*degrees);

	temp.m_iBasis.x =  temp.m_kBasis.z;
	temp.m_iBasis.z = -temp.m_kBasis.x;

	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationDegreesAboutZ(float degrees)
{
	Matrix4x4 temp;

	temp.m_iBasis.x =  cos(-1*degrees);
	temp.m_iBasis.y =  sin(-1*degrees);

	temp.m_jBasis.x = -temp.m_iBasis.y;
	temp.m_jBasis.y =  temp.m_iBasis.x;

	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationRadiansAboutX(float radians)
{
	Matrix4x4 temp;

	temp.m_jBasis.y =  cos(-1*ConvertRadiansToDegrees(radians));
	temp.m_jBasis.z =  sin(-1*ConvertRadiansToDegrees(radians));

	temp.m_kBasis.y =  -temp.m_jBasis.z;
	temp.m_kBasis.z =   temp.m_jBasis.y;

	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationRadiansAboutY(float radians)
{
	Matrix4x4 temp;

	temp.m_kBasis.x =  sin(-1*ConvertRadiansToDegrees(radians));
	temp.m_kBasis.z =  cos(-1*ConvertRadiansToDegrees(radians));

	temp.m_iBasis.x =  temp.m_kBasis.z;
	temp.m_iBasis.z = -temp.m_kBasis.x;

	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationRadiansAboutZ(float radians)
{
	Matrix4x4 temp;

	temp.m_iBasis.x =  cos(-1*ConvertRadiansToDegrees(radians));
	temp.m_iBasis.y =  sin(-1*ConvertRadiansToDegrees(radians));

	temp.m_jBasis.x = -temp.m_iBasis.y;
	temp.m_jBasis.y =  temp.m_iBasis.x;

	return temp;
}

//################################################################################################//
//##                                                                                            ##//
//################################################################################################//
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& matrix)
{
	Matrix4x4 outgoing;
	outgoing.m_iBasis.x		=    (outgoing.m_iBasis.x * matrix.m_iBasis.x )	 +	(outgoing.m_iBasis.y * matrix.m_jBasis.x )	+	(outgoing.m_iBasis.z * matrix.m_kBasis.x )  +	(outgoing.m_iBasis.w * matrix.m_translation.x );
	outgoing.m_iBasis.y		=    (outgoing.m_iBasis.x * matrix.m_iBasis.y )	 +	(outgoing.m_iBasis.y * matrix.m_jBasis.y )	+	(outgoing.m_iBasis.z * matrix.m_kBasis.y )  +	(outgoing.m_iBasis.w * matrix.m_translation.y );
	outgoing.m_iBasis.z		=    (outgoing.m_iBasis.x * matrix.m_iBasis.z )	 +	(outgoing.m_iBasis.y * matrix.m_jBasis.z )	+	(outgoing.m_iBasis.z * matrix.m_kBasis.z )  +	(outgoing.m_iBasis.w * matrix.m_translation.z );
	outgoing.m_iBasis.w		=    (outgoing.m_iBasis.x * matrix.m_iBasis.w )	 +	(outgoing.m_iBasis.y * matrix.m_jBasis.w )	+	(outgoing.m_iBasis.z * matrix.m_kBasis.w )  +	(outgoing.m_iBasis.w * matrix.m_translation.w );

	outgoing.m_jBasis.x		=    (outgoing.m_jBasis.x * matrix.m_iBasis.x )	 +	(outgoing.m_jBasis.y * matrix.m_jBasis.x )	+	(outgoing.m_jBasis.z * matrix.m_kBasis.x )  +	(outgoing.m_jBasis.w * matrix.m_translation.x );
	outgoing.m_jBasis.y		=    (outgoing.m_jBasis.x * matrix.m_iBasis.y )	 +	(outgoing.m_jBasis.y * matrix.m_jBasis.y )	+	(outgoing.m_jBasis.z * matrix.m_kBasis.y )  +	(outgoing.m_jBasis.w * matrix.m_translation.y );
	outgoing.m_jBasis.z		=    (outgoing.m_jBasis.x * matrix.m_iBasis.z )	 +	(outgoing.m_jBasis.y * matrix.m_jBasis.z )	+	(outgoing.m_jBasis.z * matrix.m_kBasis.z )  +	(outgoing.m_jBasis.w * matrix.m_translation.z );
	outgoing.m_jBasis.w		=    (outgoing.m_jBasis.x * matrix.m_iBasis.w )	 +	(outgoing.m_jBasis.y * matrix.m_jBasis.w )	+	(outgoing.m_jBasis.z * matrix.m_kBasis.w )  +	(outgoing.m_jBasis.w * matrix.m_translation.w );

	outgoing.m_kBasis.x		=    (outgoing.m_kBasis.x * matrix.m_iBasis.x )	 +	(outgoing.m_kBasis.y * matrix.m_jBasis.x )	+	(outgoing.m_kBasis.z * matrix.m_jBasis.x)  +	(outgoing.m_kBasis.w * matrix.m_translation.x);
	outgoing.m_kBasis.y		=    (outgoing.m_kBasis.x * matrix.m_iBasis.y )	 +	(outgoing.m_kBasis.y * matrix.m_jBasis.y )	+	(outgoing.m_kBasis.z * matrix.m_jBasis.y)  +	(outgoing.m_kBasis.w * matrix.m_translation.y);
	outgoing.m_kBasis.z		=    (outgoing.m_kBasis.x * matrix.m_iBasis.z )	 +	(outgoing.m_kBasis.y * matrix.m_jBasis.z )	+	(outgoing.m_kBasis.z * matrix.m_jBasis.z)  +	(outgoing.m_kBasis.w * matrix.m_translation.z);
	outgoing.m_kBasis.w		=    (outgoing.m_kBasis.x * matrix.m_iBasis.w )	 +	(outgoing.m_kBasis.y * matrix.m_jBasis.w )	+	(outgoing.m_kBasis.z * matrix.m_jBasis.w)  +	(outgoing.m_kBasis.w * matrix.m_translation.w);

	outgoing.m_translation.x=    (outgoing.m_translation.x * matrix.m_iBasis.x)    +	(outgoing.m_translation.y * matrix.m_jBasis.x)	+	(outgoing.m_translation.z * matrix.m_jBasis.x)  +	(outgoing.m_translation.w * matrix.m_translation.x);
	outgoing.m_translation.y=    (outgoing.m_translation.x * matrix.m_iBasis.y)    +	(outgoing.m_translation.y * matrix.m_jBasis.y)	+	(outgoing.m_translation.z * matrix.m_jBasis.y)  +	(outgoing.m_translation.w * matrix.m_translation.y);
	outgoing.m_translation.z=    (outgoing.m_translation.x * matrix.m_iBasis.z)    +	(outgoing.m_translation.y * matrix.m_jBasis.z)	+	(outgoing.m_translation.z * matrix.m_jBasis.z)  +	(outgoing.m_translation.w * matrix.m_translation.z);
	outgoing.m_translation.w=    (outgoing.m_translation.x * matrix.m_iBasis.w)    +	(outgoing.m_translation.y * matrix.m_jBasis.w)	+	(outgoing.m_translation.z * matrix.m_jBasis.w)  +	(outgoing.m_translation.w * matrix.m_translation.w);
	return outgoing;
}


void Matrix4x4::MultiplyTwoMatrixOutputNewResult( const Matrix4x4& leftMatrix, const Matrix4x4& rightMatrix, Matrix4x4& outgoing)
{
	const Matrix4x4& l = leftMatrix;
	const Matrix4x4& r = rightMatrix;

	outgoing.m_iBasis.x =    (r.m_iBasis.x * l.m_iBasis.x )	 +	(r.m_iBasis.y * l.m_jBasis.x )	+	(r.m_iBasis.z * l.m_kBasis.x  )  +	(r.m_iBasis.w * l.m_translation.x );
	outgoing.m_iBasis.y =    (r.m_iBasis.x * l.m_iBasis.y )	 +	(r.m_iBasis.y * l.m_jBasis.y )	+	(r.m_iBasis.z * l.m_kBasis.y  )  +	(r.m_iBasis.w * l.m_translation.y );
	outgoing.m_iBasis.z =    (r.m_iBasis.x * l.m_iBasis.z )	 +	(r.m_iBasis.y * l.m_jBasis.z )	+	(r.m_iBasis.z * l.m_kBasis.z )  +	(r.m_iBasis.w * l.m_translation.z );
	outgoing.m_iBasis.w =    (r.m_iBasis.x * l.m_iBasis.w )	 +	(r.m_iBasis.y * l.m_jBasis.w )	+	(r.m_iBasis.z * l.m_kBasis.w )  +	(r.m_iBasis.w * l.m_translation.w );

	outgoing.m_jBasis.x =    (r.m_jBasis.x * l.m_iBasis.x )	 +	(r.m_jBasis.y * l.m_jBasis.x )	+	(r.m_jBasis.z * l.m_kBasis.x  )  +	(r.m_jBasis.w * l.m_translation.x );
	outgoing.m_jBasis.y =    (r.m_jBasis.x * l.m_iBasis.y )	 +	(r.m_jBasis.y * l.m_jBasis.y )	+	(r.m_jBasis.z * l.m_kBasis.y  )  +	(r.m_jBasis.w * l.m_translation.y );
	outgoing.m_jBasis.z =    (r.m_jBasis.x * l.m_iBasis.z )	 +	(r.m_jBasis.y * l.m_jBasis.z )	+	(r.m_jBasis.z * l.m_kBasis.z )  +	(r.m_jBasis.w * l.m_translation.z );
	outgoing.m_jBasis.w =    (r.m_jBasis.x * l.m_iBasis.w )	 +	(r.m_jBasis.y * l.m_jBasis.w )	+	(r.m_jBasis.z * l.m_kBasis.w )  +	(r.m_jBasis.w * l.m_translation.w );

	outgoing.m_kBasis.x =    (r.m_kBasis.x * l.m_iBasis.x )	 +	(r.m_kBasis.y * l.m_jBasis.x )	+	(r.m_kBasis.z * l.m_kBasis.x )  +	(r.m_kBasis.w * l.m_translation.x);
	outgoing.m_kBasis.y =    (r.m_kBasis.x * l.m_iBasis.y )	 +	(r.m_kBasis.y * l.m_jBasis.y )	+	(r.m_kBasis.z * l.m_kBasis.y )  +	(r.m_kBasis.w * l.m_translation.y);
	outgoing.m_kBasis.z=    (r.m_kBasis.x * l.m_iBasis.z )	 +	(r.m_kBasis.y * l.m_jBasis.z )	+	(r.m_kBasis.z * l.m_kBasis.z)  +	(r.m_kBasis.w * l.m_translation.z);
	outgoing.m_kBasis.w=    (r.m_kBasis.x * l.m_iBasis.w )	 +	(r.m_kBasis.y * l.m_jBasis.w )	+	(r.m_kBasis.z * l.m_kBasis.w)  +	(r.m_kBasis.w * l.m_translation.w);

	outgoing.m_translation.x=    (r.m_translation.x * l.m_iBasis.x)     +	(r.m_translation.y * l.m_jBasis.x)	+	(r.m_translation.z * l.m_kBasis.x )  +	(r.m_translation.w * l.m_translation.x);
	outgoing.m_translation.y=    (r.m_translation.x * l.m_iBasis.y)     +	(r.m_translation.y * l.m_jBasis.y)	+	(r.m_translation.z * l.m_kBasis.y )  +	(r.m_translation.w * l.m_translation.y);
	outgoing.m_translation.z=    (r.m_translation.x * l.m_iBasis.z)     +	(r.m_translation.y * l.m_jBasis.z)	+	(r.m_translation.z * l.m_kBasis.z)  +	(r.m_translation.w * l.m_translation.z);
	outgoing.m_translation.w=    (r.m_translation.x * l.m_iBasis.w)     +	(r.m_translation.y * l.m_jBasis.w)	+	(r.m_translation.z * l.m_kBasis.w)  +	(r.m_translation.w * l.m_translation.w);
}

void Matrix4x4::Transpose()
{
	Matrix4x4 afterTransformedMatrix;
	afterTransformedMatrix.m_iBasis = Vector4(m_iBasis.x,m_jBasis.x, m_kBasis.x, m_translation.x);
	afterTransformedMatrix.m_jBasis = Vector4(m_iBasis.y,m_jBasis.y, m_kBasis.y, m_translation.y);
	afterTransformedMatrix.m_kBasis = Vector4(m_iBasis.z,m_jBasis.z, m_kBasis.z, m_translation.z);
	afterTransformedMatrix.m_translation = Vector4(m_iBasis.w,m_jBasis.w, m_kBasis.w, m_translation.w);
	m_iBasis = afterTransformedMatrix.m_iBasis;
	m_jBasis = afterTransformedMatrix.m_jBasis;
	m_kBasis = afterTransformedMatrix.m_kBasis;
	m_translation = afterTransformedMatrix.m_translation;
}





