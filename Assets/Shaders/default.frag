#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
in float timeValue;
in vec3 fragPos;
out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D normal_map;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

const float ambient = 0.2f;

void main()
{
	// Albedo;
	vec4 albedo = texture(texture0, texCoord);
	
	// A vector from a light source;
	vec3 source_position = vec3(lightPos.x, lightPos.y, lightPos.z);
	source_position.z = lightRadius * 0.6;
    vec3 light_vector = source_position - vec3(fragPos);
	float distance = length(light_vector);
	vec3 light = light_vector / max(distance, 0.0001);
	
	// Correct normal map;
	vec3 raw_normal = texture(normal_map, texCoord).xyz;
	float normal_x = raw_normal.x * 2.0 - 1.0;
	float normal_y = 1.0 - raw_normal.y * 2.0;
	float normal_z = raw_normal.z * 2.0 - 1.0;
	vec3 normal = vec3(normal_x, normal_y, normal_z);
	normal = normalize(normal);
	
	//Illuminance;
	float illuminance = clamp(dot(normal, light), 0.0, 1.0) * lightIntensity;
	
	//Radial falloff;
	float falloff = pow(clamp(1.0 - distance / lightRadius, 0.0, 1.0), 2.0);
	
	//Final illuminance value;
	float final_illuminance = ambient + illuminance * falloff;

	vec3 result = albedo.rgb * (lightColor * final_illuminance);
	FragColor = vec4(result, albedo.a);
}