#version 330 core

//http://9bitscience.blogspot.com/2013/07/raymarching-distance-fields_14.html

uniform vec3 camera_direction = vec3(0, 0, 1);
uniform vec3 camera_position = vec3(0, 0, -1);
uniform vec3 camera_up = vec3(0, 1, 0);
uniform vec3 camera_right = vec3(1, 0, 0);

uniform float sphere_radius = 5;
uniform vec3 sphere_position = vec3(0, 5, 5);

uniform vec3 plane_position = vec3(0, -2, 0);

uniform float fov = 45;

uniform vec2 g_resolution;
uniform float g_rmEpsilon = 1e-3;

out vec4 glColor;

float sdPlane( vec3 p )
{
	return p.y;
}

float sdSphere( vec3 p, float s )
{
    return length(p)-s;
}

float sdCube( vec3 p, float r )
{
  vec3 q = abs(p) - vec3(r);
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float repeatedCubes(vec3 p) {
    p.x = mod(p.x, 1) - 0.5;
    p.z = mod(p.z, 1) - 0.5;
    return sdCube(p, 1);
}

vec3 floor_color(in vec3 p)
{
  if (fract(p.x*0.2)>0.2)
  {
    if (fract(p.z*0.2)>0.2)
      return vec3(0,0.1,0.2);
    else
      return vec3(1,1,1);
  }
  else
  {
    if (fract(p.z*.2)>.2)
      return vec3(1,1,1);
    else
      return vec3(0.3,0,0);
   }
}

void main()
{
    vec3 eye = camera_position;
    vec3 up = camera_up;
    vec3 right = camera_right;
    vec3 forward = camera_direction;
    float radius = sphere_radius;

    float fov_radians = fov / 180.0 * 3.14;
    float f = 0.5 / tan(fov / 2);
    float u = gl_FragCoord.x * 2.0 / g_resolution.x - 1.0;
    float v = gl_FragCoord.y * 2.0 / g_resolution.y - 1.0;
    vec3 p = eye + forward * f + right * u + up * v;
    vec3 ro = eye;
    vec3 rd = normalize(p - eye);
    

    vec4 color = vec4(0.0); // Sky color

    float t = 0.0;
    const int maxSteps = 64;
    for(int i = 0; i < maxSteps; ++i)
    {
        vec3 p = ro + rd * t;
        float ds = sdSphere(p - sphere_position, radius); // Distance to sphere
        float dp = sdPlane(p - plane_position); // Distance to plane
        float d = min(ds, dp);
        if(d < g_rmEpsilon)
        {
            if(ds < g_rmEpsilon) 
            {
                color = vec4(float(maxSteps - i) / 2 / maxSteps, 0, float(i) / maxSteps, 1); // Sphere color
                //vec3 grad = gradApprox(p, sphere_position, radius);
                //color = vec4(grad, 1);
                break;
            }
            if(dp > 0 && dp < g_rmEpsilon)
            {
                color = vec4(floor_color(p), 1);
                break;
            }
        }

        t += d;
    }

    glColor = color;
}