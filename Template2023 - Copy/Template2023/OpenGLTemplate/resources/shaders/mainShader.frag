#version 400 core

struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	vec3 sMa;
	vec3 sMd;
	vec3 sMs;
	float shininess;
};

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

struct SpotLightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 direction;
	float exponent;
	float cutoff;
};


out vec4 vOutputColour;

in float fLevels;

in vec4 P;
in vec3 n;

uniform LightInfo light1; 
uniform SpotLightInfo spotLight1; 
uniform SpotLightInfo spotLight2; 
uniform MaterialInfo material1;
uniform bool renderSkybox;
uniform samplerCube CubeMapTex;
in vec3 worldPosition;
bool spotlit;

vec3 PhongModel(vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light1.position - p));
	vec3 v = normalize(-p.xyz);
	vec3 r = reflect(-s, n);
	vec3 h = normalize(v + s);
	vec3 ambient = light1.La * material1.Ma;
	float sDotN = max(dot(s, n), 0.0);
	vec3 diffuse = light1.Ld * material1.Md * sDotN;
	vec3 specular = vec3(0.0);
	if (sDotN > 0.0)
		specular = light1.Ls * material1.Ms * pow(max(dot(h, n), 0.0), material1.shininess);
	
	return ambient + diffuse + specular;

}

vec3 BlinnPhongSpotlightModel(SpotLightInfo light, vec4 p, vec3 n)
{
	vec3 s = normalize(vec3(light.position - p));
	float angle = acos(dot(-s, light.direction));
	float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	vec3 ambient = light.La * material1.sMa;
	float fragmentDist = length((light.position).xyz - P.xyz);

	if (angle < cutoff && fragmentDist < 120) {
		float spotFactor = pow(dot(-s, light.direction), light.exponent);
		vec3 v = normalize(-p.xyz);
		vec3 h = normalize(v + s);
		float sDotN = max(dot(s, n), 0.0);
		vec3 diffuse = light.Ld * material1.sMd * sDotN;
		vec3 specular = vec3(0.0);
		if (sDotN > 0.0)
			specular = light.Ls * material1.sMs * pow(max(dot(h, n), 0.0),
				material1.shininess);
		spotlit = true;
		return ambient + spotFactor * (diffuse + specular);
	} 
	else
		if(!spotlit){
			spotlit = false;
		}
		
		return ambient;
}

in vec2 vTexCoord;	

uniform float u_colorFactor;

uniform bool bUseTexture;
uniform sampler2D sampler0;  // The texture sampler

void main()
{	
	//vOutputColour = vec4(vColour, 0.5);

	if (renderSkybox)
	{
		
		vOutputColour = texture(CubeMapTex, worldPosition);
		float grey = 0.21 * vOutputColour.r + 0.71 * vOutputColour.g + 0.07 * vOutputColour.b;
		vOutputColour = vec4(vOutputColour.r * u_colorFactor + grey * (1.0 - u_colorFactor), 
			vOutputColour.g * u_colorFactor + grey * (1.0 - u_colorFactor), vOutputColour.b * u_colorFactor + 
			grey * (1.0 - u_colorFactor), 1.0);

	}
	
	else if (bUseTexture)
	{
		vec3 vColour = PhongModel(P, normalize(n));
		vec3 bColour = BlinnPhongSpotlightModel(spotLight1, P, normalize(n));
		bColour += BlinnPhongSpotlightModel(spotLight2, P, normalize(n));
		vColour += bColour;
		vec4 vTexColour = texture(sampler0, vTexCoord);	
		vec3 v = normalize(-P.xyz);

		//greyscale color toonify
		vec3 quantisedColour = (floor(vColour * fLevels)) / fLevels;
		quantisedColour = vec3(clamp(quantisedColour.x, vColour.x*0.7, vColour.x),
			clamp(quantisedColour.y, vColour.y*0.7, vColour.y),
			clamp(quantisedColour.z, vColour.z*0.7, vColour.z));
		vec3 toonColour = quantisedColour;
		vTexColour = vTexColour*vec4(toonColour, 1.0f);

		if(!spotlit){
			float grey = 0.21 * vTexColour.r + 0.71 * vTexColour.g + 0.07 * vTexColour.b;
			vOutputColour = vec4(vTexColour.r * u_colorFactor + grey * (1.0 - u_colorFactor), 
				vTexColour.g * u_colorFactor + grey * (1.0 - u_colorFactor), vTexColour.b * u_colorFactor + 
				grey * (1.0 - u_colorFactor), 1.0);
		}
		
		else{
			vOutputColour = vTexColour;
		}


	}
	else{
		vec3 vColour = PhongModel(P, normalize(n));
		vec3 v = normalize(-P.xyz);
		float edgeMask = (dot(v, n) < 0.4) ? 0 : 1;
		vec3 quantisedColour = (floor(vColour * fLevels)) / fLevels;
		quantisedColour = vec3(clamp(quantisedColour.x, vColour.x*0.7, vColour.x),
			clamp(quantisedColour.y, vColour.y*0.7, vColour.y),
			clamp(quantisedColour.z, vColour.z*0.7, vColour.z));
		vec3 toonColour = edgeMask * quantisedColour;

		float grey = 0.21 * toonColour.r + 0.71 * toonColour.g + 0.07 * toonColour.b;
		vOutputColour = vec4(toonColour.r * u_colorFactor + grey * (1.0 - u_colorFactor), 
			toonColour.g * u_colorFactor + grey * (1.0 - u_colorFactor), 
			toonColour.b * u_colorFactor + grey * (1.0 - u_colorFactor), 1.0);
	}


}

