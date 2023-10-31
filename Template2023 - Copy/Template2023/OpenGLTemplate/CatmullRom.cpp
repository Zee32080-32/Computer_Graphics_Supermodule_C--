#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
	m_rightWall1VertexCount = 0;
	m_rightWall2VertexCount = 0;
	m_rightWall3VertexCount = 0;
	m_bottomWallVertexCount = 0;
	m_leftWall3VertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{
	Release();
}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f * p1 - 3.0f * p2 + p3);

	return a + b * t + c * t2 + d * t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here

	//Set upvectors (m_controlUpVectors, one for each control point as well)

	m_controlPoints.push_back(glm::vec3(220, 15, 0));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(170, 20, 30) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(230, 20, 80));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(160, 40, 80) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(220, 40, 150));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(180, 40, 80) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(71, 10, 71));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, 1, 0))); // glm::normalize(glm::vec3(0, 10, 50) - m_controlPoints[4])
	m_controlPoints.push_back(glm::vec3(0, 60, 20));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, 1, 0)));
	m_controlPoints.push_back(glm::vec3(-11, 50, 80));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(-43, 50, 80) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-18, 35, 150));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, -1, 0)));
	m_controlPoints.push_back(glm::vec3(-22, 20, 220));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(12, 20, 225) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-55, 15, 280));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, 0, 1)));
	m_controlPoints.push_back(glm::vec3(-90, 30, 220));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(-80, 40, 240) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-140, 90, 160));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(-135, 60, 190) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-140, 140, 240));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, -1, 0)));
	m_controlPoints.push_back(glm::vec3(-120, 50, 300));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(-200, 50, 200) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-200, 20, 200));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(-160, 10, 180) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-220, 15, 90));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(-180, 40, 100) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-175, 15, -20));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(-140, 10, 20) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(-130, 15, -85));	m_controlUpVectors.push_back(glm::normalize(glm::vec3(-71, 10, 2) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(0, 15, -100));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(0, 20, -50) - m_controlPoints[m_controlPoints.size() - 1]));
	m_controlPoints.push_back(glm::vec3(120, 15, -71));		m_controlUpVectors.push_back(glm::normalize(glm::vec3(100, 20, -30) - m_controlPoints[m_controlPoints.size() - 1]));

}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int)m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i - 1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M - 1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3& p, glm::vec3& up)
{
	if (d < 0)
		return false;

	int M = (int)m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int)(d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size() - 1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;

	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j - 1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}



// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}




