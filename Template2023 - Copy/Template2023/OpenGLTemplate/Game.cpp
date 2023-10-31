/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler warnings
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "tetrahedron.h"
#include "cube.h"
#include "CatmullRom.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pBench = NULL;
	m_pHorseMesh = NULL;
	m_pCarMesh = NULL;
	m_pStreetLight = NULL;

	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pTetrahedron = NULL;
	m_pCube = NULL;
	m_pCatmullRom = NULL;
	m_dt = 0.0;
	m_at = 0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_pTree = NULL;
	Isday = true;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pBench;
	delete m_pCarMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pTetrahedron;
	delete m_pCube;
	delete m_pCatmullRom;
	delete m_pStreetLight;

	delete m_pTree;

	m_endTime = 0.f;


	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pCarMesh = new COpenAssetImportMesh;
	m_pBench = new COpenAssetImportMesh;
	m_pTree = new COpenAssetImportMesh;
	m_pStreetLight = new COpenAssetImportMesh;
	m_pCatmullRom = new CCatmullRom;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pTetrahedron = new Ctetrahedron;
	m_pCube = new Ccube;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShader.vert");
	sShaderFileNames.push_back("sphereShader.frag");
	
	for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\" + sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram* pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram* pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the sphere shader program
	CShaderProgram* pSphereProgram = new CShaderProgram;
	pSphereProgram->CreateProgram();
	pSphereProgram->AddShaderToProgram(&shShaders[4]);
	pSphereProgram->AddShaderToProgram(&shShaders[5]);
	pSphereProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSphereProgram);




	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pCarMesh->Load("resources\\models\\Car.dae");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pTree->Load("resources\\models\\tree.dae");
	m_pBench->Load("resources\\models\\bench.dae");
	m_pStreetLight->Load("resources\\models\\StreetLight.dae");

	m_pTetrahedron->Create("resources\\textures\\", "old_paper.jpg");  // Texture downloaded from https://opengameart.org/content/paper on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	m_pCube->Create("resources\\textures\\", "Powerup.jpg");  // Texture downloaded from https://opengameart.org/content/paper on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetBounds();
	m_pCatmullRom->CreateWallPoints();
	m_pCatmullRom->CreateTrack("resources\\textures\\Road_2.png"); //Downloaded from https://opengameart.org/content/4-hand-painted-ground-textures-dirt9png on March 27th
	m_pCatmullRom->CreateRightWall1Surface("resources\\textures\\grass.png"); //Downloaded from https://opengameart.org/content/hand-painted-sand-texture-0 on March 27th
	m_pCatmullRom->CreateRightWall2Surface("resources\\textures\\grass.png");
	m_pCatmullRom->CreateRightWall3Surface("resources\\textures\\grass.png"); //Downlaoded from https://opengameart.org/content/2k-handpainted-style-textures-hp04png
	m_pCatmullRom->CreateBottomWallSurface("resources\\textures\\grass.png");
	m_pCatmullRom->CreateLeftWall3Surface("resources\\textures\\grass.png");
	m_pCatmullRom->CreateLeftWall2Surface("resources\\textures\\grass.png");
	m_pCatmullRom->CreateLeftWall1Surface("resources\\textures\\grass.png");

	for (int i = 0; i < m_NumCubes; i++)
	{
		int random_place = rand() % (int)m_pCatmullRom->getWidth() / 2;
		if (i % 2) 
		{
			random_place = -random_place;
		}
		m_CubeOffset.push_back((float)random_place);
	}

	for (int i = 0; i < m_numOfSpikes; i++) 
	{
		srand(3);
		int random_place = rand() % (int)m_pCatmullRom->getWidth() / 2;
		if (i % 2) 
		{
			random_place = -random_place;
		}
		m_spikeOffset.push_back((float)random_place);
	}



}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	

	float daylight;
	float color = 1.0f;
	float spotlight;

	if (Isday == true) 
	{
		daylight = 1.f;
		color = 1.f;
		spotlight = 0.f;
	}
	else if (Isday == false)
	{
		daylight = 0.25f;
		color = 0.f;
		spotlight = 1.f;
	}


	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	glm::vec4 spotlightPosition1 = glm::vec4(m_playerPosition + m_playerSide * -1.f + m_playerUp + m_playerDirection * 2.f, 1.0);
	glm::vec4 spotlightPosition2 = glm::vec4(m_playerPosition + m_playerSide * 0.f + m_playerUp + m_playerDirection * 2.f, 1.0);
	pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(daylight));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(daylight));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(daylight));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.9f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 30.0f);		// Shininess material property
	pMainProgram->SetUniform("levels", 4);
	pMainProgram->SetUniform("u_colorFactor", color);

	//SpotLight centered on player car
	pMainProgram->SetUniform("spotLight1.position", viewMatrix * spotlightPosition1); // Light1 position in eye coordinates
	pMainProgram->SetUniform("spotLight2.position", viewMatrix * spotlightPosition2); // Light2 position in eye coordinates
	pMainProgram->SetUniform("spotLight1.La", glm::vec3(spotlight, spotlight, 0.0f));
	pMainProgram->SetUniform("spotLight1.Ld", glm::vec3(spotlight, spotlight, 0.0f));
	pMainProgram->SetUniform("spotLight1.Ls", glm::vec3(spotlight, spotlight, 01.0f));
	pMainProgram->SetUniform("material1.sMa", glm::vec3(0.001f));	// Spotlight Ambient material reflectance
	pMainProgram->SetUniform("material1.sMd", glm::vec3(0.9f));	// SpotLight Diffuse material reflectance
	pMainProgram->SetUniform("material1.sMs", glm::vec3(1.0f));	// SpotLight Specular material reflectance
	pMainProgram->SetUniform("spotLight1.direction", glm::normalize(viewNormalMatrix * m_playerDirection));
	pMainProgram->SetUniform("spotLight1.exponent", 10.0f);
	pMainProgram->SetUniform("spotLight1.cutoff", 20.0f);
	pMainProgram->SetUniform("spotLight2.La", glm::vec3(spotlight, spotlight, 0.0f));
	pMainProgram->SetUniform("spotLight2.Ld", glm::vec3(spotlight, spotlight, 0.0f));
	pMainProgram->SetUniform("spotLight2.Ls", glm::vec3(spotlight, spotlight, 0.0f));
	pMainProgram->SetUniform("spotLight2.direction", glm::normalize(viewNormalMatrix * m_playerDirection));
	pMainProgram->SetUniform("spotLight2.exponent", 10.0f);
	pMainProgram->SetUniform("spotLight2.cutoff", 20.0f);
	pMainProgram->SetUniform("t", m_at * 2);
	pMainProgram->SetUniform("levels", 4);
	pMainProgram->SetUniform("u_colorFactor", color);
	pMainProgram->SetUniform("bUseTexture", true);


		

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	




	//// Render the Car 
	//modelViewMatrixStack.Push();
	//glm::vec3 p;
	//glm::vec3 pNext;
	//m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext);
	//m_pCatmullRom->Sample(m_currentDistance, p);
	//glm::vec3 t = glm::normalize(pNext - p);
	//glm::vec3 b = glm::vec3(0, 1, 0);
	//glm::vec3 n = glm::normalize(glm::cross(t, b));

	//m_currentDistance * 0.1f;
	//m_currentDistance += .05f * (float)m_dt;
	//m_pCatmullRom->Sample(m_currentDistance, p);
	////m_pCamera->Set(p + b * 10.0f, p + t * 20.0f + b * 5.0f, glm::vec3(0, 1, 0));
	////modelViewMatrixStack.LookAt(p + b * 10.0f, p + t * 20.0f + b * 5.0f, glm::vec3(0, 1, 0));
	//modelViewMatrixStack.Translate(p + t);
	//modelViewMatrixStack.Rotate(glm::vec3(.0f, 1.0f, 0.0f), 180.0f);

	//modelViewMatrixStack.Scale(2.5f);
	//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//m_pCarMesh->Render();
	//modelViewMatrixStack.Pop();
	
	//Render Car
	modelViewMatrixStack.Push();
	glm::vec3 dum_pos = glm::vec3(60.0f, 0.0f, 50.0f);
	modelViewMatrixStack.Translate(m_playerPosition);
	//modelViewMatrixStack *= m_playerOrientation;
	modelViewMatrixStack.ApplyMatrix(m_playerOrientation);
	modelViewMatrixStack.Scale(1.f);
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

	m_pCarMesh->Render();
	modelViewMatrixStack.Pop();

	
	//// Render the barrel 
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
	//	modelViewMatrixStack.Scale(5.0f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pBarrelMesh->Render();
	//modelViewMatrixStack.Pop();
	//

	//// Render the sphere
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
	//	modelViewMatrixStack.Scale(2.0f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//	//pMainProgram->SetUniform("bUseTexture", false);
	//	m_pSphere->Render();
	//modelViewMatrixStack.Pop();
	/*
	modelViewMatrixStack.Push();
	//pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pCatmullRom->RenderCentreline();
	// Render your object here
	modelViewMatrixStack.Pop();
	*/
	////Render the tetrahedron
	//modelViewMatrixStack.Push();
	//modelViewMatrixStack.Translate(glm::vec3(71, 5, -71));
	//modelViewMatrixStack.Scale(2.0f);
	//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	////pMainProgram->SetUniform("bUseTexture", false);
	//m_pTetrahedron->Render();
	//modelViewMatrixStack.Pop();


	//// Render the cube
	//modelViewMatrixStack.Push();
	//modelViewMatrixStack.Translate(glm::vec3(5.0, 2, 0));
	//modelViewMatrixStack.Scale(2.0f);
	//pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	////pMainProgram->SetUniform("bUseTexture", false);
	//m_pCube->Render();
	//modelViewMatrixStack.Pop();


	float spikeInterval = m_pCatmullRom->getMaxDistance() / m_numOfSpikes;
	for (int x = 1; x <= m_numOfSpikes; x++)
	{
		float spikeDistance = spikeInterval * x;
		float viewDistance = spikeDistance - 0.1f;
		glm::vec3 p;
		glm::vec3 pNext;
		glm::vec3 pUp;
		m_pCatmullRom->Sample(spikeDistance, p, pUp);
		m_pCatmullRom->Sample(viewDistance, pNext);
		

		glm::vec3 T = glm::normalize(pNext - p);;
		glm::vec3 N = glm::normalize(glm::cross(T, pUp));
		glm::vec3 B = glm::normalize(glm::cross(T, N));


		modelViewMatrixStack.Push();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 pos = p + N * m_spikeOffset[x - 1];

		if (m_spikePosition.size() < m_numOfSpikes) 
		{
			m_spikePosition.push_back(pos);
		}

		modelViewMatrixStack.Translate(pos);
		modelViewMatrixStack *= glm::mat4(glm::mat3(T, -B, N));

		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		modelViewMatrixStack.Scale(5.f);
		m_pTetrahedron->Render();
		modelViewMatrixStack.Pop();
	}

	float cubeInterval = m_pCatmullRom->getMaxDistance() / m_NumCubes;
	for (int x = 1; x <= m_NumCubes; x++) 
	{
		float ring_distance = cubeInterval * x;
		float viewDistance = ring_distance - 0.1f;
		glm::vec3 p;
		glm::vec3 pNext;
		glm::vec3 pUp;
		m_pCatmullRom->Sample(ring_distance, p, pUp);
		m_pCatmullRom->Sample(viewDistance, pNext);

		glm::vec3 T = glm::normalize(pNext - p);
		glm::vec3 N = glm::normalize(glm::cross(T, pUp));
		glm::vec3 B = glm::normalize(glm::cross(T, N));

		modelViewMatrixStack.Push();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::vec3 pos = p + N * m_CubeOffset[x - 1];

		if (m_CubePosition.size() < m_NumCubes) {
			m_CubePosition.push_back(pos);
		}

		modelViewMatrixStack.Translate(p + (-B * 2.f) + N * m_CubeOffset[x - 1]);
		modelViewMatrixStack.Scale(1.0f);
		pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix",m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCube->Render();
		modelViewMatrixStack.Pop();
	}
	

	//Render the Tree
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(100, 1.5, 30));
	modelViewMatrixStack.Scale(2.0f);
	modelViewMatrixStack.Rotate(glm::vec3(1, 1, 1), 180.f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pTree->Render();
	modelViewMatrixStack.Pop();

	//Render the Tree
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(80, 1.5, -20));
	modelViewMatrixStack.Scale(2.0f);
	//modelViewMatrixStack.Rotate(glm::vec3(1, 1, 1), 180.f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pStreetLight->Render();
	modelViewMatrixStack.Pop();

	//Render the Bench
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(100, 2, 0));
	modelViewMatrixStack.Scale(5.0f);
	modelViewMatrixStack.Rotate(glm::vec3(1, 1, 1), 180.f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pBench->Render();
	modelViewMatrixStack.Pop();
	

	

	

// Render CatmullRom
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("bUseTexture", false); // turn off texturing
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix",
	m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// Render your object here

	pMainProgram->SetUniform("bUseTexture", true);
		
	m_pCatmullRom->RenderTrack();
	m_pCatmullRom->RenderRightWall1Surface();
	m_pCatmullRom->RenderRightWall2Surface();
	m_pCatmullRom->RenderRightWall3Surface();
	m_pCatmullRom->RenderBottomWallSurface();
	m_pCatmullRom->RenderLeftWall3Surface();
	m_pCatmullRom->RenderLeftWall2Surface();
	m_pCatmullRom->RenderLeftWall1Surface();

	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();
	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];
	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	fontProgram->UseProgram();


	//Lap text shadow
	if (!isGameDone)
	{
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_pFtFont->Render(20, height - 78, 40, "Lap: %i / 3", m_pCatmullRom->CurrentLap(m_currentDistance) + 1);
		modelViewMatrixStack.Pop();

		//Lap Text Rendered over shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.0f, 0.5f, 1.0f));
		m_pFtFont->Render(18, height - 80, 40, "Lap: %i / 3", m_pCatmullRom->CurrentLap(m_currentDistance) + 1);
		modelViewMatrixStack.Pop();

		// Timer Text Shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.f, 0.f, 1.0f));
		if (int(m_t) % 60 < 10) {
			m_pFtFont->Render(602, height - 80, 40, "%i : 0%i", int(m_t / 60), int(m_t) % 60);
		}
		else {
			m_pFtFont->Render(602, height - 80, 40, "%i : %i", int(m_t / 60), int(m_t) % 60);
		}

		modelViewMatrixStack.Pop();

		// Timer Text Rendered over shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.f, 1.f, 1.0f));
		if (int(m_t) % 60 < 10) {
			m_pFtFont->Render(600, height - 80, 40, "%i : 0%i", int(m_t / 60), int(m_t) % 60);
		}
		else {
			m_pFtFont->Render(600, height - 80, 40, "%i : %i", int(m_t / 60), int(m_t) % 60);
		}

		modelViewMatrixStack.Pop();
	}
	else
	{
		// Fin Title Text Shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.f, 0.f, 1.0f));
		m_pFtFont->Render(304, height - 80, 80, "FINISH :)");
		modelViewMatrixStack.Pop();

		//Fin title text rendered over shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.f, 1.f, 1.0f));
		m_pFtFont->Render(300, height - 80, 80, "FINISH :)");
		modelViewMatrixStack.Pop();


		// Finish time Text shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 0.f, 0.f, 1.0f));
		if (int(m_t) % 60 < 10) {
			m_pFtFont->Render(284, height - 300, 70, "%i : 0%i", int(m_endTime / 60), int(m_t) % 60);
		}
		else {
			m_pFtFont->Render(284, height - 300, 70, "%i : %i", int(m_endTime / 60), int(m_t) % 60);
		}
		modelViewMatrixStack.Pop();

		// Finish time Text Rendered over shadow
		modelViewMatrixStack.Push();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(0.0f, 1.f, 1.f, 1.0f));
		if (int(m_t) % 60 < 10) {
			m_pFtFont->Render(280, height - 300, 70, "%i : 0%i", int(m_endTime / 60), int(m_t) % 60);
		}
		else {
			m_pFtFont->Render(280, height - 300, 70, "%i : %i", int(m_endTime / 60), int(m_t) % 60);
		}
		modelViewMatrixStack.Pop();
	}



	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}

