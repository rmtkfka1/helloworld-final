#pragma once

class Mesh;

class Helper
{
public:

	static wstring ToWString(string value);
	static string ToString(wstring value);
	static vec3 GetAttenuationCoeff(float distance);

	static vec3 ToEulerAngles(Quaternion q);
	static shared_ptr<Mesh> MakeSquareGrid(const int numSlices, const int numStacks, const float scale, const vec2 texScale);
	static shared_ptr<Mesh> MakeSquareGrid2(const int sizeX, const int sizeZ);
};