void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(num_ctrl_pts);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);
	// Create a VBO
	m_vboCL.Create();
	m_vboCL.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		m_vboCL.AddData(&m_centrelinePoints[i], sizeof(glm::vec3));
		m_vboCL.AddData(&texCoord, sizeof(glm::vec2));
		m_vboCL.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
	}
	// Upload the VBO to the GPU
	m_vboCL.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::CreateOffsetBounds()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.

	glGenVertexArrays(1, &m_vaoLeftOffsetCurve);
	glBindVertexArray(m_vaoLeftOffsetCurve);

	m_vboLOC.Create();
	m_vboLOC.Bind();


	glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);


	glm::vec3 pointNext;

	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {

		pointNext = m_centrelinePoints[(i + 1) % m_centrelinePoints.size()];


		glm::vec3 viewDir = glm::normalize(pointNext - m_centrelinePoints[i]);

		glm::vec3 eyeT = viewDir;
		glm::vec3 eyeN = glm::normalize(glm::cross(eyeT, m_centrelineUpVectors[i]));
		glm::vec3 eyeB = glm::normalize(glm::cross(eyeT, eyeN));
		glm::vec3 offset = (width / 2.f) * eyeN;
		glm::vec3 lBorder = m_centrelinePoints[i] - offset;
		glm::vec3 wallRightTop = lBorder - ((width / 8.f) * eyeN) - ((width / 8.f) * eyeB);
		glm::vec3 wallLeftTop = lBorder - (width / 4.f) * eyeN - (width / 8.f) * eyeB;
		glm::vec3 wallBottom = wallLeftTop + (width / 4.f) * eyeB;

		vector<glm::vec3> new_vec;
		m_leftWallPoints.push_back(new_vec);
		m_leftWallPoints[i].push_back(wallRightTop);
		m_leftWallPoints[i].push_back(wallLeftTop);
		m_leftWallPoints[i].push_back(wallBottom);

		m_leftOffsetPoints.push_back(lBorder);



		m_vboLOC.AddData(&lBorder, sizeof(glm::vec3));
		m_vboLOC.AddData(&texCoord, sizeof(glm::vec2));
		m_vboLOC.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
	}

	m_vboLOC.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));

	glGenVertexArrays(1, &m_vaoRightOffsetCurve);
	glBindVertexArray(m_vaoRightOffsetCurve);


	m_vboROC.Create();
	m_vboROC.Bind();

	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {

		if (i + 1 >= m_centrelinePoints.size()) {
			pointNext = m_centrelinePoints[0];
		}

		else {
			pointNext = m_centrelinePoints[i + 1];
		}

		glm::vec3 viewDir = glm::normalize(pointNext - m_centrelinePoints[i]);

		glm::vec3 eyeT = viewDir;
		glm::vec3 eyeN = glm::normalize(glm::cross(eyeT, m_centrelineUpVectors[i]));
		glm::vec3 eyeB = glm::normalize(glm::cross(eyeT, eyeN));
		glm::vec3 offset = (width / 2.f) * eyeN;
		glm::vec3 og_vec = m_centrelinePoints[i];
		glm::vec3 rBorder = m_centrelinePoints[i] + offset;

		glm::vec3 wallRightTop = rBorder + (width / 4.f) * eyeN - (width / 8.f) * eyeB;
		glm::vec3 wallLeftTop = rBorder + (width / 8.f) * eyeN - (width / 8.f) * eyeB;
		glm::vec3 wallBottom = wallRightTop + (width / 4.f) * eyeB;

		vector<glm::vec3> new_vec;
		m_rightWallPoints.push_back(new_vec);
		m_rightWallPoints[i].push_back(wallLeftTop);
		m_rightWallPoints[i].push_back(wallRightTop);
		m_rightWallPoints[i].push_back(wallBottom);

		m_rightOffsetPoints.push_back(rBorder);

		m_vboROC.AddData(&rBorder, sizeof(glm::vec3));
		m_vboROC.AddData(&texCoord, sizeof(glm::vec2));
		m_vboROC.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
	}

	m_vboROC.UploadDataToGPU(GL_STATIC_DRAW);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateWallPoints() {
	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card
	glGenVertexArrays(1, &m_vaoWallPoints);
	glBindVertexArray(m_vaoWallPoints);
	// Create a VBO
	m_vboWallPoints.Create();
	m_vboWallPoints.Bind();
	glm::vec2 texCoord(0.0f, 0.0f);
	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		for (int w = 0; w < m_leftWallPoints[i].size(); w++) {
			m_vboWallPoints.AddData(&m_leftWallPoints[i][w], sizeof(glm::vec3));
			m_vboWallPoints.AddData(&texCoord, sizeof(glm::vec2));
			m_vboWallPoints.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
		}

		for (int w = m_rightWallPoints[i].size() - 1; w > -1; w--) {
			m_vboWallPoints.AddData(&m_rightWallPoints[i][w], sizeof(glm::vec3));
			m_vboWallPoints.AddData(&texCoord, sizeof(glm::vec2));
			m_vboWallPoints.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
		}

	}
	// Upload the VBO to the GPU
	m_vboWallPoints.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::CreateTrack(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	m_vboT.Create();
	m_vboT.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;

	for (int i = 0; i < m_leftOffsetPoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);

		m_vboT.AddData(&m_leftOffsetPoints[i], sizeof(glm::vec3));
		m_vboT.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboT.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
		m_vertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboT.AddData(&m_rightOffsetPoints[i], sizeof(glm::vec3));
		m_vboT.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboT.AddData(&m_centrelineUpVectors[i], sizeof(glm::vec3));
		m_vertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboT.AddData(&m_leftOffsetPoints[(i + 1) % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		m_vboT.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboT.AddData(&m_centrelineUpVectors[(i + 1) % m_leftOffsetPoints.size()], sizeof(glm::vec3));
		m_vertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboT.AddData(&m_rightOffsetPoints[(i + 1) % m_rightOffsetPoints.size()], sizeof(glm::vec3));
		m_vboT.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboT.AddData(&m_centrelineUpVectors[(i + 1) % m_rightOffsetPoints.size()], sizeof(glm::vec3));
		m_vertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboT.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateRightWall1Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_rightWall1Texture.Load(filename);
	m_rightWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_rightWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_rightWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_rightWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoRightWall1Surface);
	glBindVertexArray(m_vaoRightWall1Surface);

	m_vboRightWall1Surface.Create();
	m_vboRightWall1Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_rightOffsetPoints[i];
		glm::vec3 v1 = m_rightWallPoints[i][0];
		glm::vec3 v2 = m_rightOffsetPoints[(i + 1) % m_rightOffsetPoints.size()];
		glm::vec3 v3 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][0];

		// Right Wall Face 1
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboRightWall1Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboRightWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboRightWall1Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboRightWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboRightWall1Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboRightWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboRightWall1Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboRightWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall1VertexCount += 1;



		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboRightWall1Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateRightWall2Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_rightWall2Texture.Load(filename);
	m_rightWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_rightWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_rightWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_rightWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoRightWall2Surface);
	glBindVertexArray(m_vaoRightWall2Surface);

	m_vboRightWall2Surface.Create();
	m_vboRightWall2Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_rightWallPoints[i][0];
		glm::vec3 v1 = m_rightWallPoints[i][1];
		glm::vec3 v2 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][0];
		glm::vec3 v3 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][1];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboRightWall2Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboRightWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboRightWall2Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboRightWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboRightWall2Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboRightWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboRightWall2Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboRightWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall2VertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboRightWall2Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}


