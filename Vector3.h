#pragma once

struct Vector3 {
	float x, y, z;

	Vector3 operator+(const Vector3& other) { return { x + other.x, y + other.y, z + other.z }; }
	Vector3 operator-(const Vector3& other) { return { x - other.x, y - other.y, z - other.z }; }
	Vector3 operator*(const Vector3& other) { return { x * other.x, y * other.y, z * other.z }; }
	Vector3 operator/(const Vector3& other) { return { x / other.x, y / other.y, z / other.z }; }
	Vector3 operator+=(const Vector3& other) { return { x += other.x, y += other.y, z += other.z }; }
	Vector3 operator-=(const Vector3& other) { return { x -= other.x, y -= other.y, z -= other.z }; }
	Vector3 operator*=(const Vector3& other) { return { x *= other.x, y *= other.y, z *= other.z }; }
	Vector3 operator/=(const Vector3& other) { return { x /= other.x, y /= other.y, z /= other.z }; }
	Vector3 operator+(const float& other) { return { x + other, y + other, z + other }; }
	Vector3 operator-(const float& other) { return { x - other, y - other, z - other }; }
	Vector3 operator*(const float& other) { return { x * other, y * other, z * other }; }
	Vector3 operator/(const float& other) { return { x / other, y / other, z / other }; }
};
