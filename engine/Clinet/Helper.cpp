#include "pch.h"
#include "Helper.h"
#include "Mesh.h"

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

shared_ptr<Mesh> Helper::MakeSquareGrid(const int numSlices, const int numStacks, const float scale, const vec2 texScale)
{
    vector<Vertex> vec;
    vector<uint32> indices;

    float dx = 2.0f / numSlices;
    float dy = 2.0f / numStacks;

    float y = 1.0f;
    for (int j = 0; j < numStacks + 1; j++) {
        float x = -1.0f;
        for (int i = 0; i < numSlices + 1; i++) {
            Vertex v;
            v.position = vec3(x, y, 0.0f) * scale;
            v.normal = vec3(0.0f, 0.0f, -1.0f);
            v.uv = vec2(x + 1.0f, y + 1.0f) * 0.5f * texScale;
            v.tangent = vec3(1.0f, 0.0f, 0.0f);

            vec.push_back(v);

            x += dx;
        }
        y -= dy;
    }

    for (int j = 0; j < numStacks; j++) {
        for (int i = 0; i < numSlices; i++) {
            indices.push_back((numSlices + 1) * j + i);
            indices.push_back((numSlices + 1) * j + i + 1);
            indices.push_back((numSlices + 1) * (j + 1) + i);
            indices.push_back((numSlices + 1) * (j + 1) + i);
            indices.push_back((numSlices + 1) * j + i + 1);
            indices.push_back((numSlices + 1) * (j + 1) + i + 1);
        }
    }

    shared_ptr<Mesh> mesh = make_shared<Mesh>();

    mesh->Init(vec, indices);

    return mesh;
}

shared_ptr<Mesh> Helper::MakeSquareGrid2(const int sizeX, const int sizeZ)
{
    vector<Vertex> vec;

    for (int32 z = 0; z < sizeZ + 1; z++)
    {
        for (int32 x = 0; x < sizeX + 1; x++)
        {
            Vertex vtx;
            vtx.position = vec3(static_cast<float>(x), 0, static_cast<float>(z));
            vtx.uv = vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
            vtx.normal = vec3(0.f, 1.f, 0.f);
            vtx.tangent = vec3(1.f, 0.f, 0.f);

            vec.push_back(vtx);
        }
    }

    vector<uint32> idx;

    for (int32 z = 0; z < sizeZ; z++)
    {
        for (int32 x = 0; x < sizeX; x++)
        {
            //  [0]
            //   |	\
			//  [2] - [1]
            idx.push_back((sizeX + 1) * (z + 1) + (x));
            idx.push_back((sizeX + 1) * (z)+(x + 1));
            idx.push_back((sizeX + 1) * (z)+(x));
            //  [1] - [2]
            //   	\  |
            //		  [0]
            idx.push_back((sizeX + 1) * (z)+(x + 1));
            idx.push_back((sizeX + 1) * (z + 1) + (x));
            idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
        }
    }


    shared_ptr<Mesh> mesh = make_shared<Mesh>();
    mesh->Init(vec, idx);
    return mesh;
}
