#version 330

out vec4 FragColor;

in vec2 passUV;
in vec3 passNormal;
in vec3 fragPos;

struct Material
{
	sampler2D blendMap;
	sampler2D backDiffuseMap;
	sampler2D rDiffuseMap;
	sampler2D gDiffuseMap;
	sampler2D bDiffuseMap;
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

#define UV_MULTIPLYER 75
vec3 calcDiffuse();
vec3 blendedDiffuse = calcDiffuse();

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
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

vec3 calcDiffuse()
{
	vec3 totalDiffuse;

	vec3 blendDiffuse = texture(material.blendMap, passUV / UV_MULTIPLYER).rgb;

	float backTextureAmount = 1 - (blendDiffuse.r + blendDiffuse.g + blendDiffuse.b);

	vec3 backgroundDiffuse = texture(material.backDiffuseMap, passUV).rgb * backTextureAmount;
	totalDiffuse += backgroundDiffuse;
	vec3 rDiffuse = texture(material.rDiffuseMap, passUV).rgb * blendDiffuse.r;
	totalDiffuse += rDiffuse;
	vec3 gDiffuse = texture(material.gDiffuseMap, passUV).rgb * blendDiffuse.g;
	totalDiffuse += gDiffuse;
	vec3 bDiffuse = texture(material.bDiffuseMap, passUV).rgb * blendDiffuse.b;
	totalDiffuse += bDiffuse;

	return totalDiffuse;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 ambient = DIR_AMBIENT * blendedDiffuse.rgb;
	vec3 diffuse = DIR_DIFFUSE * diff * blendedDiffuse.rgb;
	return ambient + diffuse;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (PT_CONSTANT + PT_LINEAR * distance + 
	PT_QUADRATIC * (distance * distance));

	vec3 diffuse  = light.diffuse  * diff * blendedDiffuse.rgb;
	diffuse *= attenuation;
	return diffuse;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(camPos - fragPos);
	// diffuse
	float diff = 3;
	// attenuation
	float distance = length(camPos - fragPos);
	float attenuation = 1.0 / (SPT_CONSTANT + SPT_LINEAR * distance + SPT_QUADRATIC *
	 (distance * distance));
	// spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = cos(radians(SPT_INNER_CUTOFF)) - cos(radians(SPT_OUTER_CUTOFF));
	float intensity = clamp((theta - cos(radians(SPT_OUTER_CUTOFF))) / epsilon, 0.0, 1.0);
	// combine
	vec3 diffuse = SPT_DIFFUSE * diff * blendedDiffuse.rgb;
	diffuse *= attenuation * intensity;
	return diffuse;
}