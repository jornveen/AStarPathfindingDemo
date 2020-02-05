#pragma once
#include <Vector3.h>
#include <d3d12.h>
#include <directxtk/SimpleMath.h>

namespace ExMath
{
	void Clamp(float &value, float min, float max);
	void Clamp(int &value, int min, int max);

	float Abs(float value);
	float dist(float a, float b, float x, float y);
	igad::Vector3 Lerp(igad::Vector3 start, igad::Vector3 end, float percent);

	DirectX::SimpleMath::Vector3 IgadToSimple(igad::Vector3 vec);
	DirectX::SimpleMath::Vector2 IgadToSimple(igad::Vector2 vec);

	igad::Vector3 SimpleToIgad(DirectX::SimpleMath::Vector3 vec);
	igad::Vector2 SimpleToIgad(DirectX::SimpleMath::Vector2 vec);
}