void CCatmullRom::CreateRightWall3Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_rightWall3Texture.Load(filename);
	m_rightWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_rightWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_rightWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_rightWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoRightWall3Surface);
	glBindVertexArray(m_vaoRightWall3Surface);

	m_vboRightWall3Surface.Create();
	m_vboRightWall3Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_rightWallPoints[i][1];
		glm::vec3 v1 = m_rightWallPoints[i][2];
		glm::vec3 v2 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][1];
		glm::vec3 v3 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][2];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboRightWall3Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboRightWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboRightWall3Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboRightWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboRightWall3Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboRightWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboRightWall3Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboRightWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboRightWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_rightWall3VertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboRightWall3Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateBottomWallSurface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_bottomWallTexture.Load(filename);
	m_bottomWallTexture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_bottomWallTexture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_bottomWallTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_bottomWallTexture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoBottomWallSurface);
	glBindVertexArray(m_vaoBottomWallSurface);

	m_vboBottomWallSurface.Create();
	m_vboBottomWallSurface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_rightWallPoints[i][2];
		glm::vec3 v1 = m_leftWallPoints[i][2];
		glm::vec3 v2 = m_rightWallPoints[(i + 1) % m_rightOffsetPoints.size()][2];
		glm::vec3 v3 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][2];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboBottomWallSurface.AddData(&v0, sizeof(glm::vec3));
		m_vboBottomWallSurface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboBottomWallSurface.AddData(&normal, sizeof(glm::vec3));
		m_bottomWallVertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboBottomWallSurface.AddData(&v1, sizeof(glm::vec3));
		m_vboBottomWallSurface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboBottomWallSurface.AddData(&normal, sizeof(glm::vec3));
		m_bottomWallVertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboBottomWallSurface.AddData(&v2, sizeof(glm::vec3));
		m_vboBottomWallSurface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboBottomWallSurface.AddData(&normal, sizeof(glm::vec3));
		m_bottomWallVertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboBottomWallSurface.AddData(&v3, sizeof(glm::vec3));
		m_vboBottomWallSurface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboBottomWallSurface.AddData(&normal, sizeof(glm::vec3));
		m_bottomWallVertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboBottomWallSurface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateLeftWall3Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_leftWall3Texture.Load(filename);
	m_leftWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_leftWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_leftWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_leftWall3Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoLeftWall3Surface);
	glBindVertexArray(m_vaoLeftWall3Surface);

	m_vboLeftWall3Surface.Create();
	m_vboLeftWall3Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_leftWallPoints[i][2];
		glm::vec3 v1 = m_leftWallPoints[i][1];
		glm::vec3 v2 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][2];
		glm::vec3 v3 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][1];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboLeftWall3Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboLeftWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboLeftWall3Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboLeftWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboLeftWall3Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboLeftWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall3VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboLeftWall3Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboLeftWall3Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall3Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall3VertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboLeftWall3Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateLeftWall2Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_leftWall2Texture.Load(filename);
	m_leftWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_leftWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_leftWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_leftWall2Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoLeftWall2Surface);
	glBindVertexArray(m_vaoLeftWall2Surface);

	m_vboLeftWall2Surface.Create();
	m_vboLeftWall2Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_leftWallPoints[i][1];
		glm::vec3 v1 = m_leftWallPoints[i][0];
		glm::vec3 v2 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][1];
		glm::vec3 v3 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][0];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboLeftWall2Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboLeftWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboLeftWall2Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboLeftWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboLeftWall2Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboLeftWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall2VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboLeftWall2Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboLeftWall2Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall2Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall2VertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboLeftWall2Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::CreateLeftWall1Surface(string filename)
{

	// Generate a VAO called m_vaoTrack and a VBO to get the offset curve points and indices on the graphics card

	m_leftWall1Texture.Load(filename);
	m_leftWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	m_leftWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_leftWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_leftWall1Texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenVertexArrays(1, &m_vaoLeftWall1Surface);
	glBindVertexArray(m_vaoLeftWall1Surface);

	m_vboLeftWall1Surface.Create();
	m_vboLeftWall1Surface.Bind();

	std::vector<glm::vec2> texture_vec = { glm::vec2(0, 0) , glm::vec2(1, 0) , glm::vec2(0, 1) , glm::vec2(1, 1) };
	//glm::vec2 texCoord(0.0f, 0.0f);
	//glm::vec3 normal(0.0f, 1.0f, 0.0f);
	float tex_face = 1;
	float tex_perc = 0.1;
	float base_y = 0.f;
	float top_y;



	for (int i = 0; i < m_centrelinePoints.size(); i++) {
		top_y = tex_face * tex_perc;
		glm::vec2 debug_tex = glm::vec2(texture_vec[0].x, texture_vec[0].y + base_y);
		glm::vec3 normal;

		glm::vec3 v0 = m_leftWallPoints[i][0];
		glm::vec3 v1 = m_leftOffsetPoints[i];
		glm::vec3 v2 = m_leftWallPoints[(i + 1) % m_rightOffsetPoints.size()][0];
		glm::vec3 v3 = m_leftOffsetPoints[(i + 1) % m_rightOffsetPoints.size()];

		// Right Wall Face 2
		normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vboLeftWall1Surface.AddData(&v0, sizeof(glm::vec3));
		m_vboLeftWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[1].x, texture_vec[1].y + base_y);
		m_vboLeftWall1Surface.AddData(&v1, sizeof(glm::vec3));
		m_vboLeftWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[2].x, texture_vec[2].y * top_y);
		m_vboLeftWall1Surface.AddData(&v2, sizeof(glm::vec3));
		m_vboLeftWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall1VertexCount += 1;

		debug_tex = glm::vec2(texture_vec[3].x, texture_vec[3].y * top_y);
		m_vboLeftWall1Surface.AddData(&v3, sizeof(glm::vec3));
		m_vboLeftWall1Surface.AddData(&debug_tex, sizeof(glm::vec2));
		m_vboLeftWall1Surface.AddData(&normal, sizeof(glm::vec3));
		m_leftWall1VertexCount += 1;

		tex_face += 1.f;
		base_y = top_y;

		if (tex_face * tex_perc > 1.f) {
			tex_face = 1.f;
			base_y = 0.f;
		}
	}

	m_vboLeftWall1Surface.UploadDataToGPU(GL_STATIC_DRAW);
	// Set the vertex attribute locations
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)
		+ sizeof(glm::vec2)));
}

