#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include <algorithm>

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

//scale=2
//bailout=1000
//kaleidoscopic distamce estimator. More at :http://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/
float Menger3distance(float x, float y, float z, float CX = 2, float CY = 4.8, float CZ = 1) {
    float r = x * x + y * y + z * z;
    float scale = 1.3;
    float bailout = 1000;
    int i;
    int MI = 1;
    for (i = 0; i < MI && r < bailout; i++) {
        glm::vec3 rot = rotateY(glm::vec3(x, y, z), radians(25));
        x = abs(rot.x);
        y = abs(rot.y);
        z = abs(rot.z);

        if (x - y < 0) { float x1 = y; y = x; x = x1; }
        if (x - z < 0) { float x1 = z; z = x; x = x1; }
        if (y - z < 0) { float y1 = z; z = y; y = y1; }

        rot = rotateY(glm::vec3(x, y, z), 0);
        x = rot.x;
        y = rot.y;
        z = rot.z;

        x = scale * x - CX * (scale - 1.f);
        y = scale * y - CY * (scale - 1.f);
        z = scale * z - CZ * (scale - 1.f);
        //z = scale * z;
        //if (z > 0.5f * CZ * (scale - 1.f)) z -= CZ * (scale - 1.f);

        r = x * x + y * y + z * z;
    }
    return (sqrtf(x * x + y * y + z * z) - 2) * powf(scale, -i);
}


//returns pair of (DE, closest point)
//TODO!
std::pair<float, glm::vec3> Menger3Point(glm::vec3 p, float CX = 2, float CY = 4.8, float CZ = 1) {
    float r = glm::dot(p, p);
    float scale = 1.3;
    float bailout = 1000;
    float rotate1angle = 25;
    float rotate2angle = 0;
    int i;
    int MI = 1;
    //auto point = glm::vec3(0, 2, 0);
    for (i = 0; i < MI && r < bailout; i++) {
        p = rotateY(p, radians(rotate1angle));
        p = glm::abs(p);

        if (p.x - p.y < 0) {
            float x1 = p.y; p.y = p.x; p.x = x1;
        }
        if (p.x - p.z < 0) {
            float x1 = p.z; p.z = p.x; p.x = x1;
        }
        if (p.y - p.z < 0) {
            float y1 = p.z; p.z = p.y; p.y = y1;
        }

        p = rotateY(p, radians(rotate2angle));
        p = p * scale - glm::vec3(CX, CY, 0) * (scale - 1.f);
        /*
        x = scale * x - CX * (scale - 1.f);
        y = scale * y - CY * (scale - 1.f);
        //z = scale * z - CZ * (scale - 1.f);
        z = scale * z;
        */
        if (p.z > 0.5f * CZ * (scale - 1.f)) p.z -= CZ * (scale - 1.f);

        r = glm::dot(p, p);
    }
    float distance = (glm::length(p) - 2) * powf(scale, -i);
    
    return { distance, p };
}

#include <stack>
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