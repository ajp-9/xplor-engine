#version 330

out vec4 FragColor;

in vec2 passUV;
in vec3 passNormal;
in vec3 fragPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	#define MAT_SHININESS 64
};

struct DirLight
{
	vec3 direction;
	#define DIR_AMBIENT 0.075
	#define DIR_DIFFUSE vec3(1.0, 1.0, 1.0)
	#define DIR_SPECULAR 1
};

struct PointLight
{
	bool activated;
	vec3 position;
	vec3 diffuse;
	#define PT_SPECULAR 0.75

	#define PT_CONSTANT 1.0
	#define PT_LINEAR 0.9
	#define PT_QUADRATIC .36
};

struct SpotLight // position is campos
{
	bool activated;
	vec3 direction;
	#define SPT_DIFFUSE vec3(1.0, 1.0, 1.0)
	#define SPT_INNER_CUTOFF 9.0
	#define SPT_OUTER_CUTOFF 14.0
	#define SPT_SPECULAR 0.65

	#define SPT_CONSTANT 1.0
	#define SPT_LINEAR 0.9
	#define SPT_QUADRATIC .16
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[5];
uniform SpotLight spotLight;

uniform vec3 viewPos;
uniform vec3 camPos;

vec4 diffuseMap = texture(material.diffuse, passUV);
vec3 specularMap = texture(material.specular, passUV).rgb;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	if(diffuseMap.a == 0) { discard; }

	vec3 normal = passNormal;
	vec3 viewDir = normalize(camPos - fragPos);

	// directional lighting
	vec3 result = calcDirLight(dirLight, normal, viewDir);

	// iterate through point lights
	for(int i = 0; i < 5; i++)
	{
		if(pointLights[i].activated)
			result += calcPointLight(pointLights[i], normal, fragPos, viewDir);
	}

	//spotlight
	if(spotLight.activated)
		result += calcSpotLight(spotLight, normal, fragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), MAT_SHININESS);

	vec3 ambient = DIR_AMBIENT * diffuseMap.rgb;
	vec3 diffuse = DIR_DIFFUSE * diff * diffuseMap.rgb;
	vec3 specular = DIR_SPECULAR * spec * specularMap;
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), MAT_SHININESS);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (PT_CONSTANT + PT_LINEAR * distance + 
	PT_QUADRATIC * (distance * distance));

	vec3 diffuse  = light.diffuse  * diff * diffuseMap.rgb;
	vec3 specular = PT_SPECULAR * spec * specularMap;
	diffuse *= attenuation;
	specular *= attenuation;
	return diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(camPos - fragPos);
	// diffuse
    float diff = 3;
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), MAT_SHININESS);
	// attenuation
	float distance = length(camPos - fragPos);
	float attenuation = 1.0 / (SPT_CONSTANT + SPT_LINEAR * distance + SPT_QUADRATIC *
	 (distance * distance));
	// spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = cos(radians(SPT_INNER_CUTOFF)) - cos(radians(SPT_OUTER_CUTOFF));
	float intensity = clamp((theta - cos(radians(SPT_OUTER_CUTOFF))) / epsilon, 0.0, 1.0);
	// combine
	vec3 diffuse = SPT_DIFFUSE * diff * diffuseMap.rgb;
	vec3 specular = SPT_SPECULAR * spec * specularMap;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return diffuse + specular;
}