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
uniform float g_rmEpsilon = 1e-5;

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

float DE(vec3 z)
{
  z.xy = mod((z.xy),1.0)-vec2(0.5); // instance on xy-plane
  return length(z)-0.3;             // sphere DE
}

// Mandelbulb distance estimation:
// http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
vec2 SceneInfo(vec3 position) {
    vec3 z = position;
	float dr = 1.0;
	float r = 0.0;
    int iterations = 0;
    float power = 2;

	for (int i = 0; i < 25 ; i++) {
        iterations = i;
		r = length(z);

		if (r>2) {
            break;
        }
        
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.x, z.y);
		dr =  pow( r, power-1.0)*power*dr + 1.0;

		// scale and rotate the point
		float zr = pow( r,power);
		theta = theta*power;
		phi = phi*power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=position;
	}
    float dst = 0.5*log(r)*r/dr;
	return vec2(iterations,dst*1);
}

vec3 EstimateNormal(vec3 p) {
float epsilon = g_rmEpsilon;
    float x = SceneInfo(vec3(p.x+epsilon,p.y,p.z)).y - SceneInfo(vec3(p.x-epsilon,p.y,p.z)).y;
    float y = SceneInfo(vec3(p.x,p.y+epsilon,p.z)).y - SceneInfo(vec3(p.x,p.y-epsilon,p.z)).y;
    float z = SceneInfo(vec3(p.x,p.y,p.z+epsilon)).y - SceneInfo(vec3(p.x,p.y,p.z-epsilon)).y;
    return normalize(vec3(x,y,z));
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
    const int maxSteps = 128;
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
        //vec2 sceneInfo = SceneInfo(p);
        //float d = sceneInfo.y;
        //float dst = d;
        //// Ray has hit a surface
        //if (dst <= g_rmEpsilon) {
        //    float escapeIterations = sceneInfo.x;
        //    vec3 normal = EstimateNormal(ro - rd*g_rmEpsilon*2);
        //
        //    vec3 _LightDirection = vec3(0, -1, 0);
        //    float colourAMix = 1;
        //    float colourBMix = 1;
        //
        //    float colourA = clamp(dot(normal*.5+.5,-_LightDirection), 0, 1);
        //    float colourB = clamp(maxSteps/16.0, 0, 1);
        //    vec3 colourMix = vec3(clamp(colourA * colourAMix + colourB * colourBMix, 0, 1));
        //
        //    color = vec4(colourMix.xyz,1);
        //    break;
        //}

        t += d;
    }

    glColor = color;
}