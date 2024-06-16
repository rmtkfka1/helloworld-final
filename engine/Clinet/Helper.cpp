#include "pch.h"
#include "Helper.h"

std::wstring Helper::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Helper::ToString(wstring value)
{
	return string(value.begin(), value.end());
}

vec3 Helper::GetAttenuationCoeff(float distance)
{
    const auto linear_coeff = vec4(
        8.4523112e-05,
        4.4712582e+00,
        -1.8516388e+00,
        3.3955811e+01
    );
    const auto quad_coeff = vec4(
        -7.6103583e-04,
        9.0120201e+00,
        -1.1618500e+01,
        1.0000464e+02
    );

    float kc = 1.0f;
    float d = 1.0f / distance;
    auto dvec = vec4(1.0f, d, d * d, d * d * d);
    float kl = linear_coeff.Dot(dvec);
    float kq = quad_coeff.Dot(dvec);

    return vec3(kc, max(kl, 0.0f), max(kq * kq, 0.0f));
}

vec3 Helper::ToEulerAngles(Quaternion q)
{
    vec3 angles;

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}
