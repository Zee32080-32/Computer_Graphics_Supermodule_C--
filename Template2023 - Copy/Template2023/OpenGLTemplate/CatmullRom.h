#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	CCatmullRom();
	~CCatmullRom();

	void CreateCentreline();
	void RenderCentreline();

	void CreateOffsetBounds();
	void RenderOffsetCurves();

	void CreateTrack(string filename);
	void RenderTrack();


	void CreateWallPoints();
	void RenderWallPoints();
	/*void CreatePipePoints();
	void RenderPipePoints();*/

	void CreateRightWall1Surface(string filename);
	void RenderRightWall1Surface();

	void CreateRightWall2Surface(string filename);
	void RenderRightWall2Surface();

	void CreateRightWall3Surface(string filename);
	void RenderRightWall3Surface();

	void CreateBottomWallSurface(string filename);
	void RenderBottomWallSurface();

	void CreateLeftWall3Surface(string filename);
	void RenderLeftWall3Surface();

	void CreateLeftWall2Surface(string filename);
	void RenderLeftWall2Surface();

	void CreateLeftWall1Surface(string filename);
	void RenderLeftWall1Surface();

	int CurrentLap(float d); // Return the currvent lap (starting from 0) based on distance along the control curve.

	bool Sample(float d, glm::vec3& p, glm::vec3& up = _dummy_vector); // Return a point on the centreline based on a certain distance along the control curve.

	glm::vec3 Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t);

	void Release();

	float getMaxDistance() { return m_distances[m_distances.size() - 1]; };
	float getWidth() { return width; };



private:

	void SetControlPoints();
	void ComputeLengthsAlongControlPoints();
	void UniformlySampleControlPoints(int numSamples);


	vector<float> m_distances;
	CTexture m_texture;
	//CTexture m_pipeRightLeftTexture;
	CTexture m_rightWall1Texture;
	CTexture m_rightWall2Texture;
	CTexture m_rightWall3Texture;
	CTexture m_bottomWallTexture;
	CTexture m_leftWall3Texture;
	CTexture m_leftWall2Texture;
	CTexture m_leftWall1Texture;
	vector<glm::vec3> m_centrelinePoints;	// Centreline points
	GLuint m_vaoCentreline;
	GLuint m_vaoLeftOffsetCurve;
	GLuint m_vaoRightOffsetCurve;
	GLuint m_vaoTrack;
	GLuint m_vaoRightWall1Surface;
	GLuint m_vaoRightWall2Surface;
	GLuint m_vaoRightWall3Surface;
	GLuint m_vaoBottomWallSurface;
	GLuint m_vaoLeftWall3Surface;
	GLuint m_vaoLeftWall2Surface;
	GLuint m_vaoLeftWall1Surface;


	GLuint m_vaoWallPoints;


	//GLuint m_vaoLeftPipeSurface;
	//GLuint m_vaoRightPipeSurface;

	CVertexBufferObject m_vboCL;
	CVertexBufferObject m_vboLOC;
	CVertexBufferObject m_vboROC;
	CVertexBufferObject m_vboT;


	CVertexBufferObject m_vboWallPoints;
	/*vector <CVertexBufferObject> m_vboLeftPipeVector;
	vector <CVertexBufferObject> m_vboRightPipeVector;*/

	CVertexBufferObject m_vboRightWall1Surface;
	CVertexBufferObject m_vboRightWall2Surface;
	CVertexBufferObject m_vboRightWall3Surface;
	CVertexBufferObject m_vboBottomWallSurface;
	CVertexBufferObject m_vboLeftWall3Surface;
	CVertexBufferObject m_vboLeftWall2Surface;
	CVertexBufferObject m_vboLeftWall1Surface;
	/*CVertexBufferObject m_vboLeftPipeSurface;
	CVertexBufferObject m_vboRightPipeSurface;*/

	static glm::vec3 _dummy_vector;
	vector<glm::vec3> m_controlPoints;		// Control points, which are interpolated to produce the centreline points
	vector<glm::vec3> m_controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors

	vector<glm::vec3> m_centrelineUpVectors;// Centreline upvectors

	vector<glm::vec3> m_leftOffsetPoints;	// Left offset curve points
	vector<glm::vec3> m_rightOffsetPoints;	// Right offset curve points

	vector<vector<glm::vec3>> m_leftWallPoints;	// Left Wall points
	vector<vector<glm::vec3>> m_rightWallPoints;	// Right Wall points

	vector<vector<glm::vec3>> m_pipeCurvePoints;
	/*vector<vector<glm::vec3>> m_rightPipeCurvePoints;
	vector<vector<glm::vec3>> m_leftPipeCurvePoints;*/

	vector<vector<glm::vec3>> m_pipeCurveNormals;


	unsigned int m_vertexCount;				// Number of vertices in the track VBO
	unsigned int m_rightWall1VertexCount;
	unsigned int m_rightWall2VertexCount;
	unsigned int m_rightWall3VertexCount;
	unsigned int m_bottomWallVertexCount;
	unsigned int m_leftWall3VertexCount;
	unsigned int m_leftWall2VertexCount;
	unsigned int m_leftWall1VertexCount;
	/*unsigned int m_rightPipeVertexCount;
	unsigned int m_leftPipeVertexCount;*/

	int num_ctrl_pts = 650;

	float width = 20.f;
};