// Update method runs repeatedly with the Render method
void Game::Update() 
{

	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	//m_pCamera->Set(glm::vec3(0, 300, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

	//m_t += (float)(0.01f * m_dt);

	glm::vec3 p;
	glm::vec3 pNext;
	glm::vec3 pNextUp;
	glm::vec3 pNextNext;
	glm::vec3 viewPlayerDirection;
	glm::vec3 pUp;

	if (!isGamePaused)
	{
		m_currentDistance += m_dt * m_carSpeed;
	}
	float viewDistance = m_currentDistance + 10.f;
	float playerViewDistance = viewDistance + 1.f;

	m_pCatmullRom->Sample(m_currentDistance, p, pUp);
	m_pCatmullRom->Sample(viewDistance, pNext, pNextUp);
	m_pCatmullRom->Sample(playerViewDistance, pNextNext);
	viewPlayerDirection = glm::normalize(pNextNext - pNext);

	glm::vec3 t = glm::normalize(pNext - p);
	glm::vec3 n = glm::normalize(glm::cross(t, pUp));
	glm::vec3 b = glm::normalize(glm::cross(t, n));


	glm::vec3 PT = viewPlayerDirection;
	glm::vec3 PN = glm::normalize(glm::cross(PT, pNextUp));
	glm::vec3 PB = glm::normalize(glm::cross(PT, PN));

	//m_pCamera->Update(m_dt);

	m_playerPosition = pNext + PN * 1.5f + PN;
	m_playerOrientation = glm::mat4(glm::mat3(PN, -PB, -PT));
	m_playerDirection = PT;
	m_playerUp = -PB;
	m_playerSide = PN;

	//Third Person View
	if (camView == 0 && !isGamePaused) {
		glm::vec3 new_camPos = p + (7.f * -b);
		m_pCamera->Set(new_camPos, p + (10.f * t) + 2.f * -b, -b);
	}

	//First Person View
	else if (camView == 1 && !isGamePaused)
	{
		glm::vec3 new_camPos = m_playerPosition - PN * 2.f + -PB * 2.3f + PT * 2.f;
		m_pCamera->Set(new_camPos, m_playerPosition + (20.f * viewPlayerDirection) + 2.f * -b, -PB);
	}


	//Top View
	else if (camView == 2 && !isGamePaused)
	{
		glm::vec3 new_camPos = m_playerPosition + -PN * 1.f + glm::vec3(0, 1, 0) * 50.f;
		m_pCamera->Set(new_camPos, m_playerPosition + -PN * 1.f, m_playerDirection);
	}

	//Free Camera View
	else if (camView == 2 && !isGamePaused)
	{
		if (PausePass) 
		{
			m_pCamera->Set(m_pCamera->GetPosition(), glm::vec3(0, 0, 0),glm::vec3(0, 1, 0));
			PausePass = false;
		}
		else 
		{
			m_pCamera->Set(m_pCamera->GetPosition(), m_pCamera->GetView(),
			glm::vec3(0, 1, 0));
		}

		m_pCamera->Update(m_dt);
	}


	// Control player turn amount
	float spare_dist = m_currentTurnDistance;
	if (m_RightTurn) 
	{
		m_currentTurnDistance += m_dt * m_TurnSpeed;
		if (m_currentTurnDistance > m_pCatmullRom->getWidth() / 2) 
		{
			m_currentTurnDistance = spare_dist;
		}
	}
	else if (m_leftTurn) 
	{
		m_currentTurnDistance -= m_dt * m_TurnSpeed;
		if (m_currentTurnDistance < -m_pCatmullRom->getWidth() / 2)
		{
			m_currentTurnDistance = spare_dist;
		}
	}

	if (!isGamePaused && !isGameDone) 
	{
		m_playerPosition = pNext + PN * 1.5f + PN * m_currentTurnDistance;
		m_playerOrientation = glm::mat4(glm::mat3(PN,-PB,-PT));
		m_playerDirection = PT;
		m_playerUp = -PB;
		m_playerSide = PN;

		for (int pos = 0; pos < m_spikePosition.size(); pos++) {
			float length = glm::length(m_playerPosition - m_spikePosition[pos]);

			if (length < 3.5f) {
				float spare_speed = m_carSpeed;
				m_carSpeed -= 0.0001f * (float)m_dt;
				if (m_carSpeed <= 0.025f) {
					m_carSpeed = spare_speed;
				}
			}
		}

		for (int pos = 0; pos < m_CubePosition.size(); pos++) {
			float length = glm::length(m_playerPosition - m_CubePosition[pos]);

			if (length < 4.f) {
				float spare_speed = m_carSpeed;
				m_carSpeed += 0.0001f * (float)m_dt;
				if (m_carSpeed >= 0.075f) {
					m_carSpeed = spare_speed;
				}
				else 
				{
					//m_pt = 0;
					m_accelerate = true;
				}
			}
		}
	}

	if (!isGameDone && !isGamePaused) 
	{
		m_t += 0.001f * (float)m_dt; // Timer float
	}


	if (3 == m_pCatmullRom->CurrentLap(m_currentDistance) && !isGameDone)
	{ // Set up game end screen
		isGameDone = !isGameDone;
		camView = 0;
		m_endTime = m_t;
	}
	m_at += 0.01f * (float)m_dt; // increment animation timer
	m_pAudio->Update();
}



void Game::DisplayFrameRate()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;


	case WM_KEYDOWN:
		switch (w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'A':
			if (!isGameDone) {
				m_leftTurn = true;
			}
			break;
		case 'D':
			if (!isGameDone) {
				m_RightTurn = true;
			}
			break;
		case '0':
			if (!isGameDone) {
				camView += 1;
				if (camView > 3) {
					camView = 0;
					if (!PausePass) {
						PausePass = true;
					}
				}
			}
			break;

		case '6':
			if (!isGameDone)
			{
				Stop = !Stop;
				if (!PausePass) 
				{
					PausePass = true;
				}
			}
			break;
		case VK_F1:
			//m_pAudio->PlayEventSound();
			break;

		case '9':
			Isday = !Isday;
			break;
		}
		break;

	case WM_KEYUP:
		switch (w_param) {

		case 'A':
			m_leftTurn = false;
			break;
		case 'D':
			m_RightTurn = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return int(game.Execute());
}
