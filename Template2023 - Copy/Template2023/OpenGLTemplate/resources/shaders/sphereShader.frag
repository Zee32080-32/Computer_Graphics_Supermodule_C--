#version 400 core


struct MaterialInfo
{
	vec3 Ma;
	vec3 Md;
	vec3 Ms;
	float shininess;
};

struct LightInfo
{
	vec4 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};


out vec4 vOutputColour;

//in float fIntensity;
in float fLevels;

in vec4 eyeP;
in vec3 n;

uniform LightInfo light1; 
uniform MaterialInfo material1;

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

void main()
{	
	//vOutputColour = vec4(vColour, 0.5);
	vec3 vColour = PhongModel(eyeP, normalize(n));
	vec3 v = normalize(-eyeP.xyz);
	float edgeMask = (dot(v, n) < 0.4) ? 0 : 1;
	vec3 quantisedColour = (floor(vColour * fLevels)) / fLevels;
	quantisedColour = vec3(clamp(quantisedColour.x, vColour.x*0.7, vColour.x),
		clamp(quantisedColour.y, vColour.y*0.7, vColour.y),
		clamp(quantisedColour.z, vColour.z*0.7, vColour.z));
	vec3 toonColour = edgeMask * quantisedColour;
	
	vOutputColour = vec4(toonColour, 1.f);
}
