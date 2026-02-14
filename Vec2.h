#pragma once

class Vec2
{
public:
	float x;
	float y;
	Vec2(float xin, float yin);
	Vec2();

	// Operator overloads
	bool operator == (const Vec2& other)  const;
	bool operator != (const Vec2& other) const;

	// Arithmetic operators
	Vec2 operator + (const Vec2& other) const;
	Vec2 operator - (const Vec2& other) const;
	Vec2 operator * (const float& scalar) const;
	Vec2 operator / (const float& scalar) const;

	// Compound assignment operators
	Vec2& operator += (const Vec2& other);
	Vec2& operator -= (const Vec2& other);
	Vec2& operator *= (const float& scalar);
	Vec2& operator /= (const float& scalar);
	
	// Vector operations
	float magnitude() const;
	Vec2 normalize() const;
	
};

