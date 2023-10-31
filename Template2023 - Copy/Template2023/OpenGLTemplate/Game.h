#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class Ctetrahedron;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class Ccube;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox* m_pSkybox;
	CCamera* m_pCamera;
	vector <CShaderProgram*>* m_pShaderPrograms;
	CPlane* m_pPlanarTerrain;
	CFreeTypeFont* m_pFtFont;
	COpenAssetImportMesh* m_pBarrelMesh;
	COpenAssetImportMesh* m_pHorseMesh;
	COpenAssetImportMesh* m_pCarMesh;
	COpenAssetImportMesh* m_pTrafficLight;
	COpenAssetImportMesh* m_pTree;
	COpenAssetImportMesh* m_pStreetLight;
	COpenAssetImportMesh* m_pBench;

	Ccube* m_pCube;
	CSphere* m_pSphere;
	Ctetrahedron* m_pTetrahedron;

	CHighResolutionTimer* m_pHighResolutionTimer;
	CAudio* m_pAudio;
	CCatmullRom* m_pCatmullRom;
	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	float m_at; //Animation Timer
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	float m_currentDistance;
	float CarAngle;
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerDirection;
	glm::mat4 m_playerOrientation;
	glm::vec3 m_playerUp;
	glm::vec3 m_playerSide;
	glm::mat4 PlayerRotation = {};

	std::vector< glm::vec3> m_pCatmullPts;

	float m_t;
	float m_endTime;
	bool Isday;
	bool isGameDone;

	bool isGamePaused;
	bool PausePass;
	bool Stop;
	int camView;
	float m_carSpeed = 0.025f;
	float m_TurnSpeed = 0.02f;

	bool m_accelerate;

	bool m_leftTurn;
	bool m_RightTurn;
	float m_currentTurnDistance;

	int m_NumCubes = 15;
	int m_numOfSpikes = 20;

	int m_numOfTrees = 5;

	int spike_pointt = 0;
	std::vector<float> m_CubeOffset;
	std::vector<float> m_spikeOffset;
	std::vector<glm::vec3> m_spikePosition;
	std::vector<glm::vec3> m_CubePosition;
};
