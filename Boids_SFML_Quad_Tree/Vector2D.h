#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

//namespace Vector2D

	float vDistance(Vector2f a, Vector2f b)
	{
		return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	}

	float vMagnitudeSquared(Vector2f a)
	{
		return pow(a.x, 2) + pow(a.y, 2);
	}

	float vMagnitude(Vector2f a)
	{
		return sqrt(pow(a.x, 2) + pow(a.y, 2));
	}

	Vector2f vDivide(Vector2f a, float divisor)
	{
		a.x /= divisor;
		a.y /= divisor;
		return a;
	}

	Vector2f vMultiply(Vector2f a, float multiplier)
	{
		a.x *= multiplier;
		a.y *= multiplier;
		return a;
	}

	Vector2f vNormalize(Vector2f a)
	{
		float length = vMagnitude(a);
		if (length != 0)
		{
			a = vMultiply(a, 1.f / length);
		}
		return a;
	}

	Vector2f vSetMag(Vector2f a, float magnitude)
	{
		a = vNormalize(a);
		a = vMultiply(a, magnitude);
		return a;
	}

	Vector2f vLimit(Vector2f a, float max)
	{
		float magnitude_squared = vMagnitudeSquared(a);
		if (magnitude_squared > pow(max, 2))
		{
			a = vDivide(a, sqrt(magnitude_squared)); // normalize it, less maths than calling vNormalize.
			a = vMultiply(a, max);
		}
		return a;
	}



