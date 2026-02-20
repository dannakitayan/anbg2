#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
in float timeValue;
in vec3 fragPos;
in mat3 TBN;
in vec3 cameraPos;
in vec3 meshNormal;
out vec4 FragColor;

//Samplers;
uniform sampler2D albedo;
uniform sampler2D normal_map;
uniform sampler2D specular_map;
uniform sampler2D environment_map;
uniform sampler2D glossiness_metallic;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

const float ambient_strenght = 0.7f;
const float fresnel_power = 1.0f;
const float fresnel_strenght = 1.0f;
const vec3 specColor = vec3(0.04, 0.04, 0.04);
const vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));

float schlickApproximation(vec3 halfway_direction, vec3 normal, float F0)
{
	normal = normalize(normal);
	float cos_theta = clamp(dot(normal, halfway_direction), 0.0, 1.0);
	
	return F0 + (1.0 - F0) * pow(1.0 - cos_theta, fresnel_power);
}

vec3 blinnPhongSpecular(vec3 halfway_direction, vec3 normal, vec3 specular_color, float glossiness, float schlick_approximation_value)
{
    //"Width" of flare;
    const float flare_max = 256.0; //200.0 -> flare maximum value;
    const float flare_min = 4.0; //4.0 -> flare minimum value;

    //Shiness;
    float shininess = mix(flare_min, flare_max, glossiness); 
    float cos_theta = max(dot(halfway_direction, normalize(normal)), 0.0);

    //Form of flare;
    float specular_value = pow(cos_theta, shininess);

    //Intensity of flare;
    specular_value *= glossiness; 

    //Color of flare;
    return specular_color * max(schlick_approximation_value, 2.0) * specular_value;
}

vec3 metallFakeReflection(float diffuse_value, float metallic_map_value, float strength, float fresnel, vec3 reflection_map)
{
	float metall_reflection_value = diffuse_value;
    metall_reflection_value = clamp(metall_reflection_value, 0.0, 0.76);
    metall_reflection_value *= metallic_map_value;
	metall_reflection_value += fresnel * metallic_map_value;
    
    return reflection_map * metall_reflection_value * strength;
}

vec3 shieldEffect(float diffuse_value, float force, vec3 color, vec3 albedo)
{
	float reflection_value = (1.0 - diffuse_value) * force;
    reflection_value = clamp(reflection_value, 0.0, 0.3);
    vec3 base = albedo * (diffuse_value + (diffuse_value * force));
    float shield_factor = max(reflection_value + (force - diffuse_value), 0.0);
    vec3 shield_color = color * shield_factor;
    return base + shield_color;
}

vec3 ambient_occlusion(float ao_value, vec3 ambient_color, vec3 albedo_color)
{
    return albedo_color * ambient_color * ao_value;
}

vec2 dirToUV(vec3 dir) {
    vec2 uv = vec2(atan(dir.z, dir.x), asin(dir.y));
    uv *= vec2(0.1591549, 0.3183099);
    uv += 0.5;
    return uv;
}

vec3 rotateX(vec3 dir, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return vec3(
        dir.x,
        dir.y * c - dir.z * s,
        dir.y * s + dir.z * c
    );
}

void main()
{
    //Albedo;
    vec4 albedo_map = texture(albedo, texCoord);
	
	//Specular color;
	//vec3 specular_color_map = texture(specular_map, texCoord).rgb;
	
	//Normal map;
    vec3 raw_normal = texture(normal_map, texCoord).xyz;
    vec3 normal = normalize(raw_normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);
	
	//AO;
	float ao_map_value = texture(glossiness_metallic, texCoord).r;
	
	//Glossiness;
    float glossiness_map_value = texture(glossiness_metallic, texCoord).g;
	glossiness_map_value = 1.0 - glossiness_map_value;
	
	//Metallic;
	float metallic_map_value = texture(glossiness_metallic, texCoord).b;
	
	//Ambient color;
	vec3 ambient_light_color = vec3(0.2, 0.2, 0.2);
	
	vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0));
	vec3 lightDir = normalize(lightDirection);
	vec3 H = normalize(viewDir + lightDir);
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = albedo_map.rgb * diff * lightColor * lightIntensity;
	
	//Base;
	vec3 base = albedo_map.rgb * diff * lightColor * lightIntensity;
	
	//Reflection map;
	vec3 reflectDir = reflect(-viewDir, normal);
    vec2 uv = dirToUV(reflectDir);
    vec3 envColor = texture(environment_map, uv).rgb;
	
	//Ambient Occlusion;
	float mesh_diff = max(dot(meshNormal, lightDir), 0.0);
	vec3 ao_map = ambient_occlusion(ao_map_value, ambient_light_color, albedo_map.rgb);
	
	//Specular;
	float fresnel = schlickApproximation(H, normal, 0.04);
	vec3 specular = blinnPhongSpecular(H, normal, envColor, glossiness_map_value, fresnel);
	
	//Fake metallic;
	float metallic_fresnel_value = mix(0.04, 0.8, metallic_map_value);
	float metallic_fresnel = schlickApproximation(H, normal, metallic_fresnel_value);
    vec3 metallic = metallFakeReflection(diff, metallic_map_value, 0.45, metallic_fresnel, envColor);
	
	vec3 color = diffuse + ao_map + metallic + specular;
	vec3 color3 = shieldEffect(diff, 0.25, vec3(0.48235, 0.48235, 0.68627), color);
    FragColor = vec4(color, albedo_map.a);
}