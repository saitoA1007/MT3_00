#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2 operator+(const Vector2& other) { return { x + other.x, y + other.y}; }
	Vector2 operator-(const Vector2& other) { return { x - other.x, y - other.y}; }
	Vector2 operator*(const Vector2& other) { return { x * other.x, y * other.y}; }
	Vector2 operator/(const Vector2& other) { return { x / other.x, y / other.y}; }
	Vector2 operator+=(const Vector2& other) { return { x += other.x, y += other.y}; }
	Vector2 operator-=(const Vector2& other) { return { x -= other.x, y -= other.y}; }
	Vector2 operator*=(const Vector2& other) { return { x *= other.x, y *= other.y}; }
	Vector2 operator/=(const Vector2& other) { return { x /= other.x, y /= other.y}; }
};