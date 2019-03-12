#pragma once

#include <math.h>

struct CircleCollision
{
	float x, y;
	float r;

public:
	CircleCollision() : CircleCollision(0.f, 0.f, 0.f){ }
	CircleCollision(float x, float y, float radius) : x(x), y(y), r(radius) { }

	bool PointInside(float x, float y)
	{
		x = x - this->x;
		y = y - this->y;

		return (sqrtf(x * x + y * y) < r);
	}
};
