#version 330 core

///////////////////////////////////////// LAYOUT INPUTS
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

///////////////////////////////////////// OUTPUTS
out vec3 normal;
out vec2 texCoord;
out vec3 FragPos;
out vec3 NoisePos;

///////////////////////////////////////// UNIFORMS
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

///////////////////////////////////////// NOISE FUNCTIONS
float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}
///////////////////////////////////////////////////////////////////////



void main()
{

  ///////////////////////////////////////// SEND OUT THE NORMAL
  normal = normalize(mat3(transpose(inverse(model))) * aNorm);  
  
  ///////////////////////////////////////// SEND OUT THE GL_POSITION
  vec3 ws = vec3(model * vec4(aPos,1.0));
  gl_Position =  projection * view * model  *vec4(aPos, 1.0);
	
  FragPos = vec3(model * vec4(aPos,1.0));
  FragPos = vec3(model * vec4(aPos,1.0));
  
  ///////////////////////////////////////// SEND OUT THE TEXTURE COORDINATE
  texCoord = aTexCoord;

};