void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoCentreline);

	glLineWidth(6.f);
	glPointSize(5.f);
	//glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());

}

void CCatmullRom::RenderOffsetCurves()
{
	glPointSize(8.f);
	// Bind the VAO m_vaoLeftOffsetCurve and render it
	glBindVertexArray(m_vaoLeftOffsetCurve);
	//glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());

	// Bind the VAO m_vaoRightOffsetCurve and render it

	glPointSize(3.f);
	glBindVertexArray(m_vaoRightOffsetCurve);
	//glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
}


void CCatmullRom::RenderTrack()
{

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	m_texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);

	//glPointSize(20.f);
	//glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
}

void CCatmullRom::RenderWallPoints()
{
	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoWallPoints);

	//glLineWidth(6.f);
	glPointSize(5.f);
	//glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());
	glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size() * 6);

}

void CCatmullRom::RenderRightWall1Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoRightWall1Surface);
	m_rightWall1Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightWall1VertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderRightWall2Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoRightWall2Surface);
	m_rightWall2Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightWall2VertexCount);

	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderRightWall3Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoRightWall3Surface);
	m_rightWall3Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_rightWall3VertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderBottomWallSurface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoBottomWallSurface);
	m_bottomWallTexture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_bottomWallVertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderLeftWall3Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoLeftWall3Surface);
	m_leftWall3Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftWall3VertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderLeftWall2Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoLeftWall2Surface);
	m_leftWall2Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftWall2VertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

