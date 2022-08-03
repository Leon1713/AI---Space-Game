#pragma once
#include <vector>
constexpr auto NNALPHA = 0.01f;

struct NNode
{
	std::vector<float> weights;
	float output;
	float z; // input
	float alpha; // mutation value small number
	NNode()
	{
		alpha = NNALPHA;
	}
	~NNode()
	{
	}
};