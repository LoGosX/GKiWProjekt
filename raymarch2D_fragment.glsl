#version 330 core

//http://9bitscience.blogspot.com/2013/07/raymarching-distance-fields_14.html

uniform vec3 camera_direction = vec3(0, 0, 1);
uniform vec3 camera_position = vec3(0, 0, -1);
uniform vec3 camera_up = vec3(0, 1, 0);
uniform vec3 camera_right = vec3(1, 0, 0);

uniform float sphere_radius = 5;

uniform vec3 plane_position = vec3(0, -2, 0);

uniform float fov = 45;

uniform vec2 g_resolution;
uniform float g_rmEpsilon = 1e-5;

uniform float time = 0;

uniform vec3 spherePosition = vec3(0, 5, 0);
uniform float sphereRadius = 1;
uniform vec3 debugSpherePosition = vec3(20, 0, 0);
uniform float debugSphereRadius = 0.1;
uniform vec3 waveCparams = vec3(1, 0.5, 0.5); //parameters of function wave

#define BACKGROUND_COLOR vec3(0)

vec3 light_direction = vec3(0, -1, 1);

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

float sinxcoszDE(vec3 p) {
    return p.y - waveCparams.x * sin(waveCparams.y * p.x)*cos(waveCparams.z * p.z);
}

float DExy(vec3 z)
{
  z.xy = mod((z.xy),1.0)-vec2(0.5); // instance on xy-plane
  return length(z)-0.3;             // sphere DE
}
float DExyz(vec3 z)
{
  z.xyz = mod((z.xyz),1.0)-vec3(0.5); // instance on xy-plane
  return length(z)-0.3;             // sphere DE
}

vec3 sphereNormalxyz(vec3 p) {
    float epsilon = g_rmEpsilon;
    float x = DExyz(vec3(p.x+epsilon,p.y,p.z)) - DExyz(vec3(p.x-epsilon,p.y,p.z));
    float y = DExyz(vec3(p.x,p.y+epsilon,p.z)) - DExyz(vec3(p.x,p.y-epsilon,p.z));
    float z = DExyz(vec3(p.x,p.y,p.z+epsilon)) - DExyz(vec3(p.x,p.y,p.z-epsilon));
    return normalize(vec3(x,y,z));
}

vec3 sphereNormalxy(vec3 p) {
    float epsilon = g_rmEpsilon;
    float x = DExy(vec3(p.x+epsilon,p.y,p.z)) - DExy(vec3(p.x-epsilon,p.y,p.z));
    float y = DExy(vec3(p.x,p.y+epsilon,p.z)) - DExy(vec3(p.x,p.y-epsilon,p.z));
    float z = DExy(vec3(p.x,p.y,p.z+epsilon)) - DExy(vec3(p.x,p.y,p.z-epsilon));
    return normalize(vec3(x,y,z));
}


float mandelbulbDE(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
    int Iterations = 10;
    float Power = 8 + cos(time / (3.14 * 3));
    float Bailout = 1.5;
	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>Bailout) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

float scaledMandelbulbDE(vec3 p, float scale) {
    return mandelbulbDE(p / scale) * scale;
}

vec3 rotateX(vec3 p, float a) {
    return vec3(
        p.x,
        cos(a) * p.y - sin(a) * p.z,
        sin(a) * p.y + cos(a) * p.z
    );
}

vec3 rotateY(vec3 p, float a) {
    return vec3(
        cos(a) * p.x + sin(a) * p.z,
        p.y,
        -sin(a) * p.x + cos(a) * p.z
    );
}

vec3 rotateZ(vec3 p, float a) {
    return vec3(
        cos(a) * p.x - sin(a) * p.y,
        sin(a) * p.x + cos(a) * p.y,
        p.z
    );
}

float sierpinskiDE(vec3 z)
{
    float r;
    int n = 0;
    float Scale = 2;
    int Iterations = 16;
    float rot1 = 0, rot2 = 0;
    while (n < Iterations) {
        z = rotateX(z, radians(rot1));
       if(z.x+z.y<0) z.xy = -z.yx; // fold 1
       if(z.x+z.z<0) z.xz = -z.zx; // fold 2
       if(z.y+z.z<0) z.zy = -z.yz; // fold 3	
        z = rotateY(z, radians(rot2));
       z = z*Scale - vec3(1)*(Scale-1.0);
       n++;
    }
    return (length(z) ) * pow(Scale, -float(n));
}




float radians(float degrees) {
    return degrees / 180 * 3.1415926535;
}

//scale=2
//bailout=1000
float Menger3(vec3 p){
    float x = p.x;
    float y = p.y;
    float z = p.z;
   float r=x*x+y*y+z*z;
   float CX = 2, CY = 4.8, CZ = 1;
   float scale = 1.3;
   float bailout = 1000;
   int i;
   int MI = 1;
   for(i=0;i<MI && r<bailout;i++){
      vec3 rot = rotateY(vec3(x,y,z), radians(25));
      x = abs(rot.x);
      y = abs(rot.y);
      z = abs(rot.z);

      if(x-y<0){float x1=y;y=x;x=x1;}
      if(x-z<0){float x1=z;z=x;x=x1;}
      if(y-z<0){float y1=z;z=y;y=y1;}

      rot = rotateY(vec3(x,y,z), 0);
      x = rot.x;
      y = rot.y;
      z = rot.z;
    
      x=scale*x-CX*(scale-1);
      y=scale*y-CY*(scale-1);
      z=scale*z-CZ*(scale-1);
      
      //z=scale*z;
      //if(z>0.5*CZ*(scale-1)) z-=CZ*(scale-1);
      
      r=x*x+y*y+z*z;
   }
   return (sqrt(x*x+y*y+z*z)-2)*pow(scale, -i);
}

