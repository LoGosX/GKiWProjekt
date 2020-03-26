#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <algorithm>
#include <stack>

/*
glm::vec3 rotateX(glm::vec3 p, float a) {
    return glm::vec3(
        p.x,
        cos(a) * p.y - sin(a) * p.z,
        sin(a) * p.y + cos(a) * p.z
        );
}

glm::vec3 rotateY(glm::vec3 p, float a) {
    return glm::vec3(
        cos(a) * p.x + sin(a) * p.z,
        p.y,
        -sin(a) * p.x + cos(a) * p.z
        );
}

glm::vec3 rotateZ(glm::vec3 p, float a) {
    return glm::vec3(
        cos(a) * p.x - sin(a) * p.y,
        sin(a) * p.x + cos(a) * p.y,
        p.z
        );
}
*/

glm::vec3 rotateX(glm::vec3 p, float a) {
    return glm::rotateX(p, a);
}

glm::vec3 rotateY(glm::vec3 p, float a) {
    return glm::rotateY(p, a);
}

glm::vec3 rotateZ(glm::vec3 p, float a) {
    return glm::rotateZ(p, a);
}

float radians(float degrees) {
    return degrees / 180 * 3.1415926535;
}

glm::vec3 closesPointOnTriangle(glm::vec3 triangle0, glm::vec3 triangle1, glm::vec3 triangle2, glm::vec3 sourcePosition)
{
    //https://www.gamedev.net/forums/topic/552906-closest-point-on-triangle/5421122/
    glm::vec3 edge0 = triangle1 - triangle0;
    glm::vec3 edge1 = triangle2 - triangle0;
    glm::vec3 v0 = triangle0 - sourcePosition;

    float a = glm::dot(edge0,edge0);
    float b = glm::dot(edge0,edge1);
    float c = glm::dot(edge1,edge1);
    float d = glm::dot(edge0,v0);
    float e = glm::dot(edge1,v0);

    float det = a * c - b * b;
    float s = b * e - c * d;
    float t = b * d - a * e;

    if (s + t < det)
    {
        if (s < 0.f)
        {
            if (t < 0.f)
            {
                if (d < 0.f)
                {
                    s = glm::clamp(-d / a, 0.f, 1.f);
                    t = 0.f;
                }
                else
                {
                    s = 0.f;
                    t = glm::clamp(-e / c, 0.f, 1.f);
                }
            }
            else
            {
                s = 0.f;
                t = glm::clamp(-e / c, 0.f, 1.f);
            }
        }
        else if (t < 0.f)
        {
            s = glm::clamp(-d / a, 0.f, 1.f);
            t = 0.f;
        }
        else
        {
            float invDet = 1.f / det;
            s *= invDet;
            t *= invDet;
        }
    }
    else
    {
        if (s < 0.f)
        {
            float tmp0 = b + d;
            float tmp1 = c + e;
            if (tmp1 > tmp0)
            {
                float numer = tmp1 - tmp0;
                float denom = a - 2 * b + c;
                s = glm::clamp(numer / denom, 0.f, 1.f);
                t = 1 - s;
            }
            else
            {
                t = glm::clamp(-e / c, 0.f, 1.f);
                s = 0.f;
            }
        }
        else if (t < 0.f)
        {
            if (a + d > b + e)
            {
                float numer = c + e - b - d;
                float denom = a - 2 * b + c;
                s = glm::clamp(numer / denom, 0.f, 1.f);
                t = 1 - s;
            }
            else
            {
                s = glm::clamp(-e / c, 0.f, 1.f);
                t = 0.f;
            }
        }
        else
        {
            float numer = c + e - b - d;
            float denom = a - 2 * b + c;
            s = glm::clamp(numer / denom, 0.f, 1.f);
            t = 1.f - s;
        }
    }

    return triangle0 + s * edge0 + t * edge1;
}

