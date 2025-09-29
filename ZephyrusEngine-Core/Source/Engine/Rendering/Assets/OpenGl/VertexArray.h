#pragma once

// Vertex data for a simple sprite (positions, normals, texture coordinates)
constexpr float spriteVertices[] = {
	//POSITION                      NORMALS                     TEXCOORDS
	-0.5f, 0.5f, 0.0f,              0.0f, 0.0f, 0.0f,           0.0f, 0.0f,     //top left
	0.5f, 0.5f, 0.0f,               0.0f, 0.0f, 0.0f,           1.0f, 0.0f,     //top right
	-0.5f, -0.5f, 0.0f,             0.0f, 0.0f, 0.0f,           0.0f, 1.0f,     //bottom left
	0.5f, -0.5f, 0.0f,              0.0f, 0.0f, 0.0f,           1.0f, 1.0f,     //bottom right
};

constexpr float fullscreenQuadVertices[] = {
	// POSITION          NORMALS         TEXCOORDS
	-1.0f,  1.0f, 0.0f,  0.0f,0.0f,0.0f,  0.0f, 1.0f, // top-left
	 1.0f,  1.0f, 0.0f,  0.0f,0.0f,0.0f,  1.0f, 1.0f, // top-right
	-1.0f, -1.0f, 0.0f,  0.0f,0.0f,0.0f,  0.0f, 0.0f, // bottom-left
	 1.0f, -1.0f, 0.0f,  0.0f,0.0f,0.0f,  1.0f, 0.0f  // bottom-right
};

// Manages an OpenGL Vertex Array Object (VAO) and its associated Vertex Buffer Object (VBO)
class VertexArray
{
private:
	unsigned int mVerticeCount;

	unsigned int mVertexArrayId;
	unsigned int mVertexBufferId;
public:
	// Creates a vertex array with the given vertices and count
	VertexArray(const float* pVertices, unsigned int pVerticeCount);
	// Releases the resources used by the vertex array
	~VertexArray();

	void SetActive();
	
	unsigned int GetVerticeCount() const
	{
		return mVerticeCount;
	}
};