//================================================================
//                     AABB3.hpp
//================================================================

#pragma once
#ifndef  __included_AABB3__
#define  __included_AABB3__

#include "Vector3.hpp"


class AABB3
{
public:
	static const AABB3 ZERO_TO_ONE;
	Vector3 mins;
	Vector3 maxs;

	//Constructors
	~AABB3();
	AABB3();
	AABB3( const AABB3& copy );
	AABB3( float initialX, float initialY, float initialZ );
	AABB3( float minX, float minY, float minZ, float maxX, float maxY, float maxZ );
	AABB3( const Vector3& mins, const Vector3& maxs );
	AABB3( const Vector3& center, float radiusX, float radiusY, float radiusZ );

	//Mutators
	void StretchToIncludePoint( const Vector3& point );
	void AddPadding( float xPaddingRadius, float yPaddingRadius );
	void Translate( const Vector3& translation );
	const AABB3 Interpolate( const AABB3& start, const AABB3& end, float fractionFromStartToEnd );

	//Accessors
	bool IsPointInside( const Vector3& point ) const;
	const Vector3 CalcSize() const;
	const Vector3 CalcCenter() const;
	const Vector3 GetPointAtNormalizedPositionWithinBox( const Vector3& normalizedPosition ) const;
	const Vector3 GetNormalizedPositionForPointWithinBox( const Vector3& point ) const;

	const AABB3 operator + ( const Vector3& translation ) const;
	const AABB3 operator - ( const Vector3& antiTranslation ) const;
	void operator += ( const Vector3& translation );
	void operator -= ( const Vector3& antiTranslation );



};

#endif