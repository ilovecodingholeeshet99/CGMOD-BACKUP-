#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "MyMath.h"
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//x-axis
	v.pos.Set(-lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0, 0);	v.color.Set(1, 0, 0);	vertex_buffer_data.push_back(v);
	//y-axis
	v.pos.Set(0, -lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, lengthY, 0);	v.color.Set(0, 1, 0);	vertex_buffer_data.push_back(v);
	//z-axis
	v.pos.Set(0, 0, -lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, lengthZ);	v.color.Set(0, 0, 1);	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}


Mesh* MeshBuilder::GenerateCube(const std::string& meshName, Color
	color, float size)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;	
	std::vector<GLuint> index_buffer_data;
	// Add the vertices here
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 0
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 1
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 2
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 3
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 4
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 5
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 6
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b);vertex_buffer_data.push_back(v); // 7
	
	// Default winding order is Counter-clockwise (CCW)
	// top face
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);


	//Create the bottom face
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);


	// Create the left face
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(1);


	// Create the right face
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(3);

	// Create the front face
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);

	// Create the back face
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(6);
	

	//Create the face
	/*index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);*/


	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

Mesh* MeshBuilder::GenerateGymCube(const std::string& meshName, Color color, float size)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	// Add the vertices here
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 0
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 1
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 2
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 3
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 4
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 5
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 6
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 7

	// Default winding order is Counter-clockwise (CCW)
	// top face
	/*index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);*/


	//Create the bottom face
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);


	// Create the left face
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);


	// Create the right face
	//index_buffer_data.push_back(3);
	//index_buffer_data.push_back(0);
	//index_buffer_data.push_back(4);
	//index_buffer_data.push_back(4);
	//index_buffer_data.push_back(7);
	//index_buffer_data.push_back(3);

	//// Create the front face
	//index_buffer_data.push_back(0);
	//index_buffer_data.push_back(1);
	//index_buffer_data.push_back(5);
	//index_buffer_data.push_back(5);
	//index_buffer_data.push_back(4);
	//index_buffer_data.push_back(0);

	// Create the back face
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(7);


	//Create the face
	/*index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(4);*/


	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}


Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, unsigned stackCount, unsigned sliceCount, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	// Add the vertices for the spheres
	
	
	// Define the triangles that make up the surface of the spheres  
	float anglePerSlice = Math::TWO_PI / sliceCount;

	// Fill the underlined part with necessary variables
	float anglePerStack = Math::PI /stackCount;

	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);


	for (unsigned stack = 0; stack <= stackCount; stack++) {
		for (unsigned slice = 0; slice <= sliceCount; slice++) {
			// Find theta (Fill the underlined part with necessary code)
			float theta = anglePerStack * stack;
			// Find phi (Fill the underlined part with necessary code)
			float phi = anglePerSlice * slice;
			// Compute the vertices using the formulae given to find x, yand z(Fill the underlined area)
			v.pos.Set(radius * cos(phi) * sin(theta), radius * sin(phi) * sin(theta ), radius * cos(theta));
			v.color.Set(color.r, color.g, color.b);
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < stackCount; stack++) {
		// Find phi (Fill the underlined part with necessary code)
		int k1 = stack * (sliceCount + 1);
		// Find phi (Fill the underlined part with necessary code)
		int k2 = k1 + sliceCount + 1;
		for (unsigned slice = 0; slice < sliceCount; slice++) {
			// Form the faces required as seen in the diagram on step 7
				// Fill the underlined part with necessary code
			index_buffer_data.push_back(k1);
			index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k1+1);
			index_buffer_data.push_back(k1+1);
			index_buffer_data.push_back(k2);
			index_buffer_data.push_back(k2+1);
			k1++;
			k2++;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);


	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}

Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, unsigned res, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float AngleHeisenberg = Math::TWO_PI / res;
	// Add the vertices for the spheres
	v.pos.Set(0, 0, 0);
	v.color.Set(color.r, color.g, color.b); 
	vertex_buffer_data.push_back(v); // 0
	for (int i = 0; i < res+1; i++)
	{
		v.pos.Set(sin(AngleHeisenberg * i), 0, cos(AngleHeisenberg * i)); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 0
	}

	// Define the triangles that make up the surface of the spheres
	for (int j = 0; j < res; j++)
	{
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(j+1);
		index_buffer_data.push_back(j+2);
	}
	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
	return nullptr;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, unsigned res, float height , float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float AngleHeisenberg = Math::TWO_PI / res;
	// Add the vertices for the spheres
	v.pos.Set(0, 0, 0);
	v.color.Set(color.r, color.g, color.b);
	vertex_buffer_data.push_back(v); // 0
	for (int i = 0; i < res + 1; i++)
	{	
		v.pos.Set(sin(AngleHeisenberg * i), 0, cos(AngleHeisenberg * i)); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // 0
	}
	v.pos.Set(0, height, 0);
	v.color.Set(color.r, color.g, color.b);
	vertex_buffer_data.push_back(v); // 0
	for (int i = 0; i < res; i++)
	{
		index_buffer_data.push_back(res+2);
		index_buffer_data.push_back(i+1);
		index_buffer_data.push_back(i+2);

	}
	// Define the triangles that make up the surface of the spheres
	for (int j = 0; j < res; j++)
	{
		index_buffer_data.push_back(0);
		index_buffer_data.push_back(j +2 );
		index_buffer_data.push_back(j + 1);
	}
	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, unsigned res, float depth, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float angle = Math::TWO_PI / res;

	v.pos.Set(0, depth, 0); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	for (int i = 1; i <= res; i++)
	{
		v.pos.Set(sin(angle * i) * radius, depth, cos(angle * i) * radius); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	}
	v.pos.Set(0, -depth, 0); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	for (int i = 1; i <= res; i++)
	{
		v.pos.Set(sin(angle * i) * radius, -depth, cos(angle * i) * radius); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v);
	}

	for (int i = 0; i < res; i++)
	{
		if (i < (res - 1))
		{
			index_buffer_data.push_back(0);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + 2);
			index_buffer_data.push_back(res + 1);
			index_buffer_data.push_back(res + 3 + i);
			index_buffer_data.push_back(res + 2 + i);
		}
		else
		{
			index_buffer_data.push_back(0);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(res + 1);
			index_buffer_data.push_back(res + 2);
			index_buffer_data.push_back(res + 2 + i);
		}
	}
	for (int i = 1; i <= res; i++)
	{
		if (i < res)
		{
			index_buffer_data.push_back(i);
			index_buffer_data.push_back(i + res + 1);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + 1);
			index_buffer_data.push_back(i + res + 1);
			index_buffer_data.push_back((i + res + 1) + 1);
		}
		else
		{
			index_buffer_data.push_back(res);
			index_buffer_data.push_back(res + res + 1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(1);
			index_buffer_data.push_back(res + res + 1);
			index_buffer_data.push_back(res + 2);
		}
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;

}


/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	// Add the vertices here
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-right of quad
	v.pos.Set(-0.5f, -0.5f, 0.5f);v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // top-left of quad
	v.pos.Set(-.5f, -0.5f, -0.5f);v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-left of quad
	v.pos.Set(0.5f, -0.5f, -0.5f);v.color.Set(color.r, color.g, color.b); vertex_buffer_data.push_back(v); // bottom-right of quad


	// Default winding order is Counter clockwise for opengl
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(2);


	// Create the new mesh
	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;
	return mesh;
}

