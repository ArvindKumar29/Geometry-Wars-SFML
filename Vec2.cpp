#include "Vec2.h"
#include <cmath>
#include<iostream>

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin) {
};

Vec2::Vec2() : x(0.0f), y(0.0f) {};

bool Vec2::operator==(const Vec2& other) const {
	return (x == other.x && y == other.y);
}
bool Vec2::operator!=(const Vec2& other) const {
	return !(*this == other);
}
Vec2 Vec2::operator+(const Vec2& other) const {
	return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator-(const Vec2& other) const {
	return Vec2(x - other.x, y - other.y);
}
Vec2 Vec2::operator*(const float& scalar) const {
	return Vec2(x * scalar, y * scalar);
}
Vec2 Vec2::operator/(const float& scalar) const {
	return Vec2(x / scalar, y / scalar);
}
Vec2& Vec2::operator+=(const Vec2& other) {
	x += other.x;
	y += other.y;
	return *this;
}
Vec2& Vec2::operator-=(const Vec2& other) {
	x -= other.x;
	y -= other.y;
	return *this;
}
Vec2& Vec2::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}
Vec2& Vec2::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	return *this;
}
float Vec2::magnitude() const {
	return std::sqrt(x * x + y * y);
}
Vec2 Vec2::normalize() const {
	float mag = magnitude();
	if (mag == 0) {
		return Vec2(0, 0); // Return zero vector if magnitude is zero
	}
	return Vec2(x / mag, y / mag);
}
