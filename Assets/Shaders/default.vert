#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;

out vec3 vertexColor;
out vec2 texCoord;
out float timeValue;
out vec3 fragPos;
out vec3 cameraPos;
out mat3 TBN;
out vec3 meshNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 cameraPosition;

uniform samplerBuffer framesBuffer;

//Rotation XYZ;

mat4 rotateX(float angle) 
{
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0,  cos_angle,  -sin_angle,  0.0,
		0.0,  sin_angle,   cos_angle,  0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rotateY(float angle) 
{
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);
	return mat4(
		cos_angle,  0.0,  sin_angle,  0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin_angle,  0.0,  cos_angle,  0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

mat4 rotateZ(float angle) 
{
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);
	return mat4(
		cos_angle,  -sin_angle,  0.0, 0.0,
		sin_angle,   cos_angle,  0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

//Translate XYZ;

mat4 translate(float x, float y, float z) 
{
	return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0
	);
}

void main()
{	
	mat4 localTransform = translate(384.0, 0.0 + (sin(time * 50.0) * 20.0), 0.0) * rotateY(time * 20.0);
	mat4 fullTransform = model * localTransform;
	
	vec3 T = normalize(mat3(fullTransform) * aTangent);
	vec3 N = normalize(mat3(fullTransform) * aNormal);
	vec3 B = cross(N, T);
	
	meshNormal = N;
	
	TBN = mat3(T, B, N);
	
	timeValue = time;
	cameraPos = vec3(0.0, 0.0, 1.0);
	
	vec4 worldPos = fullTransform * vec4(aPos, 1.0);
    fragPos = worldPos.xyz;
	
	gl_Position = projection * view * worldPos;
	vertexColor = aColor;
	texCoord = vec2(aUV.x, aUV.y);
}