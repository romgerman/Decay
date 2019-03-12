#pragma once

struct RectfCollision
{
	float x1, y1, x2, y2;

public:
	RectfCollision() : RectfCollision(0.f, 0.f, 0.f, 0.f) { }
	RectfCollision(float width, float height) : RectfCollision(0.f, 0.f, width, height) { }
	RectfCollision(float x1, float y1, float x2, float y2) : x1(x1), y1(y1), x2(x2), y2(y2) { }

	float Width()
	{
		return x2 - x1;
	}

	float Height()
	{
		return y2 - y1;
	}

	bool PointInside(float px, float py)
	{
		return (px > x1 && px < x2 && py > y1 && py < y2);
	}
};
