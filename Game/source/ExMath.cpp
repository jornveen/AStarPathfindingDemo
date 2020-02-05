#include "ExMath.h"
#include <iostream>

void ExMath::Clamp(float &value, float min, float max)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
}

void ExMath::Clamp(int &value, int min, int max)
{
	if (value < min)
	{
		value = min;
	}
	else if (value > max)
	{
		value = max;
	}
}

float ExMath::Abs(float value)
{
	if (value < 0)
	{
		value = value * -1.0f;
	}

	return value;
}

float ExMath::dist(float a, float b, float x, float y)
{
	return std::sqrt(std::abs(((a - x) * (a - x)) + ((b - y) * (b - y))));
}

igad::Vector3 ExMath::Lerp(igad::Vector3 start, igad::Vector3 end, float percent)
{
	return (start + percent * (end - start));
}

DirectX::SimpleMath::Vector3 ExMath::IgadToSimple(igad::Vector3 vec)
{
	return DirectX::SimpleMath::Vector3(vec.x, vec.y, vec.z);
}

DirectX::SimpleMath::Vector2 ExMath::IgadToSimple(igad::Vector2 vec)
{
	return DirectX::SimpleMath::Vector2(vec.x, vec.y);
}

igad::Vector3 ExMath::SimpleToIgad(DirectX::SimpleMath::Vector3 vec)
{
	return igad::Vector3(vec.x, vec.y, vec.z);
}

igad::Vector2 ExMath::SimpleToIgad(DirectX::SimpleMath::Vector2 vec)
{
	return igad::Vector2(vec.x, vec.y);
}