//MODE 1 - sphere and infinite floor
//MODE 2 - infinite XY spheres
//MODE 3 - infinite XYZ spheres
//MODE 5 - mandelbulb
//MODE 6 - sin(x) * cos(z) wave
//MODE 7 - sierpinski triangle
#define MODE 7

#define SPHERE_POSITION vec3(0, 5, 2.5)


//returns distance to mandelbulb and color
vec2 simpleRaymarch(vec3 from, vec3 direction, int max_steps, float FOVperPixel) {
    int steps = 0;
    float td = 0;
    vec3 p = from;
    float d = 0;
    while(steps++ < max_steps) {
        p = from + direction * td;
        float min_dst = max(FOVperPixel * td, g_rmEpsilon);
        #if MODE == 5
        d = scaledMandelbulbDE(p + vec3(0, 0, -200), 150);
        #endif
        #if MODE == 6
        float dsc = sinxcoszDE(p);
        float ds = sdSphere(p - spherePosition, sphereRadius);
        d = min(ds, dsc);
        #endif
        #if MODE == 7
        float dm = sierpinskiDE(p);
        //float dm = Menger3(p);
        float ds = sdSphere(p - spherePosition, sphereRadius);
        float dsd = sdSphere(p - debugSpherePosition, debugSphereRadius);
        d = min(ds, dm);
        d = min(d, dsd);
        #endif
        if(d < min_dst)
            break;
        //if(d < g_rmEpsilon)
        //    break;
        td += d;
    }
    return vec2(td, 1.0 - float(steps) / max_steps);
}

void main()
{
    vec3 eye = camera_position;
    vec3 up = camera_up;
    vec3 right = camera_right;
    vec3 forward = camera_direction;
    float radius = sphere_radius;

    float fov_radians = fov / 180.0 * 3.14;
    float FOVperPixel = 1.0 / max(g_resolution.x, g_resolution.y);
    float f = 0.5 / tan(fov / 2);
    float u = gl_FragCoord.x * 2.0 / g_resolution.x - 1.0;
    float v = gl_FragCoord.y * 2.0 / g_resolution.y - 1.0;
    vec3 p = eye + forward * f + right * u + up * v;
    vec3 ro = eye;
    vec3 rd = normalize(p - eye);
    

    vec4 color = vec4(BACKGROUND_COLOR, 1); // Sky color

    float t = 0.0;
    const int maxSteps = 128;
    float d = 0;
    #if MODE != 5
    for(int i = 0; i < maxSteps; ++i)
    {
        vec3 p = ro + rd * t;
        float min_dst = max(FOVperPixel * t, g_rmEpsilon);

        #if MODE == 1
        float ds = sdSphere(p - SPHERE_POSITION, radius); // Distance to sphere
        float dp = sdPlane(p - plane_position); // Distance to plane
        d = opSmoothUnion(ds, dp, 1);
        if(d < g_rmEpsilon || )
        {
            if(ds < min_dst) 
            {
                color = vec4(float(maxSteps - i) / 2 / maxSteps, 0, float(i) / maxSteps, 1); // Sphere color
                //vec3 grad = gradApprox(p, SPHERE_POSITION, radius);
                //color = vec4(grad, 1);
                break;
            }
            if(dp > 0 && dp < min_dst)
            {
                color = vec4(floor_color(p), 1);
                break;
            }
        }
        #endif
        #if MODE == 2
        float d1 = DExy(p);
        //float d2 = DExy(p + vec3(0, 0, 0.75));
        //float d = opSmoothUnion(d1, d2, 0.5);
        d = DExy(p);
        if(d < min_dst) {
            vec3 normal = sphereNormalxy(p);
            p.xyz -= d * normal;
            float cosinus = dot(normal, light_direction);
            color = vec4(vec3(clamp(-cosinus, 0, 1)), clamp((32-i)/32.0, 0, 1));
            break;
        }
        #endif
        #if MODE == 3
        d = DExyz(p);
        if(d < min_dst) {
            vec3 normal = sphereNormalxyz(p);
            p.xyz -= d * normal;
            float cosinus = dot(normal, light_direction);
            color = vec4(vec3(clamp(-cosinus, 0, 0)), 1);
            break;
        }
        #endif
        #if MODE == 4
        vec2 sceneInfo = SceneInfo(p);
        d = sceneInfo.y;
        float dst = d;
        // Ray has hit a surface
        if (dst < min_dst) {
            float escapeIterations = sceneInfo.x;
            vec3 normal = EstimateNormal(ro - rd*g_rmEpsilon*2);
        
            vec3 _LightDirection = vec3(0, -1, 0);
            float colourAMix = 1;
            float colourBMix = 1;
        
            float colourA = clamp(dot(normal*.5+.5,-_LightDirection), 0, 1);
            float colourB = clamp(maxSteps/16.0, 0, 1);
            vec3 colourMix = vec3(clamp(colourA * colourAMix + colourB * colourBMix, 0, 1));
        
            color = vec4(colourMix.xyz,1);
            break;
        }
        #endif
        t += d;
    }
    #endif
    #if MODE == 5 || MODE == 6 || MODE == 7
    vec2 res = simpleRaymarch(ro, rd, 64, FOVperPixel);
    color = vec4(res.y);
    #endif

    glColor = color;
}