void CCatmullRom::RenderLeftWall1Surface()
{
	/*glLineWidth(2.f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);*/

	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoLeftWall1Surface);
	m_leftWall1Texture.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_leftWall1VertexCount);
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);*/
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

void CCatmullRom::Release()
{
	m_texture.Release();
	m_rightWall1Texture.Release();
	m_rightWall2Texture.Release();
	m_rightWall3Texture.Release();
	glDeleteVertexArrays(1, &m_vaoTrack);
	glDeleteVertexArrays(1, &m_vaoLeftOffsetCurve);
	glDeleteVertexArrays(1, &m_vaoRightOffsetCurve);
	glDeleteVertexArrays(1, &m_vaoCentreline);
	glDeleteVertexArrays(1, &m_vaoRightWall1Surface);
	glDeleteVertexArrays(1, &m_vaoRightWall2Surface);
	glDeleteVertexArrays(1, &m_vaoRightWall3Surface);
	glDeleteVertexArrays(1, &m_vaoWallPoints);
	m_vboROC.Release();
	m_vboLOC.Release();
	m_vboCL.Release();
	m_vboT.Release();
	m_vboRightWall1Surface.Release();
	m_vboRightWall2Surface.Release();
	m_vboRightWall3Surface.Release();
	m_vboBottomWallSurface.Release();
	m_vboLeftWall3Surface.Release();
	//m_vboLeftWall2Surface.Release();
	m_vboWallPoints.Release();
}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);