glm::vec3 closestPointOnTetrahedron(glm::vec3 position) {
    //http://blog.hvidtfeldts.net/index.php/2011/08/distance-estimated-3d-fractals-iii-folding-space/
    glm::vec3 tetrahedron[] = {
        { 1, 1, 1},
        {-1,-1, 1},
        { 1,-1,-1},
        {-1, 1,-1}
    };
    auto p1 = closesPointOnTriangle(tetrahedron[0], tetrahedron[1], tetrahedron[2], position);
    auto p2 = closesPointOnTriangle(tetrahedron[0], tetrahedron[1], tetrahedron[3], position);
    auto p3 = closesPointOnTriangle(tetrahedron[0], tetrahedron[2], tetrahedron[3], position);
    auto p4 = closesPointOnTriangle(tetrahedron[1], tetrahedron[2], tetrahedron[3], position);
    float d = glm::distance(position, p1);
    auto ans = p1;
    float d2 = glm::distance(position, p2);
    if (d2 < d) {
        d = d2;
        ans = p2;
    }
    d2 = glm::distance(position, p3);
    if (d2 < d) {
        d = d2;
        ans = p3;
    }
    d2 = glm::distance(position, p4);
    if (d2 < d) {
        d = d2;
        ans = p4;
    }
    return ans;
}

enum class OPERATION {
    ROTATE_X,
    ROTATE_Y,
    ROTATE_Z,
    SCALE,
    SCALE_X,
    SCALE_Y,
    SCALE_Z,
    SCALE_Z2,
    SCALE_Z3,
    CX,
    CY,
    CZ,
    OFFSET,
    ABS_X,
    ABS_Y,
    ABS_Z,
    FOLD_1,
    FOLD_2,
    FOLD_3,
    SWAP_XY,
    SWAP_XZ,
    SWAP_YZ
};

//scale=2
//bailout=1000
//kaleidoscopic distamce estimator. More at :http://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/
glm::vec3 Menger3point(glm::vec3 z, float rotationX = 0, float rotationY = 0, float rotationZ = 0)
{
    //works without rotations :/
    int Iterations = 16;
    glm::vec3 Offset(1,1,1);
    float Scale = 3;
    float CX = 1, CY = 1, CZ = 1;
    float bailout = 1000;
    float r = glm::dot(z,z), nx, nz, ny;
    int n = 0;
    std::stack<OPERATION> history;
    while (n < Iterations /*&& r < bailout*/) {
        z = rotateZ(z, rotationZ);
        history.push(OPERATION::ROTATE_Z);
        
        //abs fold
        if (z.x < 0) {
            z.x = -z.x;
            history.push(OPERATION::ABS_X);
        }
        if (z.y < 0) {
            z.y = -z.y;
            history.push(OPERATION::ABS_Y);
        }
        if (z.z < 0) {
            z.z = -z.z;
            history.push(OPERATION::ABS_Z);
        }

        //menger fold
        if (z.x - z.y < 0) { std::swap(z.x, z.y); history.push(OPERATION::SWAP_XY); }
        if (z.x - z.z < 0) { std::swap(z.x, z.z); history.push(OPERATION::SWAP_XZ); }
        if (z.y - z.z < 0) { std::swap(z.y, z.z); history.push(OPERATION::SWAP_YZ); }

        z = rotateX(z, rotationX);
        history.push(OPERATION::ROTATE_X);

        //scale wrt center CX,CY,CZ
        z.x = z.x * Scale - CX * (Scale - 1.f);
        history.push(OPERATION::SCALE_X);
        z.y = z.y * Scale - CY * (Scale - 1.f);
        history.push(OPERATION::SCALE_Y);
        z.z = z.z * Scale;
        history.push(OPERATION::SCALE_Z);
        if (z.z > 0.5f * CZ * (Scale - 1.f)) { 
            z.z -= CZ * (Scale - 1); 
            history.push(OPERATION::SCALE_Z2);
        }
        r = glm::dot(z, z);
        n++;
    }
    

    //does not work with rotations
    float q1 = 6.0F;
    float q2 = -6.0F;
    z.x = std::max(std::min(z.x, q1), q2);
    z.y = std::max(std::min(z.y, q1), q2);
    z.z = std::max(std::min(z.z, q1), q2);


    while (!history.empty()) {
        auto action = history.top(); history.pop();
        switch (action)
        {
        case OPERATION::SCALE_X:
            z.x = z.x + CX * (Scale - 1.f);
            z.x = z.x / Scale;
            break;
        case OPERATION::SCALE_Y:
            z.y = z.y + CY * (Scale - 1.f);
            z.y = z.y / Scale;
            break;
        case OPERATION::SCALE_Z:
            z.z = z.z / Scale;
            break;
        case OPERATION::SCALE_Z2:
            z.z = z.z + CZ * (Scale - 1);
            break;
        case OPERATION::OFFSET:
            z -= Offset;
            break;
        case OPERATION::ROTATE_X:
            z = rotateX(z, -rotationX);
            break;
        case OPERATION::ROTATE_Y:
            z = rotateY(z, -rotationY);
            break;
        case OPERATION::ROTATE_Z:
            z = rotateZ(z, -rotationZ);
            break;
        case OPERATION::ABS_X:
            z.x = -z.x;
            break;
        case OPERATION::ABS_Y:
            z.y = -z.y;
            break;
        case OPERATION::ABS_Z:
            z.z = -z.z;
            break;
        case OPERATION::SWAP_XY:
            std::swap(z.x, z.y);
            break;
        case OPERATION::SWAP_XZ:
            std::swap(z.x, z.z);
            break;
        case OPERATION::SWAP_YZ:
            std::swap(z.y, z.z);
            break;
        default:
            break;
        }
    }
    return z;
}

