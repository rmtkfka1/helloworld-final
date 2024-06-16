#pragma once

class Helper
{
public:

	static wstring ToWString(string value);
	static string ToString(wstring value);
	static vec3 GetAttenuationCoeff(float distance);

	static vec3 ToEulerAngles(Quaternion q);

};

