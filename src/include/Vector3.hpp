#pragma once
#include <math.h>
class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	Vector3 operator+(Vector3 other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(Vector3 other) {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(float lambda) {
		return Vector3(x * lambda, y * lambda, z * lambda);
	}

	Vector3 operator/(float lambda) {
		return lambda == 0 ? Vector3(0, 0, 0) : Vector3(x / lambda, y / lambda, z / lambda);
	}

	float dot(Vector3 other) {
		return x * other.x + y * other.y + z * other.z;
	}

	float len(void) {
		return sqrt(x * x + y * y + z * z);
	}
};

class Vector2 {
public:
	float x, y;

	float len(void) {
		return  sqrt(x * x + y * y);
	}
};
