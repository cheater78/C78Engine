#pragma once


struct Vec2 {
	float x;
	float y;

	Vec2 operator+(Vec2 v) {
		return { x + v.x, y + v.y };
	}
	Vec2 operator-(Vec2 v) {
		return { x - v.x, y - v.y };
	}
	Vec2 operator*(float v) {
		return { x * v, y * v };
	}

};

struct Vec3 {
	float x;
	float y;
	float z;

	Vec3 operator+(Vec3 v) {
		return { x + v.x, y + v.y, z + v.z };
	}
	Vec3 operator-(Vec3 v) {
		return { x - v.x, y - v.y, z - v.z };
	}
	Vec3 operator*(float v) {
		return { x * v, y * v, z * v};
	}
};