std::pair<float, glm::vec3> Menger3(glm::vec3 p, float rotationX = 0, float rotationY = 0, float rotationZ = 0) {
    auto m3 = Menger3point(p);
    return { glm::distance(p, m3), m3 };
}

glm::vec3 sierpinski3point(glm::vec3 z)
{
    int Iterations = 16;
    float Scale = 2;
    float r, nx, nz, ny;
    int n = 0;
    float rotation1 = 0;
    float rotation2 = 0;
    std::stack<int> history;
    while (n < Iterations) {
        z = rotateX(z, radians(rotation1));
        history.push(4);
        if (z.x + z.y < 0) {
            nx = -z.y;
            ny = -z.x;
            z.x = nx;
            z.y = ny;
            //z.xy = -z.yx; // fold 1
            history.push(1);
        }
        if (z.x + z.z < 0) {
            nx = -z.z;
            nz = -z.x;
            z.x = nx;
            z.z = nz;
            //z.xz = -z.zx; // fold 2
            history.push(2);
        }
        if (z.y + z.z < 0) {
            nz = -z.y;
            ny = -z.z;
            z.z = nz;
            z.y = ny;
            //z.zy = -z.yz; // fold 3	
            history.push(3);
        }
        z = rotateY(z, radians(rotation2));
        history.push(5);
        z = z * Scale - glm::vec3(1) * (Scale - 1.0f);
        history.push(0); //scale
        n++;
    }
    z = closestPointOnTetrahedron(z);
    while (!history.empty()) {
        int action = history.top(); history.pop();
        switch (action)
        {
        case 0:
            z = z + glm::vec3(1) / (Scale - 1.0f);
            z = z / Scale;
            break;
        case 1:
            nx = -z.y;
            ny = -z.x;
            z.x = nx;
            z.y = ny;
            break;
        case 2:
            nx = -z.z;
            nz = -z.x;
            z.x = nx;
            z.z = nz;
            break;
        case 3:
            nz = -z.y;
            ny = -z.z;
            z.z = nz;
            z.y = ny;
            break;
        case 4:
            z = rotateX(z, radians(-rotation1));
            break;
        case 5:
            z = rotateY(z, radians(-rotation2));
            break;
        default:
            break;
        }
    }
    return z;
}

float sierpinski3dist(glm::vec3 z)
{
    int Iterations = 16;
    float Scale = 2;
    float r;
    int n = 0;
    while (n < Iterations) {
        if (z.x + z.y < 0) {
            float nx = -z.y;
            float ny = -z.x;
            z.x = nx;
            z.y = ny;
            //z.xy = -z.yx; // fold 1
        }
        if (z.x + z.z < 0) {
            float nx = -z.z;
            float nz = -z.x;
            z.x = nx;
            z.z = nz;
            //z.xz = -z.zx; // fold 2
        }
        if (z.y + z.z < 0) {
            float nz = -z.y;
            float ny = -z.z;
            z.z = nz;
            z.y = ny;
            //z.zy = -z.yz; // fold 3	
        }
        z = z * Scale - glm::vec3(1) * (Scale - 1.0f);
        n++;
    }
    return (length(z)) * pow(Scale, -float(n));
}


std::pair<float, glm::vec3> sierpinski3(glm::vec3 p) {
    //returns pair of distance and closest point to sierpinski fractal
    auto s = sierpinski3point(p);
    return { glm::distance(s, p), s };
}